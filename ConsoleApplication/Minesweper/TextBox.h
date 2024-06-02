#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace sf;
using namespace std;

class TextBox
{
private:
    RectangleShape box;
    Text text;
    string input;
    bool hasFocus;

public:
    TextBox(float x, float y, float width, float height, const Font& font, unsigned int fontSize);
    void handleEvent(Event event);
    void draw(RenderWindow& window);

    string getText() const {
        return input;
    }

    void setText(const string& str) {
        input = str;
        text.setString(input);
    }
};

