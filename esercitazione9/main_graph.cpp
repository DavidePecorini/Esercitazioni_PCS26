#include <iostream> 
#include "unidirected_graph.hpp"
#include "graph_visit.hpp" 
#include "LifoFifo.hpp" 
#include <string>

int main()
{
	lifo<int> l;
	fifo<int> f;
	unidirected_graph<int> g;
	g.add_edge({1,2,6});
	g.add_edge({1,3,7});
	g.add_edge({1,4,9});
	g.add_edge({1,6,12});
	g.add_edge({2,4,2});
	g.add_edge({2,5,22});
	g.add_edge({2,7,15});
	g.add_edge({3,6,8});
	g.add_edge({4,6,2});
	g.add_edge({4,7,16});
	g.add_edge({5,7,4});
	g.add_edge({6,7,6});
	g.add_edge({6,8,7});
	g.add_edge({7,9,1});
	g.add_edge({8,9,2});
	
	g.print_graph("grafo_iniziale");
	
	int nodo_sorgente = 1;
	
	unidirected_graph<int> albero_visita_stack = graph_visit(g, nodo_sorgente, l);
	unidirected_graph<int> albero_visita_queue = graph_visit(g, nodo_sorgente, f);
	unidirected_graph<int> recursive_tree = recursive_dfs(g, nodo_sorgente);
	auto [distanze, predecessori] = djikstra(g, nodo_sorgente);
	unidirected_graph<int> albero_djikstra = albero_predecessori(predecessori, nodo_sorgente);
	
	albero_visita_stack.print_graph("albero_visita_stack");
	albero_visita_queue.print_graph("albero_visita_queue");
	recursive_tree.print_graph("albero_visita_recursive");
	albero_djikstra.print_graph("albero_visita_djikstra");
		
	return 0;
}
