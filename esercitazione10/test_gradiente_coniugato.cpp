#include <eigen3/Eigen/Dense>
#include <iostream> 
#include <cmath> 
#include "gradiente_coniugato.hpp" 

bool singolo_test(unsigned int n, const double tol, int id_test)
{
	Eigen::MatrixXd B = Eigen::MatrixXd::Random(n,n);
	
	// Scartiamo le matrici quasi singolari 
	if (std::abs(B.determinant()) < tol)
	{
		return true;
	}
	
	Eigen::MatrixXd A = B.transpose() * B;
	// Aggiungo una perturbazione sulla diagonale per garantire un buon condizionamento
	A += 1e-3 * Eigen::MatrixXd::Identity(n, n);
	Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);
	Eigen::VectorXd b = A * x_ex;
	Eigen::VectorXd x_0 = Eigen::VectorXd::Zero(n);
	
	auto [x_sol, it] = gradiente_coniugato(A, b, x_0, tol);
	
	const auto err_rel = (x_ex.norm() == 0) ? (x_sol - x_ex).norm() : 
						 (x_sol - x_ex).norm() / x_ex.norm();
	
	const double soglia_test = 1.0e-10;
	
	if (err_rel < soglia_test) {
		return true;
	} else {
		std::cerr << "Test #" << id_test <<  " fallito! Err rel: " << err_rel << ", It: " 
		<< it << std::endl;
		return false;
	}
}

int main(int argc, char **argv) 
{

	const double tol = 1.0e-14;
	const int n_matrici = 100;
	const unsigned int n = 100;
	int test_passati = 0;
	
	std::cout << "stress test: " << n_matrici << " matrici "  
			  << n << "x" << n << ", tol = " << tol << "\n\n";
	
	for (unsigned int i = 0; i < n_matrici; i++) {
		
		if(singolo_test(n, tol, i+1)) {
			test_passati++;
		}
		else {
			std::cerr << "Test interrotto" << std::endl;
			return EXIT_FAILURE;
		}
	}
	
	std::cout << test_passati << "/" << n_matrici << " ok!" << std::endl;
	
	
	return EXIT_SUCCESS;
}
