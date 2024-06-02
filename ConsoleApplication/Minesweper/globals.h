#pragma once
#include "Nivel.h"
#include <SFML/Graphics.hpp>
#include <vector>

const bool DEBUG = false;

const char* fontPath = "TellMeAJoke.ttf";

const int windowWidth = 1100;
const int windowHeight = 700;

const Color buttonColor = Color(192, 192, 192, 255);
const Color buttonHoverColor = Color(128, 128, 128, 255);
const Color buttonTextColor = Color::Black;
const Color buttonHoverTextColor = Color::White;
const Color buttonClickTextColor = Color::White;
const Color buttonBorderColor = Color::Black;
const Color Time_Mine_Color = Color::Transparent;
const Color n0 = Color::White;
const Color n1 = Color::Blue;
const Color n2 = Color::Green;
const Color n3 = Color::Red;
const Color n4 = Color(0, 0, 128);
const Color n5 = Color(128, 64, 64);
const Color n6 = Color::Cyan;
const Color n7 = Color::Black;
const Color n8 = Color(100, 100, 100);

const int FONT_SIZE = 28;
const int menuPosX = 50;
const int menuPosY = 50;
const int SPACE = 10;
const int cellSize = 30;
const int marginTop = 50;
const int cellsMargin = 5;
const int BUTTON_BORDER_THICKNESS = 2;
const int WIDTH = 170;
const int HEIGHT = 50;

const float xButtonText = 50;
const float yButtonText = 10;

const float scaleRate = 1.24f;
const int tableMargin = 30;

bool mainMenu = true;
bool chooseLevel = false;

Texture mineTexture;
Texture flagTexture;
Texture BOOMTexture;
Texture cellTexture;
Texture openCellTexture;
Texture backgroundTexture;

Cursor handCursor;
Cursor arrowCursor;



Font font;

struct Box {
	RectangleShape button;
	Text text;
};

struct cellBox {
	RectangleShape button;
	Text text;
	bool showText = false;
	int x = -1;
	int y = -1;
	int state = 0;
};

const vector<Nivel> levels = {
	Incepator9, Incepator16, Incepator30,
	Mediu9, Mediu16, Mediu30,
	Avansat9, Avansat16, Avansat30
};

struct Score {
	string date;
	string difficulty;
	int rows;
	int cols;
	double time;
	int mines;
	double score;
};

const string gameRules = 
"1. Jocul se desfasoara pe o grila rectangulara, avand un numar predefinit de mine ascunse.\n\
2. Fiecare celula din grila poate fi in una dintre urmatoarele stari :\n\
	a. Goala : Nu contine nicio mina si nu are asociat niciun numar.\n\
	b. Mina : Contine o mina ascunsa.Dezvaluirea unei astfel de celule duce la pierderea\n\
	   jocului.\n\
	c. Numar : Contine un numar care indica cate mine sunt invecinate cu celula\n\
	   respectiva.\n\
3. Jucatorul poate dezvalui o celula facand clic pe ea cu butonul stang al mouse - ului.\n\
4. Atunci cand o celula goala este dezvaluita, toate celulele goale adiacente, care nu contin\n\
   numere, sunt dezvaluite automat.\n\
5. Daca o celula cu numar este dezvaluita, vor fi afisate doar numarul respectiv si celulele\n\
   goale adiacente.\n\
6. Jucatorul poate marca o celula ca fiind suspecta de a contine o mina facand clic dreapta\n\
   pe ea.Celulele marcate vor fi evidentiate cu un steag.\n\
7. Dezmarcarea unei celule marcate se realizeaza facand din nou clic dreapta pe aceasta.\n\
8. Jucatorul castiga jocul daca reuseste sa dezvaluie toate celulele goale fara a detona vreo\n\
   mina.\n\
9. Jucatorul pierde jocul daca dezvaluie o celula care contine o mina. Numarul ramas de\n\
   mine este afisat in coltul din stanga, iar cronometrul jocului este afisat in coltul din\n\
   dreapta.";