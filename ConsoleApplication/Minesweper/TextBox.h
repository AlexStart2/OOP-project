#pragma once
#include <string>
#include <SFML/Graphics.hpp>

using namespace sf;

class TextBox
{
private:
    RectangleShape box;
    Text text;
    std::string input;
    bool hasFocus;

public:
    TextBox(float x, float y, float width, float height, const Font& font, unsigned int fontSize);
    void handleEvent(Event event);
    void draw(RenderWindow& window);

    std::string getText() const {
        return input;
    }

    void setText(const std::string& str) {
        input = str;
        text.setString(input);
    }
};

