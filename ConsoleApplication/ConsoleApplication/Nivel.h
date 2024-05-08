#ifndef NIVEL_H
#define NIVEL_H

struct Nivel {
	char nume[20];
	int nrLinii;
	int nrColoane;
	int nrMine;
};

const Nivel Incepator9{ "Usor 9x9", 9, 9, 10};
const Nivel Incepator16{ "Usor 16x16", 16, 16, 40 };
const Nivel Incepator30{ "Usor 16x30", 16, 30, 75 };
const Nivel Mediu9{ "Mediu 9x9", 9, 9, 20 };
const Nivel Mediu16{ "Mediu 16x16", 16, 16, 68 };
const Nivel Mediu30{ "Mediu 16x30", 16, 30, 120 };
const Nivel Avansat9{ "Avansat 9x9", 9, 9, 31 };
const Nivel Avansat16{ "Avansat 16x16", 16, 16, 96 };
const Nivel Avansat30{ "Avansat 16x30", 16, 30, 167 };


#endif
