#include "unidirected_graph.hpp"
#include <iostream> 
#include <string>

bool test_costruttore_default()
{
	unidirected_edge<int> arco;
	bool ok = (arco.from() == 0) && (arco.to() == 0);
	std::cout << "test_costruttore_default: "; 
	if (ok) {std::cout << "OK!\n";
	} else {std::cout << "FAIL!\n";}
	return ok;
}

bool test_costruttore()
{
	unidirected_edge<int> arco1(1,2);
	unidirected_edge<int> arco2(4,6);
	unidirected_edge<std::string> arco3("milano","torino");
	
	bool ok = (arco1.from() == 1) && (arco1.to() == 2) 
			&& (arco2.from() == 4) && (arco2.to() == 6)
			&& (arco3.from() == "milano") && (arco3.to() == "torino");
	std::cout << "test_costruttore: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

bool test_operator_minore() 
{
	unidirected_edge<int> arco1(1,2);
	unidirected_edge<int> arco2(4,6);
	unidirected_edge<int> arco3(3,5);
	
	bool ok = (arco1 < arco2) && !(arco2 < arco1)
			&& (arco1 < arco3) && !(arco2 < arco3)
			&& !(arco1 < arco1);
	std::cout << "test_operator_minore: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

bool test_operator_uguale() 
{
	unidirected_edge<int> arco1(1,2);
	unidirected_edge<int> arco2(1,2);
	unidirected_edge<int> arco3(2,1);
	unidirected_edge<int> arco4(3,7);
	
	bool ok = (arco1 == arco2) && (arco2 == arco3) && !(arco1 == arco4);
	std::cout << "test_operator_minore: ";
	if (ok) {
		std::cout << "OK!\n";
	} else {
		std::cout << "FAIL!\n";
	}
	return ok;
}

void test_operator_stampa() 
{
	unidirected_edge<int> arco1(1,2);
	unidirected_edge<std::string> arco2("milano","torino");

	std::cout << "test_operator_stampa - output atteso (1,2): " << arco1 << "\n";
	std::cout << "test_operator_stampa - output atteso (milano,torino): " << arco2 << "\n";
}

int main() 
{
	bool ok = test_costruttore_default()
			&& test_costruttore()
			&& test_operator_minore()
			&& test_operator_uguale();
	test_operator_stampa();
	
	if (ok) {
		return EXIT_SUCCESS;
	}
	return EXIT_FAILURE;
}