#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include "Joc.h"
#include "Nivel.h"
#include <vector>
#include "Grila.h"
#include <string>


using namespace sf;

const bool DEBUG = true;

const char* fontPath = "TellMeAJoke.ttf";

const int windowWidth = 1000;
const int windowHeight = 700;

const Color buttonColor = Color(192,192,192,255);
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

const float WIDTH = 170;
const float HEIGHT = 50;
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
};

Box createButton(const std::string& text, int x, int y, float width = WIDTH, float height = HEIGHT);
void buttonHover(Box& button, RenderWindow& window ,bool hover);
bool isCursorOverShape(const RectangleShape& shape, const RenderWindow& window);
void EventMenu(RenderWindow& window, Event& event, Box& newGameButton, 
	Box& loadGameButton, Box& exitButton, Joc& minesweper);
void drawMenu(RenderWindow& window, Box& newGameButton,
	Box& loadGameButton, Box& exitButton);
void drawLevelMenu(RenderWindow& window, std::vector<Box>& levelButtons);
void startGame(RenderWindow& window, Joc& minesweper, const Nivel level);
cellBox newCell(int x, int y, int cellSize, Celula cell, int i, int j);
int confirmToSave();

int main()
{ 
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Minesweper");
	window.setFramerateLimit(60);
	Joc minesweper;

	if (!font.loadFromFile(fontPath)) {
		std::cerr << "Error loading font file!" << std::endl;
	}
	handCursor.loadFromSystem(Cursor::Hand);
	arrowCursor.loadFromSystem(Cursor::Arrow);

	int i = 0;
	Box newGameButton = createButton("Joc nou", menuPosX, (menuPosY + SPACE) * ++i);
	Box loadGameButton = createButton("Incarca joc", menuPosX, (menuPosY + SPACE) * ++i);
	Box exitButton = createButton("Iesire", menuPosX, (menuPosY + SPACE) * ++i);

	const std::vector<Nivel> levels = {
		Incepator9, Incepator16, Incepator30,
		Mediu9, Mediu16, Mediu30,
		Avansat9, Avansat16, Avansat30
	};

	std::vector<Box> levelButtons;
	float buttonY = 50.0f;
	for (const auto& level : levels) {
		Box button = createButton(level.nume, menuPosX, buttonY);
		levelButtons.push_back(button);
		buttonY += HEIGHT + 10.0f;
	}

    while (window.isOpen())
    {
        Event event;
        while (window.pollEvent(event))
        {
            if (event.type == Event::Closed)
                window.close();

			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0, 0, event.size.width, event.size.height)));
			}

			if (mainMenu) {
				EventMenu(window, event, newGameButton, loadGameButton, exitButton, minesweper);
			}
			else if (chooseLevel) {
				for (auto& button : levelButtons) {
					if (isCursorOverShape(button.button, window)) {
						buttonHover(button, window, true);
						if (Mouse::isButtonPressed(Mouse::Left)) {
							if (DEBUG) {
								std::cout << button.text.getString().toAnsiString() << " button clicked!" << std::endl;
							}

							std::string levelName = button.text.getString().toAnsiString();
							Nivel level = Incepator16;
							for (const auto& l : levels) {
								if (l.nume == levelName) {
									level = l;
									break;
								}
							}

							startGame(window, minesweper, level);
						}
					}
					else {
						buttonHover(button, window, false);
					}
				}
			}
        }

		window.clear(Color::White);
		window.setSize(Vector2u(windowWidth, windowHeight));

		if (mainMenu) {
			drawMenu(window, newGameButton, loadGameButton, exitButton);
		}
		else if (chooseLevel) {
			drawLevelMenu(window, levelButtons);
		}
		
        window.display();
    }

    return 0;
}

