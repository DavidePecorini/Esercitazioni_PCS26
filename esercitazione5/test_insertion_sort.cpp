#include<vector> 
#include<iostream>
#include "randfiller.h"
#include "ordinamento.hpp"

int main() 
{
	randfiller rf;
	
	//Creo un vettore contenente le dimensioni di 100 vettori
	std::vector<int> dims(100);
	rf.fill(dims, 0, 5000);
	for (int i = 0; i < 100; i++) 
	{	
		// Inizializzo il vettore casuale
		std::vector<double> v;
		v.resize(dims[i]); 
		rf.fill(v, -10000.0, 10000.0);
		// print_vector(v);
		
		insertion_sort(v);
		bool ordinato = is_sorted(v);
		if (!ordinato)
		{
			std::cout << "ERRORE!";
			return EXIT_FAILURE;
		}
	}
	
	std::vector<std::string> vs = {
		"Federer", "Nadal", "Djokovic", "Nalbandian", "Borg",
		"McEnroe", "Alcaraz", "Sinner", "Becker", "Sampras" 
		};
	insertion_sort(vs);
	bool ordinato_s = is_sorted(vs);
	if (!ordinato_s) 
	{
		std::cout << "ERRORE!";
		return EXIT_FAILURE;
	}
	// print_vector(vs);

	std::cout << "Ordinamento riuscito!";
	return EXIT_SUCCESS;
}
