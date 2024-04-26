#ifndef GRILA_H
#define GRILA_H
#include "Celula.h"
#include <vector>
#include "Nivel.h"

using namespace std;

class Grila {

private:
	int nrLinii;
	int nrColoane;
	int numar_mine;
	vector<vector<Celula>> matrice;

	//metode private
	void plaseaza_mine();

public:
	bool deschide_celula(int x, int y);

	void marcheaza_celula(int x, int y);

	Grila& initializare(Nivel _nivel);

	void deschide_celulele_vecine(int x, int y);

	friend void afiseaza_grila(const Grila& grila);
};

#endif
