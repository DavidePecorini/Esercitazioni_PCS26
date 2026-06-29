#pragma once

// Struttura per le caratteristiche di un componente circuitale.
// La scelta di struct sopra class è dovuta alla possibilità di accedere
// facilmente ai valori di ogni caratteristica.
struct component
{
	char tipo = ' ';   // 'R' resistore, 'V' generatore di tensione
	int id = 0;
	double valore = 0.0;
	int nodo1 = -1;
	int nodo2 = -1;
};
