#pragma once
#include "../graph/visit.hpp"
#include <vector>
#include <set>


template<typename T>
bool finding_path(const unidirected_graph<T>& tree, const T& u, const T& v,
				  std::vector<T>& path, std::set<T>& visited)
{
	visited.insert(u);
	path.push_back(u);

	if (u == v) {
		return true;
	}

	for (const auto& neighbour : tree.neighbours(u)) {
		if (!visited.count(neighbour))
		{
			if (finding_path(tree, neighbour, v, path, visited))
			{
				return true;
			}
		}
	}
	// Backtracking: u non porta a v, lo tolgo dal cammino
	path.pop_back();
	return false;
}

template<typename T>
std::vector<std::vector<T>> DFS_for_cycles(const unidirected_graph<T>& g)
{
	T starting_node = *g.all_nodes().begin();
	std::vector<std::vector<T>> cycles;

	unidirected_graph<T> tree = recursive_dfs(g, starting_node);
	unidirected_graph<T> cotree = g - tree;

	std::vector<T> path;
	std::set<T> visited;

	for (const auto& edge : cotree.all_edges()) {
		visited.clear();

		// Il cammino nell'albero da edge.from() a edge.to(), chiuso dall'arco di
		// coalbero stesso, forma un ciclo
		finding_path(tree, edge.from(), edge.to(), path, visited);

		cycles.push_back(path);
		path.clear();
	}
	return cycles;
}


// Base dei cicli FONDAMENTALI ottenuta dall'albero DFS, restituita nello stesso
// formato di de_pina (un vettore d'incidenza con segno per ogni ciclo, indicizzato
// per numero d'arco), così da poterci costruire la matrice B e confrontarla.
// Riusa DFS_for_cycles per ottenere i cammini e li converte: ogni coppia di nodi
// consecutivi è un arco del cammino, l'arco di chiusura (di coalbero) è (front, back).
// A differenza di De Pina questi cicli non sono minimi.
template<typename T>
std::vector<std::vector<int>> cicli_fondamentali_dfs(const unidirected_graph<T>& g)
{
	size_t m = g.all_edges().size();
	std::map<unidirected_edge<T>, int> indici = g.edge_index_map();

	std::vector<std::vector<T>> cammini = DFS_for_cycles(g);

	std::vector<std::vector<int>> cicli;

	for (const auto& path : cammini) {
		std::vector<int> C(m, 0);

		// Archi del cammino nell'albero: per ogni coppia consecutiva di nodi
		// segno +1 se percorsa in verso concorde all'arco (min->max), -1 altrimenti
		for (size_t t = 0; t + 1 < path.size(); t++) {
			T u = path[t];
			T v = path[t + 1];
			int indice = indici.at(unidirected_edge<T>(u, v));
			int segno = (u < v) ? +1 : -1;
			C[indice] += segno;
		}

		// Arco di chiusura (quello di coalbero): dall'ultimo nodo al primo del cammino
		T u = path.back();
		T v = path.front();
		int indice = indici.at(unidirected_edge<T>(u, v));
		int segno = (u < v) ? +1 : -1;
		C[indice] += segno;

		cicli.push_back(C);
	}
	return cicli;
}