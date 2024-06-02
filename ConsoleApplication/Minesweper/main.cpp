#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <vector>
#include <string>
#include <sstream>
#include <iomanip>
#include <windows.h>

#include "TextBox.h"
#include "globals.h"
#include "Components.h"
#include "Joc.h"
#include "Nivel.h"
#include "Grila.h"

#define _WIN32_WINNT 0x0500

using namespace sf;
using namespace std;

int main()
{ 
	hideConsoleWindow();
	loadResources();

    RenderWindow window(VideoMode(windowWidth, windowHeight), "Minesweper");
	window.setFramerateLimit(40);

	Sprite  background(backgroundTexture);

	background.setScale(windowWidth / background.getLocalBounds().width, windowHeight / 
		background.getLocalBounds().height);

	Joc minesweper;

	int i = 0;
	Box newGameButton = createButton("Joc nou", menuPosX, (menuPosY + SPACE) * ++i);
	Box loadGameButton = createButton("Incarca joc", menuPosX, (menuPosY + SPACE) * ++i);
	Box showScoresButton = createButton("Clasament", menuPosX, (menuPosY + SPACE) * ++i);
	Box exitButton = createButton("Iesire", menuPosX, (menuPosY + SPACE) * ++i);

	Box returnButton = createButton("<", 5, 5, 20, 20);
	returnButton.text.setFont(font);

	vector<Box> levelButtons;
	int buttonY = 50;
	for (const auto& level : levels) {
		Box button = createButton(level.nume, menuPosX, buttonY);
		levelButtons.push_back(button);
		buttonY += HEIGHT + 10;
	}

	int posX = (int) (menuPosX * 2.5 + WIDTH * 2);
	int textPosX = menuPosX * 2 + WIDTH;
	int k = 1;
	const int sizeX = 200;
	const int sizeY = 40;
	Text rowsText = createText("Numarul de linii: ", textPosX, menuPosY * ++k, FONT_SIZE);
	TextBox rowsTextBox((float)posX, (float)menuPosY * k, sizeX, sizeY, font, FONT_SIZE);
	Text columnsText = createText("Numarul de coloane: ", textPosX, menuPosY * ++k, FONT_SIZE);
	TextBox columnsTextBox((float)posX,(float) menuPosY * k, sizeX, sizeY, font, FONT_SIZE);
	Text minesText = createText("Numarul de mine: ", textPosX, menuPosY * ++k, FONT_SIZE);
	TextBox minesTextBox((float)posX, (float) menuPosY * k, sizeX, sizeY, font, FONT_SIZE);
	Box startButton = createButton("Start", posX, menuPosY * ++k);

	Text errorText = createText("", posX, menuPosY * ++k, FONT_SIZE);
	errorText.setFillColor(Color::Red);

	Text titleGameRules = createText("Reguli joc", textPosX + 20, 20, FONT_SIZE + 3);
	Text txtGameRules = createText(gameRules,textPosX, 60, FONT_SIZE - 2);

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
			if (event.type == Event::Closed) {
				window.close();
				return 0;
			}
                
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0f, 0.0f,(float) event.size.width, (float)event.size.height)));
			}

			if (mainMenu) {
				EventMenu(window, event, newGameButton, loadGameButton, showScoresButton, exitButton, minesweper);
			}
			else if (chooseLevel) {
				EventLevelMenu(window, event, returnButton, levelButtons, minesweper, rowsTextBox,
					columnsTextBox, minesTextBox, startButton, errorText);
			}
        }

		window.clear(Color::White);
		window.setSize(Vector2u(windowWidth, windowHeight));
		window.draw(background);

		if (mainMenu) {
			window.draw(txtGameRules);
			window.draw(titleGameRules);
			drawMenu(window, newGameButton, loadGameButton, showScoresButton, exitButton);
		}
		else if (chooseLevel) {
			window.draw(rowsText);
			window.draw(columnsText);
			window.draw(minesText);
			window.draw(errorText);
			drawLevelMenu(window, levelButtons, returnButton, rowsTextBox, columnsTextBox, 
				minesTextBox, startButton);
		}
		
        window.display();
    }

    return 0;
}


void hideConsoleWindow() {
	HWND hWnd = GetConsoleWindow();
	ShowWindow(hWnd, SW_HIDE);
}

