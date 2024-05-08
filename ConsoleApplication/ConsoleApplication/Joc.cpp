#include "Joc.h"
#include "Nivel.h"
#include "Grila.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>


Joc::Joc() {
	nivel = Mediu16;
	scor = 0;
	timp = 0;
}

Grila& Joc::incepe_joc(Nivel nivel) {
	scor = 0;
	this->nivel = nivel;
	grila = grila.initializare(nivel);
	return grila;
}

void Joc::joc_pierdut(int y, int x) {
	x--;
	y--;

	CalculScor();
	if (ConsoleApplication) {
		cout << "Joc pierdut! Celula (" << ++y << ", " << ++x << ") continea o mina!" << endl;
		for (int i = 0; i < grila.nrLinii; i++) {
			for (int j = 0; j < grila.nrColoane; j++) {
				if (x == i && y == j) {
					cout << "X-";
					continue;
				}
					
				if (grila.matrice[i][j].getStare() == Marcata) {
					if (grila.matrice[i][j].getTip() == Mina) {
						cout << "C ";
					}
					else {
						cout << "G ";
					}
				}
				else if (grila.matrice[i][j].getTip() == Mina) {
					cout << "X ";
				}
				else if (grila.matrice[i][j].getTip() == Normala) {
					cout << grila.matrice[i][j].getNrVecini() << " ";
				}
				else if (grila.matrice[i][j].getNrVecini() == 0) {
					cout << "0 ";
				}
				else {
					cout << grila.matrice[i][j].getNrVecini() << " ";
				}
			}
			cout << endl;
		}

		cout << "Scorul dumneavoastra este: " << scor << endl;
		cin.get();
		getchar();
	}
	else {
		// TODO - implement Joc::joc_pierdut
		throw exception("Not yet implemented");
	}
}



bool Joc::actiune_joc(int y, int x, bool deschideORmarcheaza) {
	x--;
	y--;
	if (deschideORmarcheaza) {
		return grila.deschide_celula(x, y);
	}
	else {
		grila.marcheaza_celula(x, y);
	}
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

	if (grila.nrMineMarcate == grila.numar_mine) {


		for (int i = 0; i < grila.nrLinii; i++) {
			for (int j = 0; j < grila.nrColoane; j++) {
				if (grila.matrice[i][j].getTip() == Mina && grila.matrice[i][j].getStare() != Marcata) {
					//||  (grila.matrice[i][j].getTip() == Normala && grila.matrice[i][j].getStare() != Deschisa) ??
					return false;
				}
			}
		}

		CalculScor();
		salveaza_scor();

		if (ConsoleApplication) {
			cout << "Felicitari! Ati castigat!" << endl;
			cout << "Scorul dumneavoastra este: " << scor << endl;
			cin.get();
			getchar();
		}
		else {
			// TODO - implement Joc::verificaJocCastigat
			throw exception("Not yet implemented");
		}
		return true;
	}
	return false;
}

void Joc::salveaza_joc()
{
	ofstream file(FISIER);
	if (file.is_open()) {
		auto end = chrono::high_resolution_clock::now();
		float t = (float)chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000 + timp;

		file << nivel.nrLinii << DELIMITER << nivel.nrColoane << DELIMITER << nivel.nrMine << DELIMITER << grila.nrMineMarcate << DELIMITER << t << endl;

		for (int i = 0; i < grila.nrLinii; i++) {
			for (int j = 0; j < grila.nrColoane; j++) {
				if (grila.matrice[i][j].getStare() == Deschisa) {
					file << grila.matrice[i][j].getNrVecini() << DELIMITER;
				}
				else if (grila.matrice[i][j].getStare() == Inchisa) {
					if (grila.matrice[i][j].getTip() == Mina)
						file << "X" << DELIMITER;
					else
						file << grila.matrice[i][j].getNrVecini() << "$" << DELIMITER;
				}
				else if (grila.matrice[i][j].getStare() == Marcata) {
					if (grila.matrice[i][j].getTip() == Mina) {
						file << "C" << DELIMITER;
					}
					else {
						file << grila.matrice[i][j].getNrVecini()<<"G"<< DELIMITER;
					}
				}
			}
			file << endl;
		}
		file.close();
	}
	else {
		if (ConsoleApplication) {
			cout << "Eroare la deschiderea fisierului!" << endl;
		}
		else {
			// TODO - implement Joc::salveaza_joc
			throw exception("Not yet implemented");
		}
	}
}

