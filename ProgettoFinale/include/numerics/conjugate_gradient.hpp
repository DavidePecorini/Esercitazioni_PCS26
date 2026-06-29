#pragma once
#include <Eigen/Dense>


std::pair<Eigen::VectorXd, unsigned int> gradiente_coniugato(const Eigen::MatrixXd& A, 
															 const Eigen::VectorXd& b, 
															 Eigen::VectorXd& x,
															 const double tol) 
{
	// Calcolo i dati iniziali 
	Eigen::VectorXd res = b - A * x;
	Eigen::VectorXd p = res;
	// Il numero massimo di iterazioni dipende dalla dimensione del sistema
	const unsigned int it_max = 10 * b.size();
	
	// UNICA MODIFICA rispetto alla versione originale: lavoro con la norma QUADRA.
	// res.norm() chiama una radice quadrata ad ogni valutazione; il confronto
	//     res.norm() > tol * res_norm_0
	// equivale (entrambi i membri positivi) a
	//     res.squaredNorm() > tol^2 * res_norm_0^2
	// così evito la radice e calcolo il termine di destra una volta sola.
	double soglia = tol * tol * res.squaredNorm();
	unsigned int it = 0;
	
	
	while (it < it_max && res.squaredNorm() > soglia) 
	{
		// Ottimizzo e calcolo A * p una volta sola invece che due
		Eigen::VectorXd Ap = A * p;
		
		// Utilizzo .dot() per calcolare il prodotto scalare di 
		// p * A * p per non calcolarlo due volte
		double p_scalare_Ap = p.dot(Ap);
		
		// Continuo ad usare il prodotto scalare invece del prodotto tra vettori dello pseudocodice
		// poichè il prodotto tra vettori restituisce una matrice 1x1 che rende necessario usare 
		// .value() per estarre il valore mentre .dot() restituisce già uno scalare
		
		double alpha = p.dot(res) / p_scalare_Ap;
		
		// Aggiorno il valore di x e di res
		x += alpha * p;
		
		// Sostituendo la def di x nella formula del residuo otteniamo una formulazione ottimizzata
		// infatti res = b - A * x = b - A * (x + alpha * p) = r - alpha * Ap
		res -= alpha * Ap;
		
		
		// Calcolo beta e aggiorno la direzione di discesa 
		// Sfrutto l'aver già calcolato A * p e la commutatività del prodotto scalare
		double beta = res.dot(Ap) / p_scalare_Ap;
		p = res - beta * p;
		
		// Aggiorno il contatore
		it++;
	}
	
	return {x,it};
}
