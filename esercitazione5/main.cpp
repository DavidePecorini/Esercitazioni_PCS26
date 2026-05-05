#include <iostream>
#include <vector> 
#include "timecounter.h"
#include "randfiller.h"
#include <algorithm>
#include "ordinamento.hpp"
#include <iomanip> 


int main(void) {
	randfiller rf;
	timecounter tc;
	
	const int w = 15;
	int n_vec = 100;
	
	// Creo l'intestazione della mia tabella
	std::cout << "\n" << std::left
			  << std::setw(10)<< "Dim (n)"
			  << std::setw(w) << "Bubble (s)"
			  << std::setw(w) << "Insertion (s)" 
			  << std::setw(w) << "Selection (s)"
			  << std::setw(w) << "Merge (s)" 
			  << std::setw(w) << "Quick (s)"
			  << std::setw(w) << "std::sort (s)"
			  << std::setw(w) << "Hybrid (s)" << "\n";
	
	std::cout << std::string(10 + w*7, '-') << "\n";
	
	// Linee di codice per stampare i dati da inserire su MATLAB
	//std::vector<double> tempi_bubble;
	//std::vector<double> tempi_insertion;
	//std::vector<double> tempi_selection;
	//std::vector<double> tempi_merge;
	//std::vector<double> tempi_quick;
	//std::vector<double> tempi_sort;
	//std::vector<double> tempi_hybrid;
	
	for (int n = 4; n <= 100; n += 2)
	{
		// Creo un vettore di 100 vettori casuali
		std::vector<std::vector<double>> vectors(n_vec, std::vector<double>(n));
		for (int i = 0; i< n_vec; i++) {
			rf.fill(vectors[i], -10000.0, 10000.0);
		}
		
		//Creo delle copie del vettore per eseguire l'ordinamento con i vari algoritmi
		std::vector<std::vector<double>> v1 = vectors; std::vector<std::vector<double>> v2 = vectors; std::vector<std::vector<double>> v3 = vectors;
		std::vector<std::vector<double>> v4 = vectors; std::vector<std::vector<double>> v5 = vectors; std::vector<std::vector<double>> v6 = vectors;
		
		//L'ultima riga di ogni blocco serve solo a creare vettori che una volta printati rendono facile il passaggio dei dati su matlab
		
		//Tempo Bubble Sort
		tc.tic();
		for (int i = 0; i < n_vec; i++) {
			bubble_sort(v1[i]);
		}
		double t_bubble = tc.toc()/n_vec;
		//tempi_bubble.push_back(t_bubble);
		
		//Tempo Insertion Sort
		tc.tic();
		for (int i = 0; i < n_vec; i++) {
			insertion_sort(v2[i]);
		}
		double t_insertion = tc.toc()/n_vec;
		//tempi_insertion.push_back(t_insertion);
		
		//Tempo Selection Sort
		tc.tic();
		for (int i = 0; i < n_vec; i++) {
			selection_sort(v3[i]);
		}
		double t_selection = tc.toc()/n_vec;
		//tempi_selection.push_back(t_selection);
		
		//Tempo Merge Sort
		tc.tic();
		for (int i = 0; i < n_vec; i++) {
			merge_sort(v4[i]);
		}
		double t_merge = tc.toc()/n_vec;
		//tempi_merge.push_back(t_merge);
		
		//Tempo Quick Sort
		tc.tic();
		for (int i = 0; i < n_vec; i++) {
			quick_sort(v5[i]);
		}
		double t_quick = tc.toc()/n_vec;
		//tempi_quick.push_back(t_quick);
		
		//Tempo std::sort()
		tc.tic();
		for (int i = 0; i < n_vec; i++) {
			std::sort(v6[i].begin(), v6[i].end());
		}
		double t_sort = tc.toc()/n_vec;
		//tempi_sort.push_back(t_sort);
		
		//Tempo Hybrid Sort
		tc.tic();
		for (int i = 0; i < n_vec; i++) {
			hybrid_sort(vectors[i]);
		}
		double t_hybrid = tc.toc()/n_vec;
		//tempi_hybrid.push_back(t_hybrid);
		
		//Print tabella tempi
		std::cout << std::left
		  << std::setw(10)<< n
		  << std::scientific << std::setprecision(5)
		  << std::setw(w) << t_bubble
		  << std::setw(w) << t_insertion 
		  << std::setw(w) << t_selection
		  << std::setw(w) << t_merge
		  << std::setw(w) << t_quick
		  << std::setw(w) << t_sort
		  << std::setw(w) << t_hybrid
		  << "\n";
	}
	
	// Linee di codice per stampare i dati da inserire su MATLAB
	//std::cout << "v_bubble = ";
	//print_vector(tempi_bubble);
	//std::cout << "v_insert = ";
	//print_vector(tempi_insertion);
	//std::cout << "v_selection = ";
	//print_vector(tempi_selection);
	//std::cout << "v_merge = ";
	//print_vector(tempi_merge);
	//std::cout << "v_quick = ";
	//print_vector(tempi_quick);
	//std::cout << "v_sort = ";
	//print_vector(tempi_sort);
	//std::cout << "v_hybrid = ";
	//maprint_vector(tempi_hybrid);

	

	return 0;
}