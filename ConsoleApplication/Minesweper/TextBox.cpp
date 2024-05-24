#include "TextBox.h"

TextBox::TextBox(float x, float y, float width, float height, const Font& font, unsigned int fontSize)
    : hasFocus(false)
{
    box.setSize(Vector2f(width, height));
    box.setPosition(x, y);
    box.setFillColor(Color::White);
    box.setOutlineColor(Color::Black);
    box.setOutlineThickness(1);

    text.setFont(font);
    text.setCharacterSize(fontSize);
    text.setFillColor(Color::Black);
    text.setPosition(x + 5, y + 5);
}

void TextBox::handleEvent(Event event) {
    if (event.type == Event::MouseButtonPressed) {
        if (box.getGlobalBounds().contains(event.mouseButton.x, event.mouseButton.y)) {
            hasFocus = true;
        }
        else {
            hasFocus = false;
        }
    }
    else if (event.type == Event::TextEntered && hasFocus) {
        if (event.text.unicode == 8 && !input.empty()) {
            input.pop_back();
        }
        else if (event.text.unicode >= 48 && event.text.unicode <= 57) {
            input += static_cast<char>(event.text.unicode);
        }
        text.setString(input);
    }
}

void TextBox::draw(RenderWindow& window) {
    window.draw(box);
    window.draw(text);
}