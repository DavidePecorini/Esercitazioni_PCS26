#pragma once
#include <vector>
#include <iostream>

// Operazioni su vettori binari usate dall'algoritmo di De Pina.

// Prodotto scalare: parità del numero di posizioni in cui entrambi valgono 1.
int prodotto_scalare(const std::vector<bool>& vec1, const std::vector<bool>& vec2)
{
	// Se l'input non è valido segnalo l'errore e restituisco un 
	// valore sentinella (-1, impossibile per una parità)
	if (vec1.size() != vec2.size()) {
		std::cerr << "ERRORE: i vettori devono avere la stessa dimensione\n";
		return -1;
	}
	bool parita = false;
	for (size_t t = 0; t < vec1.size(); t++) {
		parita = parita != (vec1[t] && vec2[t]); // XOR logico
	}
	return parita ? 1 : 0;
}

// Differenza booleana: in somma e differenza coincidono ed equivalgono allo XOR
// componente per componente (1 se le componenti differiscono).
std::vector<bool> differenza_booleana(const std::vector<bool>& vec1,
									 const std::vector<bool>& vec2)
{
	//  Se l'input non è valido segnalo l'errore e vettore vuoto come sentinella 
	if (vec1.size() != vec2.size()) {
		std::cerr << "ERRORE: i vettori devono avere la stessa dimensione\n";
		return std::vector<bool>();
	}
	std::vector<bool> risultato(vec1.size());
	for (size_t t = 0; t < vec1.size(); t++) {
		risultato[t] = (vec1[t] != vec2[t]); // XOR
	}
	return risultato;
}