void loadResources() {
	try {
		if (!backgroundTexture.loadFromFile("Texture/background.jpg")) {
			throw runtime_error("Error loading background texture!");
		}
		if (!font.loadFromFile(fontPath)) {
			throw runtime_error("Error loading font!");
		}
		handCursor.loadFromSystem(Cursor::Hand);
		arrowCursor.loadFromSystem(Cursor::Arrow);
	}
	catch (const exception& e) {
		ErrorMessageBox(e.what());
		exit(-1);
	}
}

Box createButton(const string& text, int x, int y, float width, float height)
{
	Box button;

	button.button.setSize(Vector2f(width, height));
	button.button.setPosition((float)x, (float)y);
	button.button.setFillColor(buttonColor);
	button.button.setOutlineThickness(BUTTON_BORDER_THICKNESS);
	button.button.setOutlineColor(buttonBorderColor);

	if (text.empty()) {
		return button;
	}

	button.text.setFont(font);
	button.text.setString(text);
	button.text.setCharacterSize(FONT_SIZE);
	button.text.setFillColor(buttonTextColor);

	FloatRect textRect = button.text.getLocalBounds();
	button.text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	button.text.setPosition(x + width / 2.0f, y + height / 2.0f);
	return button;
}

void buttonHover(Box& button, RenderWindow& window, bool hover)
{
	if (hover) {
		button.button.setFillColor(buttonHoverColor);
		button.text.setFillColor(buttonHoverTextColor);
	}
	else {
		button.button.setFillColor(buttonColor);
		button.text.setFillColor(buttonTextColor);
	}
}

bool isCursorOverShape(const RectangleShape& shape, const RenderWindow& window)
{
	Vector2i mousePos = Mouse::getPosition(window);
	Vector2f buttonPos = shape.getPosition();
	Vector2f buttonSize = shape.getSize();
	if (mousePos.x >= buttonPos.x && mousePos.x <= buttonPos.x + buttonSize.x &&
		mousePos.y >= buttonPos.y && mousePos.y <= buttonPos.y + buttonSize.y)
	{
		return true;
	}
	return false;
}

void EventMenu(RenderWindow& window, Event& event, Box& newGameButton,
	Box& loadGameButton, Box& showScoresButton, Box& exitButton, Joc& minesweper)
{
	if (event.type == Event::MouseMoved || event.type == Event::MouseButtonPressed)
	{
		bool hover = false;
		if (isCursorOverShape(newGameButton.button, window)) {
			buttonHover(newGameButton, window, true);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				window.clear(Color::White);
				mainMenu = false;
				chooseLevel = true;
				if (DEBUG) {
					cout << "New Game button clicked!" <<endl;
				}
			}
			hover = true;
		}
		else {
			buttonHover(newGameButton, window, false);
		}

		if (isCursorOverShape(loadGameButton.button, window)) {
			buttonHover(loadGameButton, window, true);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (DEBUG) {
					cout << "Load Game button clicked!" << endl;
				}
				try {
					if (!minesweper.incarca_joc()) {
						cerr << "Eroare la incarcarea datelor din fisier" << endl;
					}
				}
				catch (const exception& e) {
					ErrorMessageBox(e.what());
					return;
				}
				
				mainMenu = false;
				chooseLevel = false;
				startGame(window, minesweper, minesweper.getNivel(), true);
			}
			hover = true;
		}
		else {
			buttonHover(loadGameButton, window, false);
		}

		if (isCursorOverShape(showScoresButton.button, window)) {
			buttonHover(showScoresButton, window, true);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (DEBUG) {
					cout << "Show Scores button clicked!" << endl;
				}
				scoresWindow(minesweper);
			}
			hover = true;
		}
		else {
			buttonHover(showScoresButton, window, false);
		}

		if (isCursorOverShape(exitButton.button, window)) {
			buttonHover(exitButton, window, true);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				window.close();
				return;
			}
			hover = true;
		}
		else {
			buttonHover(exitButton, window, false);
		}

		if (hover) {
			window.setMouseCursor(handCursor);
		}
		else {
			window.setMouseCursor(arrowCursor);
		}
	}
}

