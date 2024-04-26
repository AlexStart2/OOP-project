#include "Joc.h"
#include "Nivel.h"
#include <iostream>


Joc::Joc() {
	nivel = Mediu16;
	scor = 0;
}

Grila& Joc::incepe_joc(Nivel nivel) {
	scor = 0;
	this->nivel = nivel;
	grila = grila.initializare(nivel);
	return grila;
}

void Joc::termina_joc() {
	// TODO - implement Joc::termina_joc
	throw "Not yet implemented";
}



bool Joc::actiune_joc(int x, int y, bool deschideORmarcheaza) {
	x--;
	y--;
	if (deschideORmarcheaza) {
		return grila.deschide_celula(x, y);
	}
	else {
		grila.marcheaza_celula(x, y);
	}
	return true;	// continue jocul
}

bool Joc::nivelValid(const int x, const int y, const int nrMine) const {
	if (x < NR_MINIM_LINII || y < NR_MINIM_COLOANE || nrMine < NR_MINIM_MINE) {
		return false;
	}
	if (x > NR_MAXIM_LINII || y > NR_MAXIM_COLOANE) {
		return false;
	}
	if (nrMine > (x * y * NIVEL_MAX)) {
		return false;
	}
	return true;
}

bool Joc::verificaJocCastigat() {
	// TODO - implement Joc::verificaJocCastigat
	throw exception("Not yet implemented");
}