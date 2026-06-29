#include "circuit/input.hpp"
#include "circuit/solver.hpp"
#include "cycles/de_pina.hpp"
#include "cycles/dfs_cycles.hpp"
#include <Eigen/Dense>
#include <iostream>

int main(int argc, char **argv)
{
	// ----- 1. Lettura input -----
	auto [lista_componenti, nr] = read_input(argc, argv);

	// Controllo che la lista non sia vuoto
	if (lista_componenti.empty()) {
		return 1;
	}
	std::cout << "Netlist in input: \n" << std::endl;
	for (const component& c : lista_componenti)
	{
		std::cout << c.tipo << c.id << " " << c.valore << " "
				  << c.nodo1 << " " << c.nodo2 << std::endl;
	}

	// ----- 2. Costruzione del grafo del circuito -----
	unidirected_graph<int> grafo_circuito = graph_builder(lista_componenti);
	
	std::cout << "\nGrafo: \n" << std::endl;
	
	for (const unidirected_edge<int>& arco : grafo_circuito.all_edges())
	{
		std::cout << arco;
	}

	// ----- 3. Base di cicli minima (De Pina) -----
	std::vector<std::vector<int>> maglie = de_pina(grafo_circuito);
	
	std::cout << "\nBase cicli minimi: \n" << std::endl;
	
	for (const auto& maglia : maglie)
	{
		std::cout << "(";
		for (size_t k = 0; k + 1 < maglia.size(); k++)
		{
			std::cout << maglia[k] << ", ";
		}
		std::cout << maglia.back() << ")" << std::endl;
	}

	// ----- 4. Sistema alle maglie e soluzione -----
	mesh_system sys = build_system(lista_componenti, nr, maglie, grafo_circuito);
	auto [I, it] = solve_mesh_currents(sys);
	
	// Usiamo l'operatore di stamopa già presente in Eigen
	// R viene ricostruita come matrice diagonale solo per la stampa.
	std::cout << "\nMatrici ottenute:\n" << std::endl;
	std::cout << Eigen::MatrixXd(sys.r_diag.asDiagonal()) << "\n\n";
	std::cout << sys.B << "\n";

	// ----- Confronto del condizionamento della matrice B -----
	// La base di cicli minima di De Pina produce cicli corti, quindi una B con
	// colonne più sparse e meno sovrapposte: tende a essere meglio condizionata
	// (kappa più piccolo) della base fondamentale ottenuta dall'albero DFS, i cui
	// cicli possono essere lunghi. Un kappa più piccolo aiuta la convergenza del
	// gradiente coniugato. Costruisco la B della base DFS solo per il confronto.
	std::vector<std::vector<int>> maglie_dfs = cicli_fondamentali_dfs(grafo_circuito);
	mesh_system sys_dfs = build_system(lista_componenti, nr, maglie_dfs, grafo_circuito);

	double k_depina = condizionamento(sys.B);
	double k_dfs = condizionamento(sys_dfs.B);

	std::cout << "\nCondizionamento di B - De Pina: " << k_depina
			  << " | DFS: " << k_dfs << std::endl;
	// Confronto con tolleranza relativa: su circuiti piccoli i due valori possono
	// coincidere a meno del rumore di arrotondamento, e non va spacciato per un
	// miglioramento. Considero De Pina migliore solo se kappa è sensibilmente minore.
	if (k_dfs > k_depina * (1.0 + 1e-9))
	{
		std::cout << "La base di De Pina e' meglio condizionata della base DFS." << std::endl;
	}
	else
	{
		std::cout << "Su questo circuito le due basi coincidono (in generale De Pina <= DFS)." << std::endl;
	}

	// ----- 5. Report -----
	std::cout << "\nRisultati:" << std::endl;
	report_results(lista_componenti, maglie, grafo_circuito, I);
	

	return 0;
}