void EventLevelMenu(RenderWindow& window, Event& event, Box& returnButton,
	vector<Box>& levelButtons, Joc& minesweper, TextBox& rowsTextBox,
	TextBox& columnsTextBox, TextBox& minesTextBox, Box& startButton, Text& errorMessage) {

	rowsTextBox.handleEvent(event);
	columnsTextBox.handleEvent(event);
	minesTextBox.handleEvent(event);

	bool hover = false;

	if (isCursorOverShape(startButton.button, window)) {
		buttonHover(startButton, window, true);
		hover = true;
		if (Mouse::isButtonPressed(Mouse::Left)) {
			
			string rows;
			string columns;
			string mines;
			int nrLinii = 0;
			int nrColoane = 0;
			int nrMine = 0;
			errorMessage.setString("");
			try {
				rows = rowsTextBox.getText();
				columns = columnsTextBox.getText();
				mines = minesTextBox.getText();

				if (rows.empty() || columns.empty() || mines.empty()) {
					errorMessage.setString("Toate campurile sunt obligatorii!");
					return;
				}

				nrLinii = stoi(rows);
				nrColoane = stoi(columns);
				nrMine = stoi(mines);

				int code = minesweper.nivelValid(nrLinii, nrColoane, nrMine);

				if (code != 0) {
					throw code;
				}
			}
			catch (int errorCode) {
				if (errorCode == 1) {
					errorMessage.setString("Numarul de linii trebuie sa fie intre " +
						to_string(minesweper.NR_MINIM_LINII) + " si " + to_string(minesweper.NR_MAXIM_LINII));
				}
				else if (errorCode == 2) {
					errorMessage.setString("Numarul de coloane trebuie sa fie intre " +
						to_string(minesweper.NR_MINIM_COLOANE) + " si " + to_string(minesweper.NR_MAXIM_COLOANE));
				}
				else if (errorCode == 3) {
					int max = (int)(minesweper.NIVEL_MAX * nrLinii * nrColoane);
					errorMessage.setString("Numarul de mine trebuie sa fie intre " +
						to_string(minesweper.NR_MINIM_MINE) + " si " + to_string(max));
				}
				return;
			}
			catch (const exception& e) {
				ErrorMessageBox(e.what());
				return;
			}
			

			Nivel level = { "Manual", nrLinii, nrColoane, nrMine };

			startGame(window, minesweper, level, false);
		}
	}
	else {
		buttonHover(startButton, window, false);
	}


	if (isCursorOverShape(returnButton.button, window)) {
		buttonHover(returnButton, window, true);
		hover = true;
		if (Mouse::isButtonPressed(Mouse::Left)) {
			mainMenu = true;
			chooseLevel = false;
		}
	}
	else {
		buttonHover(returnButton, window, false);
	}

	for (auto& button : levelButtons) {
		if (isCursorOverShape(button.button, window)) {
			buttonHover(button, window, true);
			hover = true;
			if (Mouse::isButtonPressed(Mouse::Left)) {
				if (DEBUG) {
					cout << button.text.getString().toAnsiString() << " button clicked!" << endl;
				}

				string levelName = button.text.getString().toAnsiString();
				Nivel level = Incepator16;
				for (const auto& l : levels) {
					if (l.nume == levelName) {
						level = l;
						break;
					}
				}

				startGame(window, minesweper, level, false);
			}
		}
		else {
			buttonHover(button, window, false);
		}
	}
	if (hover) {
		window.setMouseCursor(handCursor);
	}
	else {
		window.setMouseCursor(arrowCursor);
	}
}

void drawMenu(RenderWindow& window, Box& newGameButton,
	Box& loadGameButton, Box& showScoresButton, Box& exitButton)
{
	window.draw(newGameButton.button);
	window.draw(newGameButton.text);

	window.draw(loadGameButton.button);
	window.draw(loadGameButton.text);

	window.draw(showScoresButton.button);
	window.draw(showScoresButton.text);

	window.draw(exitButton.button);
	window.draw(exitButton.text);
}

