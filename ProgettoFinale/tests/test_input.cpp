#include "test_framework.hpp"
#include "../include/circuit/input.hpp"
#include <fstream>
#include <cstdio>
#include <vector>

// Scrive un file temporaneo con il contenuto dato e lo legge con read_input,
// simulando gli argomenti da riga di comando (argc, argv)
std::pair<std::vector<component>, int> leggi_da_stringa(const std::string& contenuto)
{
	const char* percorso = "/tmp/_test_circuit_input.txt";
	std::ofstream ofs(percorso);
	ofs << contenuto;
	ofs.close();

	char arg0[] = "prog";
	char arg1[] = "/tmp/_test_circuit_input.txt";
	char* argv[] = {arg0, arg1};
	return read_input(2, argv);
}

int main()
{
	sezione("read_input");

	// Parsing base: 1 generatore + 2 resistori
	{
		auto risultato = leggi_da_stringa("V 1 10 1 2\nR 1 5 2 3\nR 2 5 3 1\n");
		std::vector<component> comp = risultato.first;
		int nr = risultato.second;
		verifica(comp.size() == 3, "tre componenti letti");
		verifica(nr == 2, "solo i resistori vengono contati");
	}

	// Convenzione di segno: un generatore con nodo1 > nodo2 ha il valore negato
	// (l'arco viene normalizzato min-max, va compensato l'orientamento)
	{
		auto risultato = leggi_da_stringa("V 1 10 2 1\n");
		std::vector<component> comp = risultato.first;
		verifica(comp.size() == 1, "un componente letto");
		verifica_vicino(comp.front().valore, -10.0, 1e-12,
						"generatore con nodi invertiti: valore negato");
	}
	// Stesso generatore con nodi in ordine: valore invariato
	{
		auto risultato = leggi_da_stringa("V 1 10 1 2\n");
		std::vector<component> comp = risultato.first;
		verifica_vicino(comp.front().valore, 10.0, 1e-12,
						"generatore con nodi in ordine: valore invariato");
	}

	// File inesistente -> lista vuota, nessun crash
	{
		char arg0[] = "prog";
		char arg1[] = "/tmp/_non_esiste_12345.txt";
		char* argv[] = {arg0, arg1};
		auto risultato = read_input(2, argv);
		verifica(risultato.first.empty(), "file inesistente: lista vuota");
		verifica(risultato.second == 0, "file inesistente: zero resistori");
	}

	// Nessun argomento (argc < 2) -> lista vuota
	{
		char arg0[] = "prog";
		char* argv[] = {arg0};
		auto risultato = read_input(1, argv);
		verifica(risultato.first.empty(), "nessun argomento: lista vuota");
	}

	sezione("graph_builder");

	// Il grafo ha tanti archi quante coppie di nodi distinte
	{
		auto risultato = leggi_da_stringa("V 1 10 1 2\nR 1 5 2 3\nR 2 5 3 1\n");
		unidirected_graph<int> g = graph_builder(risultato.first);
		verifica(g.all_edges().size() == 3, "tre archi nel grafo");
		verifica(g.all_nodes().size() == 3, "tre nodi nel grafo");
	}

	// Due componenti tra gli stessi nodi -> un solo arco (i paralleli collassano).
	// graph_builder stampa un avviso su stderr; qui controllo solo la deduplica.
	{
		auto risultato = leggi_da_stringa("R 1 5 1 2\nR 2 8 2 1\n");
		unidirected_graph<int> g = graph_builder(risultato.first);
		verifica(g.all_edges().size() == 1, "componenti in parallelo: un solo arco");
	}

	std::remove("/tmp/_test_circuit_input.txt");

	return riepilogo();
}
