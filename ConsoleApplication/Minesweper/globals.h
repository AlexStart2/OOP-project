#pragma once
#include "Nivel.h"
#include <SFML/Graphics.hpp>
#include <vector>

const bool DEBUG = true;

const char* fontPath = "TellMeAJoke.ttf";

const int windowWidth = 1000;
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
const float tableMargin = 30;

bool mainMenu = true;
bool chooseLevel = false;

Texture mineTexture;
Texture flagTexture;
Texture BOOMTexture;
Texture cellTexture;
Texture openCellTexture;

Cursor handCursor;
Cursor arrowCursor;

Texture backgroundTexture;

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

const std::vector<Nivel> levels = {
	Incepator9, Incepator16, Incepator30,
	Mediu9, Mediu16, Mediu30,
	Avansat9, Avansat16, Avansat30
};