void drawLevelMenu(RenderWindow& window, vector<Box>& levelButtons, Box& returnButton,
	TextBox& rowsTextBox, TextBox& columnsTextBox, TextBox& minesTextBox, Box& startButton)
{
	for (Box button : levelButtons) {
		button.text.setFont(font);
		window.draw(button.button);
		window.draw(button.text);
	}

	window.draw(returnButton.button);
	window.draw(returnButton.text);

	rowsTextBox.draw(window);
	columnsTextBox.draw(window);
	minesTextBox.draw(window);

	window.draw(startButton.button);
	window.draw(startButton.text);
}

void startGame(RenderWindow& window, Joc& minesweper, const Nivel level, bool loaded)
{
	try {
		if (!mineTexture.loadFromFile("Texture/openCellMine.bmp")) {
			throw exception("Error loading mine texture!");
		}
		if (!flagTexture.loadFromFile("Texture/flag (1).png")) {
			throw exception("Error loading flag texture!");
		}
		if (!BOOMTexture.loadFromFile("Texture/boomMine.bmp")) {
			throw exception("Error loading blast texture!");
		}
		if (!cellTexture.loadFromFile("Texture/cell.bmp")) {
			throw exception("Error loading cell texture!");
		}
		if (!openCellTexture.loadFromFile("Texture/openCell.bmp")) {
			throw exception("Error loading open cell texture!");
		}
	}
	catch (const exception& e) {
		ErrorMessageBox(e.what());
		return;
	}
	

	Grila& grila = minesweper.getGrila();
	if (!loaded) {
		grila = minesweper.incepe_joc(level);
	}

	int width = grila.getNrColoane() * cellSize + tableMargin;
	int height = grila.getNrLinii() * cellSize + marginTop + tableMargin / 2;

	Sprite  background(backgroundTexture);
	background.setScale(width / background.getLocalBounds().width, height /
		background.getLocalBounds().height);

	vector<cellBox> cells;

	int btnMarginTop = 10;

	Box mineCounter = createButton(to_string(grila.getNumarMine()), 40, btnMarginTop);
	FloatRect textRect = mineCounter.text.getLocalBounds();
	mineCounter.button.setSize(Vector2f(textRect.width + 30, textRect.height + btnMarginTop));
	float mCtextPosX = mineCounter.button.getPosition().x + mineCounter.button.getSize().x / 2.0f;
	float mCtextPosY = mineCounter.button.getPosition().y + mineCounter.button.getSize().y / 2.0f;
	mineCounter.text.setString(to_string(grila.getNumarMine() - grila.getNrMineMarcate()));
	mineCounter.text.setPosition(mCtextPosX, mCtextPosY);

	int tCPosX = width - 95;
	int tCPosY = 10;

	Box timeCounter = createButton("0", tCPosX, tCPosY, 60, textRect.height + 10);
	minesweper.setStartTime(chrono::high_resolution_clock::now());

	Box returnButton = createButton("<", 5, 5, 20, 20);

	Box restartButton = createButton("R", width/2 - 20, tCPosY, textRect.width + 30, textRect.height + btnMarginTop);

	for (int i = 0; i < grila.getNrLinii(); i++) {
		for (int j = 0; j < grila.getNrColoane(); j++) {
			string text = to_string(grila.getCell(i, j).getNrVecini());
			cellBox cell = newCell(j * (cellSize) + (float) tableMargin / 2, (float) i * cellSize
				+ marginTop, cellSize, grila.getCell(i, j), i, j, grila.getCell(i, j).getStare());
			cells.push_back(cell);
		}
	}

	window.setMouseCursor(arrowCursor);

	bool running = true;
	bool gameOver = false;
	bool gameWon = false;
	bool firstClick = true;
	bool wonMessage = false;
	float scor = 0.0f;
	float time = 0;

	Box winMessage;
	while (running)
	{
		bool hover = false;
		int k = 0;
		Event event;
		while (window.pollEvent(event))
		{
			if (event.type == Event::Closed) {
				if (!gameOver && firstClick) {
					firstClick = false;
					int opt = confirmToSave();
					if (opt == 0) {
						window.close();
						return;
					}
					else if (opt == 1) {
						minesweper.salveaza_joc();
						window.close();
						return;
					}
					else {
						firstClick = true;
						break;
					}
				}
				window.close();
				return;
			}

			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0f, 0.0f, (float)event.size.width, (float)event.size.height)));
			}

			if (isCursorOverShape(restartButton.button, window)) {
				buttonHover(restartButton, window, true);
				hover = true;
				if (Mouse::isButtonPressed(Mouse::Left)) {
					minesweper.endGame();
					startGame(window, minesweper, level, false);
					return;
				}
			}
			else {
				buttonHover(restartButton, window, false);
			}

			if (isCursorOverShape(returnButton.button, window)) {
				buttonHover(returnButton, window, true);
				hover = true;
				if (Mouse::isButtonPressed(Mouse::Left)) {
					mainMenu = true;
					chooseLevel = false;
					running = false;
					break;
				}
			}
			else {
				buttonHover(returnButton, window, false);
			}

			if (hover) {
				window.setMouseCursor(handCursor);
			}
			else {
				window.setMouseCursor(arrowCursor);
			}

			if (!gameOver) {
				if (Mouse::isButtonPressed(Mouse::Right)) {
					for (int i = 0; i < cells.size(); i++) {
						if (isCursorOverShape(cells[i].button, window)) {
							if (grila.getNrMineMarcate() != grila.getNumarMine() &&
								grila.getCell(cells[i].x, cells[i].y).getStare() == Inchisa) {
								minesweper.actiune_joc(cells[i].x, cells[i].y, false);
								cells[i].button.setTexture(&flagTexture);
								mineCounter.text.setString(to_string(grila.getNumarMine() - grila.getNrMineMarcate()));
								if (minesweper.verificaJocCastigat()) {
									gameWon = true;
									gameOver = true;
								}
								break;
							}
							if (grila.getCell(cells[i].x, cells[i].y).getStare() == Marcata) {
								minesweper.actiune_joc(cells[i].x, cells[i].y, false);
								cells[i].button.setTexture(&cellTexture);
							}
							mineCounter.text.setString(to_string(grila.getNumarMine() - grila.getNrMineMarcate()));
							break;
						}
					}
				}
				if (Mouse::isButtonPressed(Mouse::Left)) {
					for (int i = 0; i < cells.size(); i++) {
						if (isCursorOverShape(cells[i].button, window)) {
							if (grila.getCell(cells[i].x, cells[i].y).getStare() == Marcata) {
								break;
							}

							if (!minesweper.actiune_joc(cells[i].x, cells[i].y, true)) {
								for (int j = 0; j < cells.size();j++) {
									if (grila.getCell(cells[j].x, cells[j].y).getTip() == Mina) {
										grila.getCell(cells[j].x, cells[j].y).setStare(Deschisa);
										cells[j].button.setTexture(&mineTexture);
									}
								}
								cells[i].button.setTexture(&BOOMTexture);
								gameOver = true;
								break;
							}
							else {
								for (int i = 0; i < cells.size();i++) {
									if (grila.getCell(cells[i].x, cells[i].y).getStare() == Deschisa) {
										cells[i].button.setTexture(&openCellTexture);
										cells[i].showText = true;
									}
								}
								if (minesweper.verificaJocCastigat()) {
									gameWon = true;
									gameOver = true;
								}
								break;
							}
						}
					}
				}
			}

		}
		window.clear(Color::White);
		window.draw(background);

		window.setSize(Vector2u(width, height));

		for (cellBox button : cells) {
			window.draw(button.button);
			if (button.showText) {
				button.text.setFont(font);
				window.draw(button.text);
			}
		}

		if (!gameOver) {
			time = chrono::duration_cast<chrono::seconds>
				(chrono::high_resolution_clock::now() - minesweper.getStartTime()).count() + minesweper.getTimp();
			FloatRect textRect = timeCounter.text.getLocalBounds();
			timeCounter.text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
			timeCounter.text.setPosition(tCPosX + 60 / 2.0f, textRect.height + tCPosY / 2.0f);
		}
		timeCounter.text.setString(to_string((int)time));

		window.draw(restartButton.button);
		window.draw(restartButton.text);

		window.draw(returnButton.button);
		window.draw(returnButton.text);

		window.draw(timeCounter.button);
		window.draw(timeCounter.text);

		window.draw(mineCounter.button);
		window.draw(mineCounter.text);

		if (gameWon && !wonMessage) {
			scor = minesweper.getScor();
			scor = ceil(scor * 100.0f) / 100.0f;

			char buffer[50];
			sprintf_s(buffer, "Ai castigat!\nScorul tau este: %.2f", scor);

			winMessage = createButton(winMessage.text.getString() + buffer, width / 2 - 250 / 2,
				height / 2 - 100 / 2, 250, 100);
			winMessage.text.setFont(font);
			wonMessage = true;
		}

		if (wonMessage) {
			window.draw(winMessage.button);
			window.draw(winMessage.text);
		}

		window.display();

		if (!running && !gameOver) {
			int opt = confirmToSave();
			if (opt == 0) {
				break;
			}
			else if (opt == 1) {
				minesweper.salveaza_joc();
				break;
			}
			else {
				running = true;
				firstClick = true;
			}
		}
	}
	minesweper.endGame();
}

