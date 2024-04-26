#ifndef JOC_H
#define JOC_H

#include "Grila.h"
#include "Nivel.h"

class Joc {

private:
	const float NIVEL_MAX = 34.79f; // 34.79% din celule sunt mine
	const int NR_MINIM_LINII = 9;
	const int NR_MINIM_COLOANE = 9;
	const int NR_MINIM_MINE = 10;
	const int NR_MAXIM_LINII = 200;
	const int NR_MAXIM_COLOANE = 200;

	Nivel nivel;
	int scor;
	Grila grila;

public:

	Joc();

	Grila& incepe_joc(Nivel nivel = Mediu16);

	void termina_joc();

	bool actiune_joc(int x, int y, bool deschideORmarcheaza);

	bool nivelValid(const int x, const int y, const int nrMine) const;

	Grila& getGrila() {
		return grila;
	}

	bool verificaJocCastigat();
};


#endif
