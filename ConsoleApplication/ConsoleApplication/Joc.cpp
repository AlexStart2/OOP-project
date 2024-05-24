#include "Joc.h"
#include "Nivel.h"
#include "Grila.h"
#include <iostream>
#include <string>
#include <fstream>
#include <chrono>
#include <ctime>
#include <conio.h>


const char Joc::FISIER[20] = "joc.txt";
const char Joc::FISIER_SCOR[20] = "scores.txt";
const char Joc::FISIER_CONFIG[20] = "config.txt";

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
	if (ConsoleApplication) {
		x--;
		y--;
	}


	CalculScor();
	if (ConsoleApplication) {
		cout << "Joc pierdut! Celula (" << y+1 << ", " << x+1 << ") continea o mina!" << endl;
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
		_getch();
		system("cls");
		cout << "Scoruri:" << endl;
		cout << getScoruri();
	}
}



bool Joc::actiune_joc(int x, int y, bool deschideORmarcheaza) {
	if (deschideORmarcheaza) {
		return grila.deschide_celula(x, y);
	}
	else {
		grila.marcheaza_celula(x, y);
		return true;
	}
}

int Joc::nivelValid(const int x, const int y, const int nrMine) const {
	if (x < NR_MINIM_LINII || x > NR_MAXIM_LINII) {
		return 1;
	}
	if (y > NR_MAXIM_COLOANE || y < NR_MINIM_COLOANE) {
		return 2;
	}
	if (nrMine > x * y * NIVEL_MAX || nrMine < NR_MINIM_MINE) {
		return 3;
	}
	return 0;
}

bool Joc::verificaJocCastigat() {

	if (grila.nrMineMarcate == grila.numar_mine) {


		for (int i = 0; i < grila.nrLinii; i++) {
			for (int j = 0; j < grila.nrColoane; j++) {
				if ((grila.matrice[i][j].getTip() == Mina && grila.matrice[i][j].getStare() != Marcata) ||
					(grila.matrice[i][j].getTip() == Normala && grila.matrice[i][j].getStare() != Deschisa)) {
					return false;
				}
			}
		}

		CalculScor();
		salveaza_scor();

		if (ConsoleApplication) {
			cout << "Felicitari! Ati castigat!" << endl;
			cout << "Scorul dumneavoastra este: " << scor << endl;
			_getch();

			system("cls");
			cout << "Scoruri:" << endl;
			cout << getScoruri();
		}
		return true;
	}
	return false;
}

void Joc::salveaza_joc()
{
	ofstream file(FISIER);
	if (file.is_open()) {

		ofstream fileConfig(FISIER_CONFIG);
		if (fileConfig.is_open()) {
			auto end = chrono::high_resolution_clock::now();
			float t = (float)chrono::duration_cast<chrono::milliseconds>(end - start).count() / 1000 + timp;
			char name[20];
			// copy in name the name of the level but take only string before the first space
			strcpy_s(name, nivel.nume);
			name[strcspn(name, " ")] = 0;

			fileConfig << nivel.nrLinii << DELIMITER << nivel.nrColoane << DELIMITER
				<< nivel.nrMine << DELIMITER << name << DELIMITER << grila.nrMineMarcate 
				<< DELIMITER << t << endl;
			
			fileConfig.close();
		}
		else {
			if (ConsoleApplication) {
				cout << "Eroare la deschiderea fisierului!" << endl;
			}
			else {
				cout << "EROARE" << endl;
				string error = "Can not open the file ";
				error += FISIER_CONFIG;
				throw exception(error.c_str());
			}
		}


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
			cout << "EROARE2" << endl;
			string error = "Can not open the file ";
			error += FISIER;
			throw exception(error.c_str());
		}
	}
}

bool Joc::incarca_joc() {
	ifstream file(FISIER);
	if (file.is_open()) {
		int nrLinii, nrColoane, nrMine, nrMineMarcate;
		char nume[20];
		float timp;
		ifstream fileConfig(FISIER_CONFIG);
		if (fileConfig.is_open()) {
			fileConfig >> nrLinii >> nrColoane >> nrMine >> nume >> nrMineMarcate >> timp;

			nivel.nrColoane = nrColoane;
			nivel.nrLinii = nrLinii;
			nivel.nrMine = nrMine;
			strcpy_s(nivel.nume, nume);

			grila.nrMineMarcate = nrMineMarcate;
			grila.nrColoane = nrColoane;
			grila.nrLinii = nrLinii;
			grila.numar_mine = nrMine;
			fileConfig.close();
		}
		else {
			if (ConsoleApplication) {
				cout << "Eroare la deschiderea fisierului!" << endl;
				_getch();
				return false;
			}
			else {
				cout << "Eroare la deschiderea fisierului!" << endl;
				string error = "Can not open the file ";
				error += FISIER_CONFIG;
				throw exception(error.c_str());
			}
		}

		this->timp = timp;

		grila.matrice.clear();

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
						_getch();
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
			string error = "Can not open the file ";
			error += FISIER;
			throw exception(error.c_str());
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
	timp = t / 1000;
	scor = grila.nrMineMarcate / t * 100000;
	return scor;
}

float Joc::salveaza_scor() {

	ofstream file(FISIER_SCOR, ios::app);
	if (file.is_open()) {
		file << getCurrentDate() << DELIMITER << nivel.nume 
			<< DELIMITER << grila.getNrColoane() << DELIMITER << grila.getNrColoane() << timp  
			<< DELIMITER << grila.numar_mine << DELIMITER << grila.getNrMineMarcateGresit() 
			<< DELIMITER << scor << endl;

		file.close();
		return scor;
	}
	else {
		if (ConsoleApplication) {
			cout << "Eroare la deschiderea fisierului!" << endl;
		}
		else {
			string error = "Can not open the file ";
			error += FISIER_SCOR;
			throw exception(error.c_str());
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
			string error = "Can not open the file ";
			error += FISIER_SCOR;
			throw exception(error.c_str());
		}
	}
	return scoruri;
}


void Joc::endGame() {
	timp = 0;
	scor = 0;
	grila.matrice.clear();
}