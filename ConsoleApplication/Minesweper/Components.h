#pragma once

#include <SFML/Graphics.hpp>
#include <vector>
#include "Joc.h"
#include "Nivel.h"

Box createButton(const string& text, int x, int y, float width = WIDTH, float height = HEIGHT);
void buttonHover(Box& button, RenderWindow& window, bool hover);
bool isCursorOverShape(const RectangleShape& shape, const RenderWindow& window);
void EventMenu(RenderWindow& window, Event& event, Box& newGameButton,
	Box& loadGameButton, Box& showScoresButton, Box& exitButton, Joc& minesweper);
void drawMenu(RenderWindow& window, Box& newGameButton,
	Box& loadGameButton, Box& showScoresButton, Box& exitButton);
void drawLevelMenu(RenderWindow& window, vector<Box>& levelButtons, Box& returnButton,
	TextBox& rowsTextBox, TextBox& columnsTextBox, TextBox& minesTextBox, Box& startButton);
void startGame(RenderWindow& window, Joc& minesweper, const Nivel level, bool loaded);
cellBox newCell(float x, float y, int cellSize, Celula cell, int i, int j, int st);
int confirmToSave();
void EventLevelMenu(RenderWindow& window, Event& event, Box& returnButton, vector<Box>& levelButtons,
	Joc& minesweper, TextBox& rowsTextBox, TextBox& columnsTextBox, TextBox& minesTextBox, Box& startButton,
	Text& errorMessage);
Text createText(const string& text, int x, int y, int size);
void scoresWindow(Joc& minesweper);
bool compareScores(const Score& a, const Score& b);
vector<Score> parseScores(const string& scoresStr);
void ErrorMessageBox(const string& message);
void loadResources();
void hideConsoleWindow();