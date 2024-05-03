#include "Grila.h"
#include "Celula.h"
#include "Nivel.h"
#include <iostream>
#include <vector>
#include <random>
#include <ctime>

using namespace std;


bool Grila::deschide_celula(const int x, const int y) {

	if (matrice[x][y].getStare() == Marcata) {
		if (ConsoleApplication) {
			cout << "Celula marcata! Nu se poate deschide!" << endl;
			getchar();
		}
		else {
			// TODO - implement Grila::deschide_celula
		}
			
		return true;
	}

	if (matrice[x][y].getTip() == Mina) {
		return false; // jocul s-a terminat
	}

	matrice[x][y].setStare(Deschisa);
	deschide_celulele_vecine(x, y);

	return true; // continua jocul

}

void Grila::deschide_celulele_vecine(int x, int y) {
	if (matrice[x][y].getNrVecini() != 0) {
		return;
	}

	for (int i = x - 1; i <= x + 1; ++i) {
		for (int j = y - 1; j <= y + 1; ++j) {
			if (i >= 0 && i < nrLinii && j >= 0 && j < nrColoane) {
				if (matrice[i][j].getStare() == Inchisa) {
					matrice[i][j].setStare(Deschisa);
					deschide_celulele_vecine(i, j);
				}
			}
		}
	}
}

void Grila::marcheaza_celula(const int x, const int y) {
	if (matrice[x][y].getStare() == Marcata) {
		matrice[x][y].setStare(Inchisa);
		nrMineMarcate--;
	}
	else if (matrice[x][y].getStare() == Inchisa) {
		matrice[x][y].setStare(Marcata);
		nrMineMarcate++;
	}
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

bool Grila::coordonateValide(int y, int x) const {
	return x > 0 && x <= nrLinii && y > 0 && y <= nrColoane;
}
