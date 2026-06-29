#pragma once
#include <iostream>

// Arco non orientato con attributi tipo e peso.
// Confronto e uguaglianza usano SOLO la coppia di nodi: due componenti in
// parallelo tra gli stessi nodi collassano nello stesso arco (segnalato in graph_builder)
template<typename T>
class unidirected_edge
{
	private:
		T nodo1;
		T nodo2;
		double _peso;

	public:
		
		unidirected_edge()
			: nodo1(T{}), nodo2(T{}), _peso(1.0)
		{}

		// Peso di default 1.0
		unidirected_edge(const T& n1, const T& n2, double p = 1.0)
			: _peso(p)
		{
			// Normalizzo l'ordine dei nodi: l'arco è non orientato
			if (n1 < n2) {
				nodo1 = n1;
				nodo2 = n2;
			} else {
				nodo1 = n2;
				nodo2 = n1;
			}
		}

		T from() const {return nodo1;}
		T to() const {return nodo2;}
		double peso() const {return _peso;}

		// Ordinamento lessicografico sulla coppia (nodo1, nodo2)
		bool operator<(const unidirected_edge<T>& other) const
		{
			if (nodo1 != other.nodo1)
			{
				return nodo1 < other.nodo1;
			}
			return nodo2 < other.nodo2;
		}

		bool operator==(const unidirected_edge<T>& other) const
		{
			return nodo1 == other.nodo1 && nodo2 == other.nodo2;
		}
};

// Operatore di stream: stampa (a,b) : peso = p, tipo = t
template<typename T>
std::ostream& operator<<(std::ostream& os, const unidirected_edge<T>& arco)
{
	os << "(" << arco.from() << "," << arco.to() << ") : peso = "
	   << arco.peso() << std::endl;
	return os;
}
