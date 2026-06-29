#pragma once
#include <iostream>
#include <string>
#include <cmath>

// Strumenti di test minimali, scritti con sole funzioni (niente macro).
// Ogni suite di test e' un eseguibile separato, quindi questo header viene
// incluso in un'unica unita' di traduzione: le variabili e le funzioni qui
// definite non hanno bisogno di "inline", esattamente come gli altri header
// del progetto.

int test_passati = 0;
int test_falliti = 0;

// Stampa il titolo di un gruppo di controlli
void sezione(const std::string& nome)
{
	std::cout << "\n[ " << nome << " ]\n";
}

// Controllo di base: se la condizione e' vera il test passa, altrimenti
// viene stampata la descrizione del controllo fallito
void verifica(bool condizione, const std::string& descrizione)
{
	if (condizione)
	{
		test_passati++;
	}
	else
	{
		test_falliti++;
		std::cout << "  FALLITO: " << descrizione << "\n";
	}
}

// Controllo tra due numeri reali a meno di una tolleranza
// (i risultati numerici non sono mai esatti)
void verifica_vicino(double valore, double atteso, double tolleranza, const std::string& descrizione)
{
	double differenza = valore - atteso;
	if (differenza < 0)
	{
		differenza = -differenza;
	}
	if (differenza <= tolleranza)
	{
		test_passati++;
	}
	else
	{
		test_falliti++;
		std::cout << "  FALLITO: " << descrizione
				  << " [ottenuto " << valore << ", atteso " << atteso << "]\n";
	}
}

// Stampa il riepilogo finale e restituisce il codice di uscita
// (0 se tutti i test passano, 1 se almeno uno fallisce)
int riepilogo()
{
	std::cout << "\n----------------------------------------\n";
	std::cout << "Test passati: " << test_passati
			  << "   falliti: " << test_falliti << "\n";
	std::cout << "----------------------------------------\n";
	if (test_falliti == 0)
	{
		return 0;
	}
	return 1;
}
