/*
    nombre: Librería de Botón (Implementación)
    autor: David Emmanuel Santana Romero, con scrapping del código de Diego Emilio Putricio Casta Valle
    fecha: 2 de noviembre de 2023
    versión: 1.0
*/

#include "button.h"

Button::Button(bool &binded, float position_x, float position_y, sf::Font &font, wstring title, float size_x, float size_y, sf::Color color_shape, sf::Color color_shape_clicked, sf::Color color_text) {
    this->x = &binded;
    this->colors[0] = color_shape;
    this->colors[1] = color_shape_clicked;
    this->shape = sf::RectangleShape (sf::Vector2f(size_x, size_y));
    this->shape.setPosition (position_x, position_y);
    this->shape.setFillColor (colors[0]);
    this->title.setFont(font);
    this->title.setString(title);
    this->title.setCharacterSize(18);
    this->title.setFillColor(color_text);
    this->title.setStyle(sf::Text::Bold);
    this->title.setPosition(shape.getPosition().x + ((shape.getSize().x - this->title.getLocalBounds().width)/2), shape.getPosition().y + ((shape.getSize().y - this->title.getLocalBounds().height)/2));
    this->isClicking = false;
}

void Button::checkClicking (sf::Vector2i mouse) {
    if (shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse))) {
        isClicking = true;
    }
}

void Button::setIsClicking (bool v) {
    isClicking = v;
}

void Button::updateState () {
    *x = isClicking;
    shape.setFillColor(colors[isClicking]);
}

void Button::draw(sf::RenderWindow &window) {
    window.draw(shape);
    window.draw(title);
}