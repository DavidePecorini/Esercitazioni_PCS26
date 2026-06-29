#include "test_framework.hpp"
#include "../include/circuit/input.hpp"
#include "../include/circuit/solver.hpp"
#include "../include/cycles/de_pina.hpp"
#include <fstream>
#include <cstdio>
#include <map>

// Esegue l'intera pipeline, identica al main: scrive il circuito su file, lo
// legge con read_input (cosi' e' coperta anche la convenzione di segno sui
// generatori), costruisce il grafo, trova le maglie con De Pina, assembla e
// risolve il sistema, estrae le correnti di ramo. Restituisce le correnti e,
// tramite il parametro potenza, la potenza dissipata totale.
std::map<int, double> risolvi_circuito(const std::string& contenuto, double& potenza)
{
	const char* percorso = "/tmp/_test_circuit_e2e.txt";
	std::ofstream ofs(percorso);
	ofs << contenuto;
	ofs.close();

	char arg0[] = "prog";
	char arg1[] = "/tmp/_test_circuit_e2e.txt";
	char* argv[] = {arg0, arg1};

	auto [lista, nr] = read_input(2, argv);
	unidirected_graph<int> g = graph_builder(lista);
	std::vector<std::vector<int>> maglie = de_pina(g);
	mesh_system sys = build_system(lista, nr, maglie, g);
	auto [I, it] = solve_mesh_currents(sys);
	std::map<int, double> correnti = compute_branch_currents(lista, maglie, g, I);

	// Potenza dissipata totale = somma di R * I^2 (invariante fisico,
	// indipendente da segni e orientamenti)
	potenza = 0.0;
	for (const auto& c : lista)
	{
		if (c.tipo == 'R')
		{
			double corrente = correnti.at(c.id);
			potenza += c.valore * corrente * corrente;
		}
	}
	std::remove(percorso);
	return correnti;
}

// Valore assoluto di una corrente (confronto i moduli: il segno dipende
// dall'orientamento interno delle maglie, il modulo no)
double modulo(double x)
{
	if (x < 0)
	{
		return -x;
	}
	return x;
}

int main()
{
	const double tol = 1e-6;
	double potenza;

	sezione("Circuito serie (verifica a mano: I = 10/10 = 1 A)");
	{
		std::map<int, double> I = risolvi_circuito("V 1 10 1 2\nR 1 5 2 3\nR 2 5 3 1\n", potenza);
		verifica_vicino(modulo(I.at(1)), 1.0, tol, "corrente in R1");
		verifica_vicino(modulo(I.at(2)), 1.0, tol, "corrente in R2");
		verifica_vicino(potenza, 10.0, tol, "potenza totale");
	}

	sezione("Circuito a ponte (verifica con analisi nodale indipendente)");
	{
		std::map<int, double> I = risolvi_circuito(
			"V 1 10 1 2\nR 1 5 2 3\nR 2 10 3 4\nR 3 20 4 1\nR 4 8 1 3\n", potenza);
		verifica_vicino(modulo(I.at(1)), 0.883721, tol, "corrente in R1");
		verifica_vicino(modulo(I.at(2)), 0.186047, tol, "corrente in R2");
		verifica_vicino(modulo(I.at(3)), 0.186047, tol, "corrente in R3");
		verifica_vicino(modulo(I.at(4)), 0.697674, tol, "corrente in R4");
		verifica_vicino(potenza, 8.837209, tol, "potenza totale");
	}

	sezione("Circuito due rami paralleli (verifica a mano: 12V/6ohm = 2 A per ramo)");
	{
		std::map<int, double> I = risolvi_circuito(
			"V 1 12 1 2\nR 1 2 2 3\nR 2 4 3 1\nR 3 3 2 4\nR 4 3 4 1\n", potenza);
		verifica_vicino(modulo(I.at(1)), 2.0, tol, "corrente in R1");
		verifica_vicino(modulo(I.at(2)), 2.0, tol, "corrente in R2");
		verifica_vicino(modulo(I.at(3)), 2.0, tol, "corrente in R3");
		verifica_vicino(modulo(I.at(4)), 2.0, tol, "corrente in R4");
		verifica_vicino(potenza, 48.0, tol, "potenza totale");
	}

	return riepilogo();
}
