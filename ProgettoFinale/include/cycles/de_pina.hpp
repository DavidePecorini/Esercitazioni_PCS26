#pragma once
#include "../graph/visit.hpp"
#include "op_bool.hpp"
#include <algorithm>
#include <vector>
#include <map>

// Introduciamo una funzione per contare il numero di archi presente in un ciclo, ovvero
// tutte le entry diverse da 0. 
int conta_archi(const std::vector<int>& C)
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

template<typename T>
std::vector<int> trova_ciclominimo(const unidirected_graph<T>& g,
								   const std::vector<bool>& S,
								   const std::map<unidirected_edge<T>, int>& indici)
{
	// Creo il grafo sdoppiato

	unidirected_graph<T> G_primo;

	int n = g.all_nodes().size();
	int m = g.all_edges().size();

	for (const auto& arco : g.all_edges())
	{
		T u = arco.from();
		T v = arco.to();

		// Controllo che l'arco sia attivo, valuto il valore di S_i per l'indice dell'arco in g
		if (S[indici.at(arco)])
		{
			// Si noti che nel nostro modello vale u = u+ e v+n+1 = v-
			// La scelta di n+1 al posto di n è dovuta al fatto di dover evitare la comparsa di un nodo 0 
			// quando viene fatta l'operazione %n
			// Notiamo che questa soluzione funziona solo in caso di nodi numerati consecutivamente
			G_primo.add_edge(unidirected_edge<T>(u,v+n+1));
			G_primo.add_edge(unidirected_edge<T>(u+n+1,v));
		}
		else
		{
			G_primo.add_edge(unidirected_edge<T>(u,v));
			G_primo.add_edge(unidirected_edge<T>(u+n+1,v+n+1));
		}
	}
	// Sentinella: C_best parte vuoto e "migliore" è inizializzato a m+1, cioè a
	// più archi di quanti ne possa usare qualunque ciclo reale (al massimo m).
	// NB: questo va di pari passo col fix del conteggio. Con l'inizializzazione
	// originale C_best(m,1) e il conteggio corretto (conta_archi), un ciclo che usa
	// tutti gli m archi (es. il triangolo) pareggerebbe il sentinella e non lo
	// sostituirebbe, restituendo il vettore fittizio (1,1,...,1). m+1 evita il pareggio.
	std::vector<int> C_best;
	int migliore = m + 1;

	for (const auto& nodo : g.all_nodes())
	{
		auto [distanze, predecessori] = djikstra(G_primo, nodo);
		auto vec_opt = vettore_cammino_minimo(predecessori, distanze, nodo, nodo+n+1);

		if (!vec_opt.has_value()) continue;

		std::vector<T> vec_minimo = vec_opt.value();

		std::vector<int> C(m,0);

		for (size_t t = 0; t + 1 < vec_minimo.size(); t++)
		{
			T u = vec_minimo[t] % (n+1);
			T v = vec_minimo[t+1] % (n+1);

			// Nel cercare l'indice anche se u > v la classe trova comunque l'arco in ordine corretto
			size_t indice = indici.at(unidirected_edge<T>(u,v));

			// L'arco è memorizzato: min - max
			// se u < v : verso concorde: +1
			// se u > v : verso discorde: -1

			int segno = (u < v) ? +1 : -1;

			C[indice] += segno;
		}

		int archi_usati = conta_archi(C);
		if (archi_usati < migliore)
		{
			migliore = archi_usati;
			C_best = C;
		}
	}

	// Se nessun ciclo è stato trovato restituisco un vettore nullo (non dovrebbe
	// accadere: la funzione viene chiamata solo quando il coalbero non è vuoto)
	if (C_best.empty())
	{
		C_best.assign(m, 0);
	}
	return C_best;
}



template<typename T>

std::vector<std::vector<int>> de_pina(const unidirected_graph<T>& g){
	
	// Scelgo un nodo
	T starting_node = *g.all_nodes().begin();

	unidirected_graph<T> tree = recursive_dfs(g, starting_node);

	unidirected_graph<T> cotree = g - tree;

	size_t m = g.all_edges().size();  //questo m è il numero di archi del grafo e sarà la dimensione di tutti i vettori booleani che creeremo


	std::map<unidirected_edge<T>, int> indici = g.edge_index_map();

	std::vector<std::vector<bool>> S;  //creo la matrice dove inserirò i vettori Si, S avrà quindi dimensione k X m di elementi booleani
	for(const auto& arco_coalbero : cotree.all_edges()){
		std::vector<bool> si(m,false); //impongo la dimensione e inizializzo gli elementi falsi
		int indice = indici.at(arco_coalbero);
		si[indice] = true;
		S.push_back(si);
	}
	//costruzione dell'algoritmo di De Pina
	std::vector<std::vector<int>> cicli_minimi; //questo vettore di vettori sarà il risultato dell'intera funzione

	size_t k = S.size(); //equivale al numero dei vettori del coalbero, basti vedere da che ciclo for è stato generato
	//seguo l'iter del pdf, implemento lo pseudocodice e poi implementerò la funzione che trova veramente il ciclo minimo

	for(size_t i = 0; i < k; i++)
	{
		std::vector<int> Ci_segno = trova_ciclominimo(g,S[i],indici);


		cicli_minimi.push_back(Ci_segno);

		// Ricostruisco il vettore di booleani da utilizzare per modificare S dal vettore di int
		std::vector<bool> Ci(m, false);
		for (size_t t = 0; t < m; t++) { Ci[t] = (Ci_segno[t] != 0); };

		for(size_t j = i+1; j<k;j++)
		{
			if(prodotto_scalare(Ci,S[j]) == 1)
			{
				S[j] = differenza_booleana(S[j],S[i]);
			}
		}
	}
	return cicli_minimi;
}
