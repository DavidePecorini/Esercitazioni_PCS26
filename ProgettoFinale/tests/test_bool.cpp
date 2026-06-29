#include "test_framework.hpp"
#include "../include/cycles/op_bool.hpp"

// Conta quante posizioni di un vettore di bool valgono true
int conta_veri(const std::vector<bool>& v)
{
	int n = 0;
	for (size_t t = 0; t < v.size(); t++)
	{
		if (v[t])
		{
			n++;
		}
	}
	return n;
}

int main()
{
	sezione("operazioni bool");

	// prodotto_scalare = parita' del numero di posizioni con entrambi 1.
	// Una sola coincidenza -> dispari -> 1
	verifica(prodotto_scalare({1, 0, 1}, {1, 0, 0}) == 1,
			 "prodotto_scalare con una coincidenza deve dare 1");
	// Due coincidenze -> pari -> 0
	verifica(prodotto_scalare({1, 1, 1}, {1, 1, 0}) == 0,
			 "prodotto_scalare con due coincidenze deve dare 0");
	// Nessuna coincidenza (ortogonali) -> 0
	verifica(prodotto_scalare({1, 0, 0}, {0, 1, 1}) == 0,
			 "prodotto_scalare di vettori ortogonali deve dare 0");
	// Vettore nullo -> 0
	verifica(prodotto_scalare({0, 0, 0}, {1, 1, 1}) == 0,
			 "prodotto_scalare con vettore nullo deve dare 0");
	// Tre coincidenze -> dispari -> 1
	verifica(prodotto_scalare({1, 1, 1}, {1, 1, 1}) == 1,
			 "prodotto_scalare con tre coincidenze deve dare 1");

	// differenza_booleana = XOR componente per componente
	std::vector<bool> diff = differenza_booleana({1, 1, 0, 0}, {1, 0, 1, 0});
	std::vector<bool> atteso = {0, 1, 1, 0};
	verifica(diff == atteso, "differenza_booleana deve fare lo XOR");

	// In GF(2) un vettore meno se stesso da' il vettore nullo
	std::vector<bool> v = {1, 0, 1, 1};
	std::vector<bool> zero = differenza_booleana(v, v);
	verifica(conta_veri(zero) == 0, "x - x deve dare il vettore nullo");

	// Dimensioni diverse -> errore su cerr e valore sentinella.
	// prodotto_scalare restituisce -1 (impossibile per una parità)
	verifica(prodotto_scalare({1, 0}, {1, 0, 1}) == -1,
			 "prodotto_scalare con dimensioni diverse restituisce -1");

	// differenza_booleana restituisce un vettore vuoto
	std::vector<bool> diff_errore = differenza_booleana({1, 0}, {1, 0, 1});
	verifica(diff_errore.empty(),
			 "differenza_booleana con dimensioni diverse restituisce un vettore vuoto");

	return riepilogo();
}
