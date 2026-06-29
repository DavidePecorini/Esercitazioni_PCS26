#include "test_framework.hpp"
#include "../include/cycles/de_pina.hpp"
#include <vector>
#include <map>

// --- Strumenti di verifica, indipendenti dall'algoritmo testato ---

// Conta gli archi effettivamente usati da un ciclo con segno (valore diverso da 0)
int conta_non_nulli(const std::vector<int>& C)
{
	int n = 0;
	for (size_t t = 0; t < C.size(); t++)
	{
		if (C[t] != 0)
		{
			n++;
		}
	}
	return n;
}

// Verifica che un vettore con segno C sia una vera circolazione sul grafo,
// cioe' che soddisfi la legge dei nodi di Kirchhoff: in ogni nodo i flussi
// entranti e uscenti si devono annullare. Con la convenzione "+C sull'arco
// from->to" il bilancio di un nodo e':
//   somma di C sugli archi che escono dal nodo (from == nodo)
//   meno la somma di C sugli archi che entrano (to == nodo)
// Questo controlla DIRETTAMENTE la logica di De Pina (segni e indici degli
// archi), non Dijkstra.
template<typename T>
bool e_circolazione(const unidirected_graph<T>& g, const std::vector<int>& C)
{
	std::map<T, int> bilancio;
	std::map<unidirected_edge<T>, int> indici = g.edge_index_map();

	for (const auto& arco : g.all_edges())
	{
		int c = C[indici.at(arco)];
		bilancio[arco.from()] += c;
		bilancio[arco.to()] -= c;
	}
	for (const auto& coppia : bilancio)
	{
		if (coppia.second != 0)
		{
			return false;
		}
	}
	return true;
}

// Rango in GF(2) di una matrice di vettori booleani, per eliminazione gaussiana.
// Serve a verificare che i cicli trovati siano linearmente indipendenti, cioe'
// che formino davvero una base dello spazio dei cicli.
int rango_gf2(std::vector<std::vector<bool>> M)
{
	if (M.empty())
	{
		return 0;
	}
	int righe = M.size();
	int colonne = M[0].size();
	int rango = 0;

	for (int col = 0; col < colonne && rango < righe; col++)
	{
		// Cerco una riga, da rango in poi, con un 1 in questa colonna (il pivot)
		int pivot = -1;
		for (int r = rango; r < righe; r++)
		{
			if (M[r][col])
			{
				pivot = r;
				break;
			}
		}
		if (pivot == -1)
		{
			continue;
		}
		std::swap(M[rango], M[pivot]);

		// Azzero la colonna nelle altre righe sommando (XOR) la riga pivot
		for (int r = 0; r < righe; r++)
		{
			if (r != rango && M[r][col])
			{
				for (int c = 0; c < colonne; c++)
				{
					M[r][c] = M[r][c] != M[rango][c];
				}
			}
		}
		rango++;
	}
	return rango;
}

// Esegue tutti i controlli strutturali su un grafo dato
template<typename T>
void verifica_base_cicli(const unidirected_graph<T>& g, const std::string& nome)
{
	int n = g.all_nodes().size();
	int m = g.all_edges().size();
	// Per un grafo connesso la dimensione dello spazio dei cicli e' m - n + 1
	int dim_attesa = m - n + 1;

	std::cout << "  (" << nome << ": n=" << n << " m=" << m
			  << " dim attesa=" << dim_attesa << ")\n";

	std::vector<std::vector<int>> cicli = de_pina(g);

	// 1. Numero corretto di cicli (confronto due int, senza cast)
	int numero_cicli = cicli.size();
	verifica(numero_cicli == dim_attesa, nome + ": numero di cicli pari a m - n + 1");

	// 2. Ogni ciclo e' una circolazione valida e non banale
	std::vector<std::vector<bool>> cicli_bool;
	for (size_t i = 0; i < cicli.size(); i++)
	{
		const std::vector<int>& C = cicli[i];
		verifica(e_circolazione(g, C), nome + ": il ciclo e' una circolazione valida (N*C = 0)");
		verifica(conta_non_nulli(C) > 0, nome + ": il ciclo non e' nullo");

		std::vector<bool> b(C.size(), false);
		for (size_t t = 0; t < C.size(); t++)
		{
			b[t] = (C[t] != 0);
		}
		cicli_bool.push_back(b);
	}

	// 3. I cicli sono indipendenti in GF(2): formano una base
	verifica(rango_gf2(cicli_bool) == dim_attesa, nome + ": i cicli sono indipendenti (base)");
}

int main()
{
	sezione("De Pina - proprieta strutturali");

	// Triangolo: 1 solo ciclo
	{
		unidirected_graph<int> g;
		g.add_edge(unidirected_edge<int>(1, 2));
		g.add_edge(unidirected_edge<int>(2, 3));
		g.add_edge(unidirected_edge<int>(1, 3));
		verifica_base_cicli(g, "triangolo");
	}

	// Circuito esempio (ponte): 2 cicli
	{
		unidirected_graph<int> g;
		g.add_edge(unidirected_edge<int>(1, 2));
		g.add_edge(unidirected_edge<int>(2, 3));
		g.add_edge(unidirected_edge<int>(3, 4));
		g.add_edge(unidirected_edge<int>(1, 4));
		g.add_edge(unidirected_edge<int>(1, 3));
		verifica_base_cicli(g, "ponte");
	}

	// Circuito due rami paralleli: 2 cicli
	{
		unidirected_graph<int> g;
		g.add_edge(unidirected_edge<int>(1, 2));
		g.add_edge(unidirected_edge<int>(2, 3));
		g.add_edge(unidirected_edge<int>(1, 3));
		g.add_edge(unidirected_edge<int>(2, 4));
		g.add_edge(unidirected_edge<int>(1, 4));
		verifica_base_cicli(g, "due rami");
	}

	sezione("De Pina - conta_archi");
	// conta_archi deve contare anche gli archi percorsi in verso discorde (-1),
	// non solo i +1
	verifica(conta_archi({1, -1, 0, 1, -1}) == 4, "conta_archi conta sia +1 che -1");
	verifica(conta_archi({0, 0, 0}) == 0, "conta_archi di un vettore nullo da' 0");
	verifica(conta_archi({-1, -1, -1}) == 3, "conta_archi conta i -1");

	return riepilogo();
}
