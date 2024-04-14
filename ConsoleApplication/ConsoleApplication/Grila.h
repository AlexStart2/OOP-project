#ifndef GRILA_H
#define GRILA_H
#include "Celula.h"
#include <vector>

class Grila {

private:
	int dimensiune;
	int numar_mine;
	std::vector<Celula[]> celule;

public:
	bool deschide_celula(int x, int y);

	void marcheaza_celula(int x, int y);

	Grila initializare(int _nivel);
};

#endif
