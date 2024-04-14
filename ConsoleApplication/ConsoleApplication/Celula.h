#ifndef CELULA_H
#define CELULA_H

#include "Tip.h"
#include "Stare.h"

class Celula {

private:
	Tip tip;
	Stare stare;

public:
	bool deschide(int x, int y);

	void marcheaza(int x, int y);
};

#endif