cellBox newCell(float x, float y, int cellSize, Celula cell, int i, int j, int st) {
	cellBox button;


	button.x = i;
	button.y = j;

	button.button.setSize(Vector2f((float)cellSize, (float)cellSize));
	button.button.setPosition(x, y);
	button.button.setOutlineThickness(1);
	button.button.setOutlineColor(buttonBorderColor);
	if (st == Deschisa) {
		button.button.setTexture(&openCellTexture);
		button.showText = true;
	}
	else if (st == Marcata) {
		button.button.setTexture(&flagTexture);
	}
	else {
		button.button.setTexture(&cellTexture);
	}

	button.text.setFont(font);
	if (cell.getNrVecini() != 0) {
		button.text.setString(to_string(cell.getNrVecini()));
	}

	button.text.setCharacterSize(FONT_SIZE);

	switch (cell.getNrVecini())
	{
	case 1:
		button.text.setFillColor(n1);
		break;
	case 2:
		button.text.setFillColor(n2);
		break;
	case 3:
		button.text.setFillColor(n3);
		break;
	case 4:
		button.text.setFillColor(n4);
		break;
	case 5:
		button.text.setFillColor(n5);
		break;
	case 6:
		button.text.setFillColor(n6);
		break;
	case 7:
		button.text.setFillColor(n7);
		break;
	case 8:
		button.text.setFillColor(n8);
		break;
	default:
		break;
	}

	FloatRect textRect = button.text.getLocalBounds();
	button.text.setOrigin(textRect.left + textRect.width / 2.0f, textRect.top + textRect.height / 2.0f);
	button.text.setPosition(x + cellSize / 2.0f, y + cellSize / 2.0f);

	return button;
}

