#ifndef NIVEL_H
#define NIVEL_H

struct Nivel {
	int nrLinii;
	int nrColoane;
	int nrMine;
};

const Nivel Incepator9{ 9, 9, 10 };
const Nivel Incepator16{ 16, 16, 40 };
const Nivel Incepator30{ 16, 30, 75 };
const Nivel Mediu9{ 9, 9, 20 };
const Nivel Mediu16{ 16, 16, 68 };
const Nivel Mediu30{ 16, 30, 120 };
const Nivel Avansat9{ 9, 9, 31 };
const Nivel Avansat16{ 16, 16, 96 };
const Nivel Avansat30{ 16, 30, 167 };


#endif
