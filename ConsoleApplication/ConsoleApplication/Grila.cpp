#include "Grila.h"
#include "Celula.h"
#include "Nivel.h"
#include <iostream>
#include <vector>
#include <random>
#include <conio.h>

using namespace std;

Celula Grila::getCell(const int i, const int j) {
	if (i < 0 || i >= nrLinii || j < 0 || j >= nrColoane) {
		throw exception("Coordonate invalide!");
	}
	else {
		return matrice[i][j];
	}
}


bool Grila::deschide_celula(const int x, const int y) {

	if (matrice[x][y].getStare() == Marcata) {
		if (ConsoleApplication) {
			cout << "Celula marcata! Nu se poate deschide!" << endl;
			_getch();
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
	else if (matrice[x][y].getStare() == Deschisa) {
		if (ConsoleApplication) {
			cout << "Celula deschisa! Nu se poate marca!" << endl;
			_getch();
		}
		else {
			// TODO - implement Grila::marcheaza_celula
		}
	}
}

Grila& Grila::initializare(Nivel _nivel) {
	numar_mine = _nivel.nrMine;
	nrColoane = _nivel.nrColoane;
	nrLinii = _nivel.nrLinii;
	nrMineMarcate = 0;

	matrice.clear();

	matrice.resize(nrLinii, vector<Celula>(nrColoane));

	plaseaza_mine();

	return *this;
}

void Grila::plaseaza_mine()
{
	srand((unsigned int)time(NULL));

	int dimZonaX = 0;
	int dimZonaY = 0;
	int nrX = 4;
	while (dimZonaX == 0) {
		int x = nrColoane / nrX;
		if (x < 4) {
			nrX--;
			continue;
		}
		dimZonaX = x;
	}

	int nrY = 4;
	while (dimZonaY == 0) {
		int y = nrLinii / nrY;
		if (y < 4) {
			nrY--;
			continue;
		}
		dimZonaY = y;
	}

	int restX = nrColoane % dimZonaX;
	int restY = nrLinii % dimZonaY;
	int addXcurrent = 0;
	int addYcurrent = 0;
	int addXprecedent = 0;
	int addYprecedent = 0;

	if (restX != 0) {
		addXcurrent = 1;
		restX--;
	}
	if (restY != 0) {
		addYcurrent = 1;
		restY--;
	}

	int nrMine = numar_mine;

	while (nrMine > 0) {
		for (int nrXi = 0; nrXi < nrX; ++nrXi) {
			for (int nrYi = 0; nrYi < nrY; ++nrYi) {

				int nrZonaX = nrXi * dimZonaX ;
				int nrZonaY = nrYi * dimZonaY ;

				int limSupX = nrZonaX + dimZonaX - 1 + addXcurrent + addXprecedent;
				int limSupY = nrZonaY + dimZonaY - 1 + addYcurrent + addYprecedent;

				int limInfX = nrZonaX + addXprecedent;
				int limInfY = nrZonaY + addYprecedent;

				int x;
				int y;
				do { // x si y sunt inversate pentru ca x reprezinta linia si y reprezinta coloana
					y = rand() % (limSupX - limInfX + 1) + limInfX;
					x = rand() % (limSupY - limInfY + 1) + limInfY;
				} while (matrice[x][y].getTip() == Mina);
				

				matrice[x][y].setTip(Mina);
				addYprecedent += addYcurrent;

				nrMine--;

				if (nrMine == 0) {
					break;
				}

				
				if (restY != 0) {
					addYcurrent = 1;
					restY--;
				}
				else {
					addYcurrent = 0;
				}
			}
			if (nrMine == 0) {
				break;
			}
			addXprecedent += addXcurrent;
			if (restX != 0) {
				addXcurrent = 1;
				restX--;
			}
			else {
				addXcurrent = 0;
			}
			
			addYprecedent = 0;
			restY = nrLinii % dimZonaY;
			if (restY != 0) {
				addYcurrent = 1;
				restY--;
			}
		}
		addXprecedent = 0;
		restX = nrColoane % dimZonaX;
		if (restX != 0) {
			addXcurrent = 1;
			restX--;
		}
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

int Grila::getNrMineMarcateGresit() const {
	int nrMineGresit = 0;
	for (int i = 0; i < nrLinii; ++i) {
		for (int j = 0; j < nrColoane; ++j) {
			if (matrice[i][j].getStare() == Marcata && matrice[i][j].getTip() == Normala) {
				nrMineGresit++;
			}
		}
	}
	return nrMineGresit;
}
