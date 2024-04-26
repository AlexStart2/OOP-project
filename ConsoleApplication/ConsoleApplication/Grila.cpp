#include "Grila.h"
#include "Nivel.h"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>



bool Grila::deschide_celula(const int x, const int y) {

	if (matrice[x][y].getTip() == Mina) {
		return false; // jocul s-a terminat
	}

	matrice[x][y].setStare(Deschisa);
	deschide_celulele_vecine(x, y);

	return true; // continua jocul

}

void Grila::deschide_celulele_vecine(int x, int y) {
	
	// to check if the code is okay
	while (true) {
		bool deschis = false;
		for (int i = x - 1; i <= x + 1; ++i) {
			for (int j = y - 1; j <= y + 1; ++j) {
				if (i >= 0 && i < nrLinii && j >= 0 && j < nrColoane) {
					if (matrice[i][j].getTip() == Mina) {
						continue;
					}
					if (matrice[i][j].getStare() == Inchisa) {
						matrice[i][j].setStare(Deschisa);
						if (matrice[i][j].getTip() == Normala) {
							deschis = true;
						}
					}
				}
			}
		}
		if (!deschis) {
			break;
		}
	}
}

void Grila::marcheaza_celula(const int x, const int y) {
	matrice[x][y].setStare(Marcata);
}

Grila& Grila::initializare(Nivel _nivel) {
	numar_mine = _nivel.nrMine;
	nrColoane = _nivel.nrColoane;
	nrLinii = _nivel.nrLinii;

	matrice.resize(nrLinii, vector<Celula>(nrColoane));

	plaseaza_mine();

	return *this;
}

void Grila::plaseaza_mine()
{
	srand(time(NULL));

	// Plasarea aleatorie a minelor
	for (int i = 0; i < numar_mine; ++i) {
		int linie, coloana;

		do {
			linie = rand() % nrLinii;
			coloana = rand() % nrColoane;
		} while (matrice[linie][coloana].getTip() == Mina);

		matrice[linie][coloana].setTip(Mina);
	}

	// Calcularea numarului de mine vecine pentru fiecare celula
	for (int i = 0; i < nrLinii; ++i) {
		for (int j = 0; j < nrColoane; ++j) {
			if (matrice[i][j].getTip() == Mina) {
				continue;
			}

			for (int k = i - 1; k <= i + 1; ++k) {
				for (int l = j - 1; l <= j + 1; ++l) {
					if (k >= 0 && k < nrLinii && l >= 0 && l < nrColoane) {
						if (matrice[k][l].getTip() == Mina) {
							matrice[i][j].setNrVecini(matrice[i][j].getNrVecini() + 1);
						}
					}
				}
			}
		}
	}
}
