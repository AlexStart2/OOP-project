
#include <iostream>
#include <string>
#include <cctype>
#include <chrono>
#include "Joc.h"
#include "Grila.h"



using namespace std;

void ruleaza_joc(Joc& minesweper);
int introdu_coordonata();
bool validInput(const string& _input);

int main()
{
    cout << "Start" << endl;
    Joc minesweper;
	
    char option = '0';

    Nivel level = Incepator9;

    do {
        cout << "1. Incepe joc" << endl;
        cout << "2. Configureaza nuvelul de greutate" << endl;
		cout << "3. Incarca joc" << endl;
        cout << "X. Exit" << endl;

        cout << "Alegeti optiune: ";
        cin >> option;
        option = toupper(option);
        switch (option)
        {
        case '1':
			system("cls");
            minesweper.incepe_joc(level);
			ruleaza_joc(minesweper);
            break;
        case '2':
			char opt;

			while (true) {
				system("cls");
				cout << "Alegeti nivelul de greutate: " << endl << endl;
				cout << "1. Incepator 9x9 10 mine" << endl;
				cout << "2. Incepator 16x16 40 mine" << endl;
				cout << "3. Incepator 16x30 75 mine" << endl;
				cout << "4. Mediu 9x9 20 mine" << endl;
				cout << "5. Mediu 16x16 68 mine" << endl;
				cout << "6. Mediu 16x30 120 mine" << endl;
				cout << "7. Avansat 9x9 31 mine" << endl;
				cout << "8. Avansat 16x16 96 mine" << endl;
				cout << "9. Avansat 16x30 167 mine" << endl << endl;
				cout << "0. Custom" << endl;
				cout << "Alegeti optiune: ";

				cin.get();
				opt = getchar();
				switch (opt) {
				case '1':
					level = Incepator9;
					break;
				case '2':
					level = Incepator16;
					break;
				case '3':
					level = Incepator30;
					break;
				case '4':
					level = Mediu9;
					break;
				case '5':
					level = Mediu16;
					break;
				case '6':
					level = Mediu30;
					break;
				case '7':
					level = Avansat9;
					break;
				case '8':
					level = Avansat16;
					break;
				case '9':
					level = Avansat30;
					break;
				case '0':
					cout << "Introduceti numarul de linii, coloane si mine: ";
					cin >> level.nrLinii >> level.nrColoane >> level.nrMine;
					if (!minesweper.nivelValid(level.nrLinii, level.nrColoane, level.nrMine)) {
						cout << "Nivelul introdus nu este valid" << endl;
						cin.get();
						getchar();
						system("cls");
						break;
					}
					system("cls");
					afiseaza_grila(minesweper.incepe_joc(level));
					ruleaza_joc(minesweper);
					break;
				default:
					cout << "Optiune gresita" << endl;
					cin.get();
					getchar();
					system("cls");
					break;
				}

				if (opt-'0' >= 1 && opt-'0' <= 9)
				{
					break;
				}
			}
			system("cls");
			minesweper.incepe_joc(level);
			ruleaza_joc(minesweper);

            break;

        case 'X':
            cout << "End" << endl;
            break;
		case '3':
			if (!minesweper.incarca_joc()) {
				cout << "Jocul nu a putut fi incarcat" << endl;
				cin.get();
				getchar();
				break;
			}
			system("cls");
			ruleaza_joc(minesweper);
			break;
        default:
            cout << "Optiune gresita" << endl;
			cin.get();
			getchar();
            break;
        }

    } while (option != 'X');

    return 0;
}

void ruleaza_joc(Joc& minesweper) {

	int x, y;
	char opt;


	minesweper.setStartTime(chrono::high_resolution_clock::now());

	do {
		cout << "Timpul: " << (float)chrono::duration_cast<chrono::milliseconds>(chrono::high_resolution_clock::now() - minesweper.getStartTime()).count()/1000 + minesweper.getTimp() << "\b secunde" << endl;
		afiseaza_grila(minesweper.getGrila());
		cout << "Alegeti actiunea: " << endl;
		cout << "1. Deschide celula" << endl;
		cout << "2. Marcheaza celula" << endl;
		cout << "3. Salveaza joc" << endl;
		cout << "X. Exit" << endl;
		cout << "Alegeti optiune: ";
		cin >> opt;
		opt = toupper(opt);
		switch (opt)
		{
		case '1':
			cout << "Alegeti coordonatele celulei" << endl;
			cout << "Nr. coloana: ";
			x = introdu_coordonata();
			cout << "Nr. linie: ";
			y = introdu_coordonata();

			if (!minesweper.getGrila().coordonateValide(x, y)) {
				cout << "Coordonate invalide!" << endl;
				cin.get();
				getchar();
				break;
			}

			if (!minesweper.actiune_joc(x, y, true)) {
				minesweper.joc_pierdut(x, y);
				cout << "Doriti sa salvati scorul? (D/N): ";
				cin >> opt;
				opt = toupper(opt);
				if (opt == 'D') {
					minesweper.salveaza_scor();
				}
				return;
			}
			else if(minesweper.verificaJocCastigat()){
				return;
			}
			break;
		case '2':
			cout << "Alegeti coordonatele celulei"<< endl;
			cout << "Nr. coloana: ";
			x = introdu_coordonata();
			cout << "Nr. linie: ";
			y = introdu_coordonata();

			if (!minesweper.getGrila().coordonateValide(x, y)) {
				cout << "Coordonate invalide!" << endl;
				cin.get();
				getchar();
				break;
			}
			minesweper.actiune_joc(x, y, false);
			if (minesweper.verificaJocCastigat()) {
				return;
			}
			break;

		case '3':
			minesweper.salveaza_joc();
			cout << "Jocul a fost salvat" << endl;
			cin.get();
			getchar();
			break;
		case 'X':
			cout << "End" << endl;
			cin.get();
			getchar();
			system("cls");
			return;
		default:
			cout << "Optiune gresita" << endl;
			cin.get();
			getchar();
			break;
		}
		system("cls");
	} while (true);
}


void afiseaza_grila(const Grila& grila) {  // se poate de pus cu culori putin
	for (int i = 0; i < grila.getNrLinii(); i++) {
		for (int j = 0; j < grila.getNrColoane(); j++) {
			if (grila.matrice[i][j].getStare() == Deschisa) {
				cout << grila.matrice[i][j].getNrVecini() << "  ";
			}
			else if (grila.matrice[i][j].getStare() == Marcata) {
				cout << "M  ";
			}
			else {
				cout << "[] ";
			}
		}
		cout << endl;
	}
	cout << endl;
	cout << "Numarul de mine ramase: " << grila.getNumarMine() - grila.getNrMineMarcate() << endl;
}

int introdu_coordonata() {
	
	char cstr[100] = "1";
	cin >> cstr;

	while (!validInput(cstr)) {
		cout << "Coordonate invalide! Introduceti alt numar: ";
		cin >> cstr;
	}
	int x = stoi(cstr);
	return x;
}


bool validInput(const string& _input) {

	for (int i = 0; i < _input.length(); i++) {
		if (!isdigit(_input[i])) {
			return false;
		}
	}
	return true;
}