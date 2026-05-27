#include <Eigen/Dense>
#include <iostream> 
#include <cmath> 
#include "gradiente_coniugato.hpp" 

double cond(const Eigen::MatrixXd& A)
{
  Eigen::JacobiSVD<Eigen::MatrixXd> svd(A);
  Eigen::VectorXd singularValuesA = svd.singularValues();
  return singularValuesA.maxCoeff() / singularValuesA.minCoeff();
}


int singolo_test(unsigned int n, const double tol, int id_test)
{
	Eigen::MatrixXd B = Eigen::MatrixXd::Random(n,n);
	
	// Scartiamo le matrici con condizionamento elevato 
	// Cond(A) = Cond(B^T B) = cond(B)^2 quindi usiamo 1e5 per avere cond(A) < 1.0e10
	if (cond(B) > 1.0e5) 
	{
		return -1;
	}
	
	Eigen::MatrixXd A = B.transpose() * B;
	Eigen::VectorXd x_ex = Eigen::VectorXd::Ones(n);
	Eigen::VectorXd b = A * x_ex;
	Eigen::VectorXd x_0 = Eigen::VectorXd::Zero(n);
	
	auto [x_sol, it] = gradiente_coniugato(A, b, x_0, tol);
	
	const auto err_rel = (x_ex.norm() == 0) ? (x_sol - x_ex).norm() : 
						 (x_sol - x_ex).norm() / x_ex.norm();
	
	const double soglia_test = 1.0e-10;
	
	if (err_rel < soglia_test) {
		return 1;
	} else {
		std::cerr << "Test #" << id_test <<  "(dim " << n << "x" << n << "): fallito! Err rel: "
				  << err_rel << ", It: " << it << std::endl;
		return 0;
	}
}

int main(int argc, char **argv) 
{

	const double tol = 1.0e-14;
	unsigned int n_max = 202;
	int test_passati = 0;
	int test_skippati = 0;
	unsigned int i = 0;
	
	std::cout << "stress test: matrici da 10x10 a "
			  << n_max << "x" << n_max << ", tol = " << tol << "\n\n";
	
	for (unsigned int n = 10; n <= n_max; n += 4)
	{
		int esito = singolo_test(n, tol, ++i);
		if (esito == 1) {
			test_passati++;
		} else if (esito == -1) {
			test_skippati++;
		} else {
			std::cerr << "Test interrotto! Matrice di dimensione " << n << "x" << n << std::endl;
			return EXIT_FAILURE;
		}
	}
	
	std::cout << test_passati  << "/" << (test_passati + test_skippati) << " passati" << std::endl;
	std::cout << test_skippati << "/" << (test_passati + test_skippati) << " skippati" << std::endl;
	
	return EXIT_SUCCESS;
}