int confirmToSave() {
	RenderWindow window(VideoMode(300, 200), "Minesweper");
	Text text;
	text.setFont(font);
	text.setString("Doriti sa salvati jocul?");
	text.setCharacterSize(FONT_SIZE);
	text.setFillColor(Color::Black);
	text.setPosition(50, 50);
	Box yesButton = createButton("Da", 75, 100, 50, 30);
	Box noButton = createButton("Nu", 175, 100, 50, 30);

	while (window.isOpen())
	{
		Event event;

		while (window.pollEvent(event))
		{
			bool hover = false;
			if (event.type == Event::Closed) {
				window.close();
				return 2; // Continua jocul
			}

			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0.0f, 0.0f, (float)event.size.width, (float)event.size.height)));
			}

			if (isCursorOverShape(yesButton.button, window)) {
				buttonHover(yesButton, window, true);
				hover = true;
				if (Mouse::isButtonPressed(Mouse::Left)) {
					window.close();
					return 1; // Salveaza joc
				}
			}
			else {
				buttonHover(yesButton, window, false);
			}

			if (isCursorOverShape(noButton.button, window)) {
				buttonHover(noButton, window, true);
				hover = true;
				if (Mouse::isButtonPressed(Mouse::Left)) {
					window.close();
					return 0; // Nu salva joc
				}
			}
			else {
				buttonHover(noButton, window, false);
			}
			if (hover) {
				window.setMouseCursor(handCursor);
			}
			else {
				window.setMouseCursor(arrowCursor);
			}
		}

		window.clear(Color::White);
		window.draw(text);
		window.draw(yesButton.button);
		window.draw(yesButton.text);
		window.draw(noButton.button);
		window.draw(noButton.text);
		window.display();
	}
	return 1;
}