Box createButton(const std::string& text, int x, int y, float width, float height)
{
	Box button;

	button.button.setSize(Vector2f(width, height));
	button.button.setPosition(x, y);
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
	Box& loadGameButton, Box& exitButton, Joc& minesweper)
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
					std::cout << "New Game button clicked!" << std::endl;
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
					std::cout << "Load Game button clicked!" << std::endl;
				}
				minesweper.incarca_joc();
				if (minesweper.getGrila().getNrLinii() != 0) {
					mainMenu = false;
					chooseLevel = false;
					startGame(window, minesweper, minesweper.getNivel());
				}
			}
			hover = true;
		}
		else {
			buttonHover(loadGameButton, window, false);
		}

		if (isCursorOverShape(exitButton.button, window)) {
			buttonHover(exitButton, window, true);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				window.close();
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

void drawMenu(RenderWindow& window, Box& newGameButton,
	Box& loadGameButton, Box& exitButton)
{
	window.draw(newGameButton.button);
	window.draw(newGameButton.text);

	window.draw(loadGameButton.button);
	window.draw(loadGameButton.text);

	window.draw(exitButton.button);
	window.draw(exitButton.text);
}

void drawLevelMenu(RenderWindow& window, std::vector<Box>& levelButtons)
{
	for (Box button : levelButtons) {
		button.text.setFont(font);
		window.draw(button.button);
		window.draw(button.text);
	}
}

void startGame(RenderWindow& window, Joc& minesweper, const Nivel level)
{
	if (!mineTexture.loadFromFile("Texture/openCellMine.bmp")) {
		std::cerr << "Error loading mine image!" << std::endl;
	}
	if (!flagTexture.loadFromFile("Texture/flag (1).png")) {
		std::cerr << "Error loading flag image!" << std::endl;
	}
	if (!BOOMTexture.loadFromFile("Texture/boomMine.bmp")) {
		std::cerr << "Error loading blast image!" << std::endl;
	}
	if (!cellTexture.loadFromFile("Texture/cell.bmp")) {
		std::cerr << "Error loading cell image!" << std::endl;
	}
	if (!openCellTexture.loadFromFile("Texture/openCell.bmp")) {
		std::cerr << "Error loading open cell image!" << std::endl;
	}

	Grila& grila = minesweper.incepe_joc(level);

	int width = grila.getNrColoane() * cellSize + tableMargin;
	int height = grila.getNrLinii() * cellSize + marginTop + tableMargin / 2;

	vector<cellBox> cells;

	Box mineCounter = createButton(std::to_string(grila.getNumarMine()) , 40, 10);
	FloatRect textRect = mineCounter.text.getLocalBounds();
	mineCounter.button.setSize(Vector2f(textRect.width + 30, textRect.height + 10));
	mineCounter.text.setPosition(mineCounter.button.getPosition().x + mineCounter.button.getSize().x / 2.0f,
		mineCounter.button.getPosition().y + mineCounter.button.getSize().y / 2.0f);

	Box timeCounter = createButton("0", width - 95, 10, 60, textRect.height + 10);
	minesweper.setStartTime(chrono::high_resolution_clock::now());

	Box returnButton = createButton("<", 5, 5, 20, 20);
	
	for (int i = 0; i < grila.getNrLinii(); i++) {
		for (int j = 0; j < grila.getNrColoane(); j++) {
			string text = to_string(grila.getCell(i,j).getNrVecini());
			cellBox cell = newCell(j * (cellSize) + tableMargin/2 , i * (cellSize 
				 ) + marginTop, cellSize, grila.getCell(i,j),i , j);
			cells.push_back(cell);
		}
	}
	
	window.setMouseCursor(arrowCursor);

	bool running = true;
	bool gameOver = false;
	int time = 0;
	bool firstClick = true;

	while (running)
	{
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
					}
					else if (opt == 1) {
						minesweper.salveaza_joc();
						window.close();
					}
					else {
						firstClick = true;
						break;
					}
				}
				window.close();
			}
				
			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0, 0, event.size.width, event.size.height)));
			}

			if (isCursorOverShape(returnButton.button, window)) {
				buttonHover(returnButton, window, true);
				window.setMouseCursor(handCursor);
				if (Mouse::isButtonPressed(Mouse::Left)) {
					mainMenu = true;
					chooseLevel = false;
					running = false;
					break;
				}
			}
			else {
				buttonHover(returnButton, window, false);
				window.setMouseCursor(arrowCursor);
			}

			if (!gameOver) {
				if (Mouse::isButtonPressed(Mouse::Right)) {
					for (int i = 0; i < cells.size(); i++) {
						if (isCursorOverShape(cells[i].button, window)) {
							if(grila.getNrMineMarcate() != grila.getNumarMine() && 
								grila.getCell(cells[i].x, cells[i].y).getStare() == Inchisa) {
								grila.marcheaza_celula(cells[i].x, cells[i].y);
								cells[i].button.setTexture(&flagTexture);
								mineCounter.text.setString(to_string(grila.getNumarMine() - grila.getNrMineMarcate()));
								break;
							}							
							if (grila.getCell(cells[i].x, cells[i].y).getStare() == Marcata) {
								grila.marcheaza_celula(cells[i].x, cells[i].y);
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

							if (!grila.deschide_celula(cells[i].x, cells[i].y)) {
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
								break;
							}
						}
					}
				}
			}
			
		}
		window.clear(Color::White);

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
				(chrono::high_resolution_clock::now() - minesweper.getStartTime()).count() + minesweper.getTimp();;
		}
		timeCounter.text.setString(std::to_string(time));


		window.draw(returnButton.button);
		window.draw(returnButton.text);

		window.draw(timeCounter.button);
		window.draw(timeCounter.text);

		window.draw(mineCounter.button);
		window.draw(mineCounter.text);
		
		window.display();

		if (!running && !gameOver) {
			if (confirmToSave()) {
				minesweper.salveaza_joc();
			}
		}
	}

	if (gameOver) {
		minesweper.salveaza_scor();
	}

	minesweper.endGame();
}

cellBox newCell(int x, int y, int cellSize, Celula cell, int i, int j) {
	cellBox button;
	
	button.x = i;
	button.y = j;

	button.button.setSize(Vector2f(cellSize, cellSize));
	button.button.setPosition(x, y);
	button.button.setTexture(&cellTexture);
	button.button.setOutlineThickness(1);
	button.button.setOutlineColor(buttonBorderColor);

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
				return 2;
			}

			if (event.type == Event::Resized) {
				window.setView(View(FloatRect(0, 0, event.size.width, event.size.height)));
			}

			if (isCursorOverShape(yesButton.button, window)) {
				buttonHover(yesButton, window, true);
				hover = true;
				if (Mouse::isButtonPressed(Mouse::Left)) {
					window.close();
					return 1;
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
					return 0;
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
}