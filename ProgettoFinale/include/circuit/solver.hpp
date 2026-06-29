#pragma once
#include "component.hpp"
#include "../graph/graph.hpp"
#include "../numerics/conjugate_gradient.hpp"
#include <Eigen/Dense>
#include <iostream>
#include <vector>

// Il vecchio solve_system faceva quattro cose insieme (costruzione matrici,
// soluzione, stampa di debug, report dei risultati). Qui è spezzato in tre
// funzioni con una responsabilità ciascuna, componibili dal main.

// Numero di condizionamento di una matrice rettangolare a rango pieno:
// rapporto tra il valore singolare massimo e quello minimo (via SVD).
// Più è piccolo, meglio è condizionata. Serve a confrontare la B prodotta da
// basi di cicli diverse (De Pina vs DFS).
double condizionamento(const Eigen::MatrixXd& M)
{
	Eigen::JacobiSVD<Eigen::MatrixXd> svd(M);
	Eigen::VectorXd valori_singolari = svd.singularValues();
	// JacobiSVD restituisce i valori singolari in ordine decrescente
	double massimo = valori_singolari(0);
	double minimo = valori_singolari(valori_singolari.size() - 1);
	return massimo / minimo;
}

// Sistema alle maglie: diag(r) è la diagonale della matrice delle resistenze,
// B la matrice di incidenza maglie-resistori, b il vettore dei generatori.
struct mesh_system
{
	Eigen::VectorXd r_diag;  // in quanto R è diag non ha senso memorizzarla come matrice

	Eigen::MatrixXd B;
	Eigen::VectorXd b;
};

mesh_system build_system(const std::vector<component>& lista_componenti,
								int nr,
								const std::vector<std::vector<int>>& maglie,
								const unidirected_graph<int>& g)
{
	mesh_system sys;
	sys.r_diag = Eigen::VectorXd::Zero(nr);
	sys.B = Eigen::MatrixXd::Zero(nr, maglie.size());
	sys.b = Eigen::VectorXd::Zero(maglie.size());

	auto indici = g.edge_index_map();

	for (const auto& c : lista_componenti)
	{
		// L'indice dell'arco del componente viene calcolato UNA volta per componente
		const int idx_arco = indici.at(unidirected_edge<int>(c.nodo1, c.nodo2));

		if (c.tipo == 'R')
		{
			// -1 perché i resistori sono numerati da 1 ma le matrici da 0
			sys.r_diag(c.id - 1) = c.valore;

			for (size_t k = 0; k < maglie.size(); k++)
			{
				sys.B(c.id - 1, k) = maglie[k][idx_arco];
			}
		}
		else if (c.tipo == 'V')
		{
			for (size_t k = 0; k < maglie.size(); k++)
			{
				// Contributo del generatore alle maglie che lo contengono,
				// con il segno dato dal verso di percorrenza
				if (maglie[k][idx_arco] != 0)
				{
					sys.b(k) += c.valore * maglie[k][idx_arco];
				}
			}
		}
	}
	return sys;
}

// Risolve B^T diag(r) B I = b con il gradiente coniugato e restituisce
// le correnti di maglia I e il numero di iterazioni
std::pair<Eigen::VectorXd, unsigned int> solve_mesh_currents(
	const mesh_system& sys, const double tol = 1.0e-7)
{
	// asDiagonal() dice a Eigen di trattare r_diag come matrice diagonale:
	// il prodotto costa O(nr * maglie) invece di O(nr^2 * maglie) del prodotto denso
	Eigen::MatrixXd A = sys.B.transpose() * sys.r_diag.asDiagonal() * sys.B;

	Eigen::VectorXd x0 = Eigen::VectorXd::Zero(sys.b.size());

	return gradiente_coniugato(A, sys.b, x0, tol);
}

// Corrente (con segno) su ogni resistore: id resistore -> corrente di ramo.
// Estratto in una funzione separata così la logica è testabile senza passare
// dalla stampa su stdout.
std::map<int, double> compute_branch_currents(
	const std::vector<component>& lista_componenti,
	const std::vector<std::vector<int>>& maglie,
	const unidirected_graph<int>& g,
	const Eigen::VectorXd& I)
{
	auto indici = g.edge_index_map();
	std::map<int, double> correnti;

	for (const auto& c : lista_componenti)
	{
		if (c.tipo != 'R') continue;

		const int idx_arco = indici.at(unidirected_edge<int>(c.nodo1, c.nodo2));

		// La corrente nel ramo è la somma (con segno) delle correnti
		// delle maglie che attraversano il ramo
		double corrente = 0.0;
		for (size_t k = 0; k < maglie.size(); k++)
		{
			corrente += maglie[k][idx_arco] * I[k];
		}
		correnti[c.id] = corrente;
	}
	return correnti;
}

// Calcola e stampa tensione e corrente su ogni resistore
void report_results(const std::vector<component>& lista_componenti,
						   const std::vector<std::vector<int>>& maglie,
						   const unidirected_graph<int>& g,
						   const Eigen::VectorXd& I)
{
	auto correnti = compute_branch_currents(lista_componenti, maglie, g, I);
	for (const auto& c : lista_componenti)
	{
		if (c.tipo != 'R') continue;
		double corrente = correnti.at(c.id);
		std::cout << "R" << c.id << " = " << c.valore * corrente
				  << " V, I = " << corrente << " A." << std::endl;
	}
}
