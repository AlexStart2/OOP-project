#include "Celula.h"

using namespace std;

void Celula::setNrVecini(int nr) {
	if (nr >= 0 && nr <= 8) {
		nrVecini = nr;
	}
	else {
		throw "Numarul de vecini trebuie sa fie intre 0 si 8";
	}
}