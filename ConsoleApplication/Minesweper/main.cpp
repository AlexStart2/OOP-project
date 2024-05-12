#include <SFML/Graphics.hpp>
#include <iostream>
#include <string>
#include <ConsoleApplication/Joc.h>
#include <ConsoleApplication/Nivel.h>

using namespace sf;

const bool DEBUG = true;

const char* fontPath = "TellMeAJoke.ttf";
const int windowWidth = 1000;
const int windowHeight = 700;
const Color buttonColor = Color(192, 192, 192, 255);
const Color buttonHoverColor = Color(128, 128, 128, 255);
const Color buttonClickColor = Color(64, 64, 64, 255);
const Color buttonTextColor = Color::Black;
const Color buttonHoverTextColor = Color::White;
const Color buttonClickTextColor = Color::White;
const Color buttonBorderColor = Color::Black;
const int BUTTON_BORDER_THICKNESS = 2;
const float WIDTH = 170;
const float HEIGHT = 50;
const float xButtonText = 50;
const float yButtonText = 10;
const int FONT_SIZE = 28;
const int menuPosX = 50;
const int menuPosY = 50;
const int SPACE = 10;


bool mainMenu = true;
bool chooseLevel = false;

struct Button {
	RectangleShape button;
	Text text;
	Font font;
};

Button createButton(const std::string& text, float x, float y, float xText = xButtonText,
	float yText = yButtonText, float width = WIDTH, float height = HEIGHT);
void buttonHover(Button& button, RenderWindow& window ,bool hover);
void buttonClick(Button& button);
bool isCursorOverShape(const RectangleShape& shape, const RenderWindow& window);
void EventMenu(RenderWindow& window, Event& event, Button& newGameButton, 
	Button& loadGameButton, Button& exitButton);
void drawMenu(RenderWindow& window, Button& newGameButton,
	Button& loadGameButton, Button& exitButton);
void drawLevelMenu(RenderWindow& window, std::vector<Button>& levelButtons);

int main()
{
    RenderWindow window(VideoMode(windowWidth, windowHeight), "Minesweper");
	Joc minesweper();

	int i = 0;
	Button newGameButton = createButton("Joc nou", menuPosX, (menuPosY + SPACE) * ++i);
	Button loadGameButton = createButton("Incarca joc", menuPosX, (menuPosY + SPACE) * ++i);
	Button exitButton = createButton("Iesire", menuPosX, (menuPosY + SPACE) * ++i);

	const std::vector<Nivel> levels = {
		Incepator9, Incepator16, Incepator30,
		Mediu9, Mediu16, Mediu30,
		Avansat9, Avansat16, Avansat30
	};

	std::vector<Button> levelButtons;
	float buttonY = 50.0f;
	for (const auto& level : levels) {
		Button button = createButton(level.nume, menuPosX, buttonY);
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
			if (mainMenu) {
				EventMenu(window, event, newGameButton, loadGameButton, exitButton);
			}
			else if (chooseLevel) {
				for (auto& button : levelButtons) {
					if (isCursorOverShape(button.button, window)) {
						buttonHover(button, window, true);
						if (Mouse::isButtonPressed(Mouse::Left)) {
							buttonClick(button);
							if (DEBUG) {
								std::cout << "Level button clicked!" << std::endl;
							}
						}
					}
					else {
						buttonHover(button, window, false);
					}
				}
			}
        }

		window.clear(Color::White);
		window.setSize(Vector2u(1000, 700));

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

Button createButton(const std::string& text, float x, float y, float xText,
	float yText, float width, float height)
{
	Button button;

	button.button.setSize(Vector2f(width, height));
	button.button.setPosition(x, y);
	button.button.setFillColor(buttonColor);

	if (!button.font.loadFromFile(fontPath)) {
		std::cerr << "Error loading font file!" << std::endl;
	}

	button.text.setFont(button.font);
	button.text.setString(text);
	button.text.setCharacterSize(FONT_SIZE);
	button.text.setFillColor(buttonTextColor);
	button.text.setPosition(x + xText, y + yText);

	button.button.setOutlineThickness(BUTTON_BORDER_THICKNESS);
	button.button.setOutlineColor(buttonBorderColor);

	return button;
}


void buttonHover(Button& button, RenderWindow& window, bool hover)
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

void buttonClick(Button& button)
{
	button.button.setFillColor(buttonClickColor);
	button.text.setFillColor(buttonClickTextColor);
}


void EventMenu(RenderWindow& window, Event& event, Button& newGameButton,
	Button& loadGameButton, Button& exitButton)
{
	Cursor handCursor;
	handCursor.loadFromSystem(Cursor::Hand);
	Cursor arrowCursor;
	arrowCursor.loadFromSystem(Cursor::Arrow);

	if (event.type == Event::MouseMoved || event.type == Event::MouseButtonPressed)
	{
		bool hover = false;
		if (isCursorOverShape(newGameButton.button, window)) {
			buttonHover(newGameButton, window, true);
			if (Mouse::isButtonPressed(Mouse::Left)) {
				buttonClick(newGameButton);
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
				buttonClick(loadGameButton);
				if (DEBUG) {
					std::cout << "Load Game button clicked!" << std::endl;
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
				buttonClick(exitButton);
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

void drawMenu(RenderWindow& window, Button& newGameButton,
	Button& loadGameButton, Button& exitButton)
{
	window.draw(newGameButton.button);
	window.draw(newGameButton.text);

	window.draw(loadGameButton.button);
	window.draw(loadGameButton.text);

	window.draw(exitButton.button);
	window.draw(exitButton.text);
}

void drawLevelMenu(RenderWindow& window, std::vector<Button>& levelButtons)
{
	for (Button button : levelButtons) {
		button.font.loadFromFile(fontPath);
		button.text.setFont(button.font);
		window.draw(button.button);
		window.draw(button.text);
	}
}