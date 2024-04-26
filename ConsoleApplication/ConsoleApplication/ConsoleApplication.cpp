
#include <iostream>
#include "Joc.h"
#include "Grila.h"

using namespace std;

void ruleaza_joc(Joc& minesweper);

int main()
{
    cout << "Start" << endl;
    Joc minesweper;

    char option = '0';

    Nivel level = Incepator9;

    do {
        cout << "1. Incepe joc" << endl;
        cout << "2. Configureaza nuvelul de greutate" << endl;
        cout << "X. Exit" << endl;

        cout << "Alegeti optiune: ";
        cin >> option;
        option = toupper(option);
        switch (option)
        {
        case '1':
			system("cls");
            afiseaza_grila(minesweper.incepe_joc(level));
			ruleaza_joc(minesweper);
            break;
        case '2':
			int opt;

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

				cin >> opt;
				switch (opt) {
				case 1:
					level = Incepator9;
					break;
				case 2:
					level = Incepator16;
					break;
				case 3:
					level = Incepator30;
					break;
				case 4:
					level = Mediu9;
					break;
				case 5:
					level = Mediu16;
					break;
				case 6:
					level = Mediu30;
					break;
				case 7:
					level = Avansat9;
					break;
				case 8:
					level = Avansat16;
					break;
				case 9:
					level = Avansat30;
					break;
				case 0:
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
				if (opt >= 1 && opt <= 9)
				{
					break;
				}
			}
			system("cls");
			afiseaza_grila(minesweper.incepe_joc(level));
			ruleaza_joc(minesweper);

            break;

        case 'X':
            cout << "End" << endl;
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

void ruleaza_joc(Joc& minesweper) {  //////////////////////// trebuie de facut update la grila dupa fiecare actiune

	int x, y;
	char opt;

	do {
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
			cout << "Alegeti coordonatele celulei: ";
			cin >> x >> y;
			if (!minesweper.actiune_joc(x, y, true)) {
				minesweper.termina_joc();
			}
			else {
				minesweper.verificaJocCastigat();
			}
			break;
		case '2':
			cout << "Alegeti coordonatele celulei: ";
			cin >> x >> y;
			minesweper.actiune_joc(x, y, false);
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
		afiseaza_grila(minesweper.getGrila());
	} while (true);
}


void afiseaza_grila(const Grila& grila) {  // trebuie de terminat
	for (int i = 0; i < grila.nrLinii; i++) {
		for (int j = 0; j < grila.nrColoane; j++) {
			cout << grila.matrice[i][j].getStare() << " ";
		}
		cout << endl;
	}
}