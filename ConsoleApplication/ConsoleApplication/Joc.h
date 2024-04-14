#ifndef JOC_H
#define JOC_H

#include "Grila.h"
#include "Nivel.h"

class Joc {

private:
	Nivel nivel;
	int scor;
	Grila grila;

public:
	void incepe_joc();

	void termina_joc();

	bool actiune_joc();
};


#endif