Text createText(const string& text, int x, int y, int size) {
	Text t;
	t.setFont(font);
	t.setString(text);
	t.setCharacterSize(size);
	t.setFillColor(Color::Black);
	t.setPosition((float)x, (float)y);
	return t;
}

void scoresWindow(Joc& minesweper) {
	const int width = 850;
	const int height = 600;
	RenderWindow scoresWindow(VideoMode(width, height), "Clasament");
	Font font;
	try {
		if (!font.loadFromFile(fontPath)) {
			throw exception("Error loading font file!");
			return;
		}
	}
	catch (const exception& e) {
		ErrorMessageBox(e.what());
		return;
	}
	

	Text scoresTitle = createText("Clasament", 350, 10, FONT_SIZE + 3);
	vector<Score> scores = parseScores(minesweper.getScoruri());
	sort(scores.begin(), scores.end(), compareScores);

	int startY = 50;
	int offsetY = FONT_SIZE + 5;
	int columnWidth = 140;

	vector<Text> scoreTexts;

	vector<string> headers = { "Date", "Difficulty", "Rows x Cols", "Time", "Mines", "Score" };
	int headerX = 10;

	for (const auto& header : headers) {
		Text headerText = createText(header, headerX + (columnWidth / 2), startY, FONT_SIZE - 2);
		headerText.setOrigin(headerText.getLocalBounds().width / 2, 0);
		scoreTexts.push_back(headerText);
		headerX += columnWidth;
	}

	startY += offsetY;

	for (const auto& score : scores) {
		string scoreStr = to_string(score.score);
		scoreStr = scoreStr.substr(0, scoreStr.find(".") + 3);
		ostringstream rowStream;
		rowStream << setw(0) << score.date << " " << score.difficulty << " " << score.rows 
			<< "x" << score.cols << " " << score.time << " " << score.mines << " " << scoreStr;
		istringstream row(rowStream.str());

		int columnX = 10;
		string cell;
		while (row >> cell) {
			Text cellText = createText(cell, columnX + (columnWidth / 2), startY, FONT_SIZE - 2);
			cellText.setOrigin(cellText.getLocalBounds().width / 2, 0);
			scoreTexts.push_back(cellText);
			columnX += columnWidth;
		}

		startY += offsetY;
	}

	View view(FloatRect(0.0f, 0.0f, (float)scoresWindow.getSize().x, (float)scoresWindow.getSize().y));
	int totalHeight = startY;

	Sprite  background(backgroundTexture);

	background.setScale(windowWidth / background.getLocalBounds().width, windowHeight /
		background.getLocalBounds().height);

	while (scoresWindow.isOpen()) {
		Event event;
		while (scoresWindow.pollEvent(event)) {
			if (event.type == Event::Closed) {
				scoresWindow.close();
			}
			if (event.type == Event::MouseWheelScrolled) {
				if (event.mouseWheelScroll.delta < 0 && view.getCenter().y + view.getSize().y / 2 < totalHeight) {
					view.move(0, 10);
				}
				else if (event.mouseWheelScroll.delta > 0 && view.getCenter().y - view.getSize().y / 2 > 0) {
					view.move(0, -10);
				}
			}
		}

		scoresWindow.clear(Color::White);
		scoresWindow.setSize(Vector2u(width, height));

		scoresWindow.draw(background);
		scoresWindow.setView(view);
		scoresWindow.draw(scoresTitle);
		for (const auto& text : scoreTexts) {
			scoresWindow.draw(text);
		}
		scoresWindow.display();
	}
}

vector<Score> parseScores(const string& scoresStr) {
	vector<Score> scores;
	istringstream ss(scoresStr);
	string line;

	while (getline(ss, line)) {
		istringstream lineStream(line);
		Score score;
		lineStream >> score.date >> score.difficulty >> score.rows >> score.cols >> 
			score.time >> score.mines >> score.score;
		scores.push_back(score);
	}

	return scores;
}

bool compareScores(const Score& a, const Score& b) {
	return a.score > b.score;
}

void ErrorMessageBox(const string& message) {
	MessageBoxA(NULL, message.c_str(), "Error", MB_OK | MB_ICONERROR);
	return;
}