#include "unidirected_graph.hpp"
#include <iostream> 

// Nell'intero test le chiamate al metodo add_edge non sono fatte con la sintassi classica
// add_edge(unidirected_edge<int>(1,2) ma con add_edge({1,2}), tuttavia il risultato è il medesimo
// in quanto nel secondo caso avviene una costruzione implicita: il compilatore sa che add_edge
// si aspetta un unidirected_edge e quindi chiama coerentemente il costruttore

bool test_costruttore_default()
{
	unidirected_graph<int> grafo;
	bool ok = (grafo.all_nodes().empty()) && grafo.all_edges().empty();
	std::cout << "test_costruttore_default: "; 
	if (ok) {std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

bool test_costruttore()
{
	unidirected_graph<int> grafo;
	
	unidirected_graph<int> copia(grafo);
	grafo.add_edge({1,3});
	grafo.add_edge({1,2});
	
	copia.add_edge({2,4});
	bool ok = (copia.all_nodes() != grafo.all_nodes()) && (grafo.all_edges().size() == 2)
			&& (copia.all_edges().size() == 1);
	std::cout << "test_costruttore: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

bool test_add_edge()
{
	unidirected_graph<int> grafo;
	grafo.add_edge({1,2});
	grafo.add_edge({1,2});
	grafo.add_edge({2,1});
	
	bool ok = grafo.all_nodes().contains(1) && grafo.all_nodes().contains(2)
			&& grafo.all_edges().contains({1,2}) && (grafo.all_edges().size() == 1);
		std::cout << "test_add_edge: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

bool test_neighbours() 
{
	unidirected_graph<int> grafo;
	grafo.add_edge({1,2});
	grafo.add_edge({1,3});
	grafo.add_edge({2,3});
	
	const std::set<int> vicini = grafo.neighbours(1);
	
	bool ok = vicini.contains(2) && vicini.contains(3) && (vicini.size() == 2);
	std::cout << "test_neighbours: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

bool test_edge_number_e_edge_at() 
{
	unidirected_graph<int> grafo;
	grafo.add_edge({3,4});
	grafo.add_edge({5,6});
	grafo.add_edge({1,2});

	bool ok = (grafo.edge_number({3,4}) == 1) 
			&& (grafo.edge_number({5,6}) == 2) 
			&& (grafo.edge_number({1,2}) == 0)
			&& (grafo.edge_number({25,32}) == -1)
			&& (grafo.edge_at(100) == unidirected_edge<int>())
			&& (grafo.edge_at(1) == unidirected_edge<int>(3,4))
			&& (grafo.edge_at(2) == unidirected_edge<int>(5,6))
			&& (grafo.edge_at(0) == unidirected_edge<int>(1,2));
	std::cout << "test_edge_number_e_edge_at: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

bool test_operator_differenza()
{
	unidirected_graph<int> grafo1;
	grafo1.add_edge({1,2});
	grafo1.add_edge({1,3});
	grafo1.add_edge({2,3});
	
	unidirected_graph<int> grafo2;
	grafo2.add_edge({1,2});
	
	unidirected_graph<int> diff = grafo1 - grafo2;
	
	unidirected_graph<int> vuoto = grafo1 - grafo1;
	unidirected_graph<int> stesso= grafo1 - unidirected_graph<int>();
	
	bool ok = !diff.all_edges().contains({1,2})
			&& diff.all_edges().contains({1,3})
			&& diff.all_edges().contains({2,3})
			&& vuoto.all_edges().empty()
			&& stesso.all_edges() == grafo1.all_edges();
	std::cout << "test_operator_differenza: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

int main() 
{
	bool ok = test_costruttore_default()
			&& test_costruttore()
			&& test_add_edge()
			&& test_neighbours()
			&& test_edge_number_e_edge_at()
			&& test_operator_differenza();
	
	if (ok) {
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}