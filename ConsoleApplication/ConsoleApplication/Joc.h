#ifndef JOC_H
#define JOC_H

#include "Grila.h"
#include "Nivel.h"
#include <string>
#include <chrono>

class Joc {

private:
	

	static const char DELIMITER = ' ';

	Nivel nivel;
	float scor;
	Grila grila;
	chrono::time_point<std::chrono::steady_clock> start;
	float timp;
public:
	static const char FISIER[20];
	static const char FISIER_SCOR[20];
	static const char FISIER_CONFIG[20];
	const float NIVEL_MAX = 34.79f; // 34.79% din celule sunt mine
	const int NR_MINIM_LINII = 9;
	const int NR_MINIM_COLOANE = 9;
	const int NR_MINIM_MINE = 10;
	const int NR_MAXIM_LINII = 100;
	const int NR_MAXIM_COLOANE = 100;

	Joc();

	Grila& incepe_joc(Nivel nivel = Mediu16);

	void joc_pierdut(int x, int y);

	bool actiune_joc(int y, int x, bool deschideORmarcheaza);

	bool nivelValid(const int x, const int y, const int nrMine) const;

	Grila& getGrila() {
		return grila;
	}

	bool verificaJocCastigat();

	void salveaza_joc();

	void salveaza_scor();

	bool incarca_joc();

	bool validareDateFisier(string data);

	void setStartTime(chrono::time_point<std::chrono::steady_clock> start) {
		this->start = start;
	}
	chrono::time_point<std::chrono::steady_clock> getStartTime() const {
		return start;
	}

	float getTimp() const {
		return timp;
	}

	float CalculScor();

	string getCurrentDate();

	static string getScoruri();

	void endGame();
};


#endif