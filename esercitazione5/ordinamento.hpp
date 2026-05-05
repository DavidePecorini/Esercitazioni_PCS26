#pragma once
#include <vector>
#include <iostream>
#include <string>



template<typename T>

void bubble_sort(std::vector<T>& v)
{
	if (v.size() < 2)
	{
		return;
	}
	for (size_t i = 0; i < (v.size()-1); i++) 
	{
		for (size_t j = (v.size()-1); j > i; j--)
		{
			if  (v[j]<v[j-1]) 
			{
				std::swap(v[j],v[j-1]);
			}
		}
	}
}

template<typename T> 

void insertion_sort(std::vector<T>& v)
{
	if (v.size() < 2)
	{
		return;
	}
	for (size_t j = 1; j < v.size(); j++)
	{
		T key = v[j];
		size_t i = j;
		while ( i > 0 && v[i-1] > key) 
		{
			v[i] = v[i-1];
			i--;
		}
		v[i] = key;
	}
}

template<typename T>

void selection_sort(std::vector<T>& v)
{
	if (v.size() < 2)
	{
		return;
	}
	for (size_t i = 0; i < (v.size()-1); i++)
	{
		size_t min = i;
		for (size_t j = i+1; j < v.size(); j++)
		{
			if (v[j] < v[min]) 
			{
				min = j;
			}
		}
		if (min != i)
		{
			std::swap(v[min], v[i]);
		}
	}
}

template<typename T> 

void a_merge(std::vector<T>& v, size_t p, size_t q, size_t r)
{
	size_t n1 = q-p+1;
	size_t n2 = r-q;
	// l'array non ha dimensione n* + 1 perchè in questo algoritmo la gestione di "Array is finished" 
	// è fatta con una variabile booleana e non con il valore infinito in posizione n* + 1
	std::vector<T> L(n1);
	std::vector<T> R(n2);
	for (size_t i = 0; i < n1; i++) 
	{
		L[i] = v[p+i]; // controlla se è giusto 
	}
	for (size_t j = 0; j < n2; j++)
	{
		R[j] = v[q+j+1];
	}
	size_t i = 0;
	size_t j = 0;
	for (size_t k = p; k <= r;  k++)
	{
		bool L_finito = (i >= n1);
		bool R_finito = (j >= n2);
		if (!L_finito && (R_finito || L[i] <= R[j]))
		{
			v[k] = L[i];
			i++;
		}
		else {
			v[k] = R[j];
			j++;
		}
	}
}

template<typename T> 

void merge_sort_struct(std::vector<T>& v, size_t p, size_t r)
{
	if (p<r)
	{
		size_t q = (p+r)/2; // viene fatta automaticamente la divisione intera
		merge_sort_struct(v, p, q);
		merge_sort_struct(v, q+1, r);
		a_merge(v, p, q, r);
	}
}

template<typename T>

void merge_sort(std::vector<T>& v)
{
	if (v.size() < 2) {return;}
	merge_sort_struct(v, 0, v.size()-1);
}

template<typename T> 

size_t partition(std::vector<T>& v, size_t p, size_t r)
{
	T x = v[r];
	
	//L'implementazione fatta è leggermente diversa da quella che si ricava dallo pseudocodice poichè questa portava alla possibilità di avere i = -1 che essendo un tipo unsigned traduce
	//la lettura di un numero negativo con un numero molto grande
	
	size_t i = p;
	
	for (size_t j = p; j <= r-1; j++)
	{
		if (v[j] <= x)
		{
			std::swap(v[i],v[j]);
			i++;
		}
	}
	std::swap(v[i], v[r]);
	return i;
}

template<typename T> 

void quick_sort_struct(std::vector<T>& v, size_t p, size_t r)
{
	if (p < r)
	{
		size_t q = partition(v, p, r);
		
		if (q > p) {
			quick_sort_struct(v, p, q-1);
		}
		quick_sort_struct(v, q+1, r);
	}
}

template<typename T> 

void quick_sort(std::vector<T>& v)
{
	if (v.size() < 2) {return;}
	quick_sort_struct(v, 0, v.size()-1);
}


template<typename T> 

void hybrid_sort_struct(std::vector<T>& v, size_t p, size_t r)
{
	// Se la dimensione del vettore è minore di 38 esce e va subito all'insertion sort
	if ((r-p) < 38) {
		return;
	}
	// Altrimenti si procede con il QuickSort normale
	if (p < r)
	{
		size_t q = partition(v, p, r);
		
		if (q > p) {
			hybrid_sort_struct(v, p, q-1);
		}
		hybrid_sort_struct(v, q+1, r);
	}
}


template<typename T> 

void hybrid_sort(std::vector<T>& v)
{
	if (v.size() < 2) {return;}
	
	//L'algoritmo hybrid_sort_struct implementa il QuickSort fino a quando la ricorsione non raggiunge vettori di dim < 38 dopodichè si ferma
	hybrid_sort_struct(v, 0, v.size()-1);
	
	insertion_sort(v);
}


template<typename T> 

bool is_sorted(const std::vector<T>& v)
{
	if (v.size() < 2)
	{
		return true;
	}
	for (size_t i = 0; i < (v.size() - 1); i++)
	{
		if (v[i] > v[i+1])
		{
		return false;
		}
	}
	return true;
}

// Print vector è stata modificata in modo da produrre vettori direttamente inseribili su MATLAB

template<typename T>

void print_vector(const std::vector<T>& v)
{
	std::cout << "[";
	for (size_t i = 0; i < v.size(); i++) {
		std::cout << v[i] << " ";
	}
	std::cout << "];\n";
}

