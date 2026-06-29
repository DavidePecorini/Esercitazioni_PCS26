#pragma once
#include "component.hpp"
#include "../graph/graph.hpp"
#include <fstream>
#include <iostream>
#include <vector>
#include <string>
#include <utility>

// Legge il file di input e restituisce la lista dei componenti e il numero di resistori
std::pair<std::vector<component>, int> read_input(int argc, char **argv)
{
	// Inizializziamo la lista di componenti e il numero di resistori
	// necessario per creare la matrice R
	std::vector<component> lista_componenti;
	int n_resistori = 0;

	if (argc < 2) {
		std::cerr << "ERRORE! È necessario inserire il nome del file!" << std::endl;
		return {lista_componenti, n_resistori};
	}

	std::string filename = argv[1];
	std::ifstream ifs(filename);
	if (!ifs.is_open())
	{
		std::cerr << "ERRORE! Impossibile aprire il file: " << filename << std::endl;
		return {lista_componenti, n_resistori};
	}

	char t;
	int n;
	double v;
	int n1;
	int n2;

	while (ifs >> t >> n >> v >> n1 >> n2)
	{
		if (t != 'R' && t != 'V') 
		{
			std::cerr << "ERRORE: Componente non riconosciuta" << std::endl;
			continue;
		}
		
		component c;
		c.tipo = t;
		c.id = n;
		c.valore = v;
		c.nodo1 = n1;
		c.nodo2 = n2;

		if (t == 'R')
		{
			n_resistori++;
		}
		else if (t == 'V' && n1 > n2)
		{
			// Convenzione di orientamento: l'arco viene normalizzato min-max,
			// quindi se il generatore è specificato "al contrario" inverto il segno
			c.valore *= -1;
		}

		lista_componenti.push_back(c);
	}
	return {lista_componenti, n_resistori};
}

// Costruisce il grafo del circuito a partire dalla lista dei componenti.
unidirected_graph<int> graph_builder(const std::vector<component>& lista_componenti)
{
	unidirected_graph<int> grafo_circuito;
	for (const auto& c : lista_componenti)
	{
		unidirected_edge<int> arco(c.nodo1, c.nodo2, c.valore);

		// gli archi sono identificati dalla sola coppia di nodi, quindi
		// due componenti in parallelo tra gli stessi nodi collasserebbero
		// silenziosamente in un unico arco producendo risultati errati.
		// Ora il caso viene almeno segnalato all'utente.
		if (!grafo_circuito.add_edge(arco))
		{
			std::cerr << "ATTENZIONE: componente " << c.tipo << c.id
					  << " ignorato: esiste già un componente tra i nodi "
					  << c.nodo1 << " e " << c.nodo2
					  << " (componenti in parallelo non supportati)" << std::endl;
		}
	}
	return grafo_circuito;
}
