#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Joc.h"
#include "Nivel.h"

Box createButton(const std::string& text, int x, int y, float width = WIDTH, float height = HEIGHT);
void buttonHover(Box& button, RenderWindow& window, bool hover);
bool isCursorOverShape(const RectangleShape& shape, const RenderWindow& window);
void EventMenu(RenderWindow& window, Event& event, Box& newGameButton,
	Box& loadGameButton, Box& exitButton, Joc& minesweper);
void drawMenu(RenderWindow& window, Box& newGameButton,
	Box& loadGameButton, Box& exitButton);
void drawLevelMenu(RenderWindow& window, std::vector<Box>& levelButtons, Box& returnButton,
	TextBox& rowsTextBox, TextBox& columnsTextBox, TextBox& minesTextBox, Box& startButton);
void startGame(RenderWindow& window, Joc& minesweper, const Nivel level, bool loaded);
cellBox newCell(int x, int y, int cellSize, Celula cell, int i, int j, int st);
int confirmToSave();
void EventLevelMenu(RenderWindow& window, Event& event, Box& returnButton, vector<Box>& levelButtons,
	Joc& minesweper, TextBox& rowsTextBox, TextBox& columnsTextBox, TextBox& minesTextBox, Box& startButton,
	Text& errorMessage);
Text createText(const std::string& text, int x, int y, int size);