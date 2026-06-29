#pragma once
#include "edge.hpp"
#include <set>
#include <map>
#include <iostream>

template<typename T>
class unidirected_graph
{
	private:
		std::map<T, std::set<T>> lista_adiacenza; // per neighbours()
		std::set<T> nodi;                         // per all_nodes()
		std::set<unidirected_edge<T>> archi;      // per all_edges(), edge_at(), edge_number()

	public:
		// Costruttore di default e di copia generati
		// dal compilatore: tutti i membri sono container standard che si copiano da soli,
		unidirected_graph() = default;

		const std::set<T>& neighbours(const T& nodo) const
		{
			return lista_adiacenza.at(nodo);
		}

		// add.edge() restituisce bool (true se l'arco è stato davvero inserito),
		// così il chiamante può accorgersi di archi duplicati (es. componenti in parallelo)
		bool add_edge(const unidirected_edge<T>& arco)
		{
			// insert restituisce (iteratore, bool): il bool è true solo se
			// l'elemento non era già presente
			auto controllo = archi.insert(arco);

			if (controllo.second) {
				lista_adiacenza[arco.from()].insert(arco.to());
				lista_adiacenza[arco.to()].insert(arco.from());
				nodi.insert(arco.from());
				nodi.insert(arco.to());
			}
			return controllo.second;
		}

		const std::set<unidirected_edge<T>>& all_edges() const
		{
			return archi;
		}

		const std::set<T>& all_nodes() const
		{
			return nodi;
		}

		// edge_number è O(m) per chiamata perché scorre il set dall'inizio. 
		// Nei cicli annidati (De Pina, solver) veniva chiamata m volte
		// per iterazione -> costo O(m^2) o peggio. Questa funzione costruisce UNA
		// volta sola la mappa arco -> indice in O(m log m); ogni lookup successivo
		// con .at() costa O(log m).
		// Da chiamare quando il grafo è completo (gli indici seguono l'ordinamento
		// del set, quindi un inserimento successivo li invaliderebbe).
		std::map<unidirected_edge<T>, int> edge_index_map() const
		{
			std::map<unidirected_edge<T>, int> indici;
			int i = 0;
			for (const auto& arco : archi) {
				indici[arco] = i;
				i++;
			}
			return indici;
		}

		
		int edge_number(const unidirected_edge<T>& arco) const
		{
			auto indirizzo_arco = archi.find(arco);

			if (indirizzo_arco == archi.end()) {
				std::cerr << "ERRORE: l'arco " << arco << " non è presente nel grafo\n";
				return -1;
			}
			// Scorro il set dall'inizio fino all'arco contando le posizioni
			int indice = 0;
			for (auto it = archi.begin(); it != indirizzo_arco; it++) {
				indice++;
			}
			return indice;
		}

		unidirected_edge<T> edge_at(size_t indice) const
		{
			// Se l'indice è maggiore del numero di archi returno un arco vuoto
			if (indice >= archi.size()) {
				std::cerr << "ERRORE indice " << indice << ": l'indice fornito è maggiore del numero di archi\n";
				return unidirected_edge<T> ();
			}
			// Inizializzo un iteratore sul set
			auto indirizzo_arco = archi.begin();

			// Aggiorno l'iteratore all'elemento successivo per tante volte quante il valore dell'indice
			for (size_t i = 0; i < indice; i++) {
				++indirizzo_arco;
			}
			// Returno ciò a cui punto l'iteratore così aggiornato, ovvero l'arco desiderato
			return *indirizzo_arco;
		}

		// Differenza tra grafi: archi del minuendo non presenti nel sottraendo
		unidirected_graph operator-(const unidirected_graph& other) const
		{
			unidirected_graph ris;

			for (const unidirected_edge<T>& arco : archi)
			{
				if (!other.archi.contains(arco))
				{
					ris.add_edge(arco);
				}
			}
			return ris;
		}
};