bool Joc::incarca_joc() {
	ifstream file(FISIER);
	if (file.is_open()) {
		int nrLinii, nrColoane, nrMine, nrMineMarcate;
		float timp;
		file >> nrLinii >> nrColoane >> nrMine >> nrMineMarcate >> timp;

		nivel.nrColoane = nrColoane;
		nivel.nrLinii = nrLinii;
		nivel.nrMine = nrMine;

		grila.nrMineMarcate = nrMineMarcate;
		grila.nrColoane = nrColoane;
		grila.nrLinii = nrLinii;
		grila.numar_mine = nrMine;

		this->timp = timp;

		grila.matrice.resize(nrLinii, vector<Celula>(nrColoane));
		
		for (int i = 0; i < grila.nrLinii; i++) {
			for (int j = 0; j < grila.nrColoane; j++) {
				string line;
				getline(file, line, DELIMITER);

				if (line.find('\n') != string::npos) {
					line = line.replace(line.find('\n'), 1, "");
				}
				
				if (!validareDateFisier(line)) {
					if (ConsoleApplication) {
						cout << "Datele din fisier sunt invalide!" << endl;
						cin.get();
						getchar();
						return false;
					}
					else {
						// TODO - implement Joc::incarca_joc
						throw exception("Not yet implemented");
					}
				}

				if (line == "X") {
					grila.matrice[i][j].setTip(Mina);
					grila.matrice[i][j].setStare(Inchisa);
				}
				else if (line == "C") {
					grila.matrice[i][j].setTip(Mina);
					grila.matrice[i][j].setStare(Marcata);
				}
				else if (line.length() != 1) {
					if (line[1] == '$') {
						grila.matrice[i][j].setNrVecini(line[0] - '0');
						grila.matrice[i][j].setTip(Normala);
						grila.matrice[i][j].setStare(Inchisa);
					}
					else if (line[1] == 'G') {
						grila.matrice[i][j].setNrVecini(line[0] - '0');
						grila.matrice[i][j].setTip(Normala);
						grila.matrice[i][j].setStare(Marcata);
					}
				}
				else {
					grila.matrice[i][j].setNrVecini(line[0] - '0');
					grila.matrice[i][j].setTip(Normala);
					grila.matrice[i][j].setStare(Deschisa);
				}
			}
		}
		file.close();
		return true;
	}
	else {
		if (ConsoleApplication) {
			cout << "Eroare la deschiderea fisierului!" << endl;
			return false;
		}
		else {
			return false;
		}
	}
}

bool Joc::validareDateFisier(string data) {

	if (data.length() == 2) {
		if (data[0] >= '0' && data[0] <= '8' && (data[1] != 'G' || data[1] != '$')) {
			return true;
		}
		else {
			//exceptie
			return false;
		}
	}
	else if (data.length() == 1 && isdigit(data[0])) {
		if (data[0] < '0' || data[0] > '9') {
			//exceptie
			return false;
		}
		else {
			return true;
		}
	}
	else if(data == "C" || data == "X") {
		return true;
	}
	else {
		//exceptie
		return false;
	}
	
}


float Joc::CalculScor() {
	auto end = chrono::high_resolution_clock::now();
	float t = (float)chrono::duration_cast<chrono::milliseconds>(end - start).count() + timp;
	timp = t;
	scor = grila.nrMineMarcate / t * 100000;
	return scor;
}

void Joc::salveaza_scor() {

	
	ofstream file(FISIER_SCOR, ios::app);
	if (file.is_open()) {

		file << getCurrentDate() << DELIMITER << nivel.nume 
			<< DELIMITER << grila.nrLinii << "x" << grila.nrColoane << DELIMITER
			<< timp  << DELIMITER << grila.numar_mine << DELIMITER << grila.getNrMineMarcateGresit() 
			<< DELIMITER << scor << endl;

		file.close();
	}
	else {
		if (ConsoleApplication) {
			cout << "Eroare la deschiderea fisierului!" << endl;
		}
		else {
			// TODO - implement Joc::salveaza_scor
			throw exception("Not yet implemented");
		}
	}
}

string Joc::getCurrentDate() {

	time_t now = time(0);
	tm ltm;
	localtime_s(&ltm, &now);

	string date = to_string(1900 + ltm.tm_year) + "-" + to_string(1 + ltm.tm_mon) 
		+ "-" + to_string(ltm.tm_mday);
	return date;
}

string Joc::getScoruri() {
	ifstream file(FISIER_SCOR);
	string line;
	string scoruri = "";
	if (file.is_open()) {
		while (getline(file, line)) {
			scoruri += line + "\n";
		}
		file.close();
	}
	else {
		if (ConsoleApplication) {
			cout << "Eroare la deschiderea fisierului!" << endl;
		}
		else {
			// TODO - implement Joc::getScoruri
			throw exception("Not yet implemented");
		}
	}
	return scoruri;
}