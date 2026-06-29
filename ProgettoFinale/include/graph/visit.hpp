#pragma once
#include "graph.hpp"
#include <set>
#include <queue>
#include <limits>
#include <optional>
#include <vector>
#include <algorithm>
#include <utility>

// ----------------------------- DFS ricorsiva -------------------------------

template<typename T>
void recursive_dfs_struct(const unidirected_graph<T>& G,
						  const T& nodo,
						  std::set<T>& nodi_visitati,
						  unidirected_graph<T>& grafo_ris)
{
	nodi_visitati.insert(nodo);

	for (const T& nodo_vicino : G.neighbours(nodo)) {

		if (!nodi_visitati.count(nodo_vicino)) {

			grafo_ris.add_edge(unidirected_edge<T>(nodo, nodo_vicino));

			// Lo stack delle chiamate (LIFO) realizza implicitamente la politica DFS
			recursive_dfs_struct(G, nodo_vicino, nodi_visitati, grafo_ris);
		}
	}
}

template<typename T>
unidirected_graph<T> recursive_dfs(const unidirected_graph<T>& G, const T& nodo_sorgente)
{
	unidirected_graph<T> grafo_ris;
	std::set<T> nodi_visitati;

	if (!G.all_nodes().count(nodo_sorgente)) {
		return grafo_ris;
	}

	recursive_dfs_struct(G, nodo_sorgente, nodi_visitati, grafo_ris);
	return grafo_ris;
}

// ------------------------------- Dijkstra ----------------------------------

// Nel nostro caso l'algoritmo di Djikstra restituisce una coppia di mappe: la prima di forma nodo - cammino minimo per raggiungerlo dal nodo sorgente;
// la seconda restituisce per ogni nodo qual è il nodo immediatamente precedente nel cammino minimo (se ce n'è uno)
template<typename T>
std::pair<std::map<T, double>, std::map<T, std::optional<T>>> djikstra(const unidirected_graph<T>& G, const T& nodo_sorgente) 
{
	// Inizializzo la mia coppia di mappe
	std::map<T, double> distanze;
	std::map<T,std::optional<T>> predecessori;
	
	// Controllo che il nodo sorgente sia parte del grafo, in caso contrario restituisco le mappe vuote
	auto nodi = G.all_nodes();
	if (!nodi.count(nodo_sorgente)) {
		return {distanze, predecessori};
	}
	
	// Setto per ogni nodo il cammino minimo a lunghezza infinito e il predecessore a nullopt
	for (const T& nodo : G.all_nodes()) {
		distanze[nodo] = std::numeric_limits<double>::infinity();
		predecessori[nodo] = std::nullopt; //non metto -1 come nello pseudocodice perchè se il nodo non è di tipo int non funziona
	}
	// Setto ora cammino minimo e predecessore del nodo sorgente come nello pseudocodice
	predecessori[nodo_sorgente] = nodo_sorgente;
	distanze[nodo_sorgente] = 0.0;
	
	// Inizializzo la mia struttura priority queue
	
	// Creo un alias per quello che sarà l'elemento contenuto nella priority queue
	// L'elemento della pq sarà una coppia distanza accumulata nodo
	using Coppia = std::pair<double, T>;
	
	// uso std::greater il modo che il minore va in cima (processo per prima la distanza minima)
	std::priority_queue<Coppia, std::vector<Coppia>, std::greater<Coppia>> pq;
	
	// Inserisco il mio nodo_sorgente a distanza 0 nella pq
	pq.push({0.0, nodo_sorgente});
	
	while (!pq.empty()) {
		// estraggo l'elemento in cima alla pq (quello con distanza corrente minore, ovvero il più vicino al nodo_sorgente) 
		// e lo rimuovo per evitare di processarlo di nuovo
		auto [distanza_corrente, nodo_corrente] = pq.top();
		pq.pop();
		
		// scorro i vicini di nodo_corrente e recupero gli archi che li collegano con relativo peso
		for (const T& nodo_vicino : G.neighbours(nodo_corrente)) {
			// Creo l'arco tra nodo_vicino e nodo_corrente
			unidirected_edge<T> arco_corrente(nodo_corrente, nodo_vicino);
			// Trovo la posizione di arco_corrente nel mio set di archi del grafo con un iteratore
			auto it = G.all_edges().find(arco_corrente);
			// lo dereferenzio e estraggo dall'arco nel set il peso 
			int peso = it->peso();
			
			// Controllo se la distanza memorizzata per arrivare a nodo_vicino è maggiore di quella trovata passando da nodo_corrente
			// se passare da nodo_corrente conviene aggiorno le mappe e passo questa nuova combinazione alla pq
			if (distanze[nodo_vicino] > distanze[nodo_corrente] + peso) {
				distanze[nodo_vicino] = distanze[nodo_corrente] + peso;
				predecessori[nodo_vicino] = nodo_corrente;
				pq.push({distanze[nodo_vicino], nodo_vicino});
			}
		}
	}
	return {distanze, predecessori};
}


// Ricostruzione del cammino minimo sorgente -> destinazione
template <typename T>
std::optional<std::vector<T>> vettore_cammino_minimo(
	const std::map<T, std::optional<T>>& predecessori,
	const std::map<T, double>& distanze,
	const T& sorgente,
	const T& destinazione)
{
	// Destinazione non raggiungibile -> distanza ancora infinita
	if (std::isinf(distanze.at(destinazione))) {
		return std::nullopt;
	}

	std::vector<T> vec_ris;
	T nodo_corrente = destinazione;
	vec_ris.push_back(nodo_corrente);

	while (nodo_corrente != sorgente) {
		nodo_corrente = predecessori.at(nodo_corrente).value();
		vec_ris.push_back(nodo_corrente);
	}
	std::reverse(vec_ris.begin(), vec_ris.end());

	return vec_ris;
}
