#ifndef GRILA_H
#define GRILA_H
#include "Celula.h"
#include <vector>
#include "Nivel.h"
#include <SFML/Graphics.hpp>

using namespace std;

class Grila {

private:
	int nrLinii;
	int nrColoane;
	int numar_mine;
	vector<vector<Celula>> matrice;
	int nrMineMarcate = 0;

	//metode private
	void plaseaza_mine();

public:
	bool deschide_celula(int x, int y);

	void marcheaza_celula(int x, int y);

	Grila& initializare(Nivel _nivel);

	void deschide_celulele_vecine(int x, int y);

	int getNrLinii() const {
		return nrLinii;
	}

	int getNrColoane() const {
		return nrColoane;
	}

	int getNumarMine() const {
		return numar_mine;
	}

	int getNrMineMarcate() const {
		return nrMineMarcate;
	}

	Celula getCell(const int i, const int j);

	int getNrMineMarcateGresit() const;

	bool coordonateValide(int y,int x) const;

	friend class Joc;

	friend void afiseaza_grila(const Grila& grila);
	friend void startGame(sf::RenderWindow& window, Grila& grila);
};

#endif
