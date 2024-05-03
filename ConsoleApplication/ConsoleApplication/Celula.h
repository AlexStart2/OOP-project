#ifndef CELULA_H
#define CELULA_H

#include "Tip.h"
#include "Stare.h"

const bool ConsoleApplication = true;

class Celula {

private:
	Tip tip = Normala;
	Stare stare = Inchisa;
	int nrVecini = 0;

public:

	Tip getTip()const {
		return tip;
	}

	void setTip(Tip s) {
		tip = s;
	}

	Stare getStare() const {
		return stare;
	}

	void setStare(Stare s) {
		stare = s;
	}

	int getNrVecini() const {
		return nrVecini;
	}

	void setNrVecini(int nr);
};

#endif
