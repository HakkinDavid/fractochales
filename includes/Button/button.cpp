/*
    nombre: Librería de Botón (Implementación)
    autor: David Emmanuel Santana Romero, con scrapping del código de Diego Emilio Putricio Casta Valle
    fecha: 2 de noviembre de 2023
    versión: 1.0
*/

#include "button.h"

Button::Button
(
    bool &binded,
    float position_x,
    float position_y,
    sf::Font &font,
    wstring title,
    float size_x, float size_y,
    sf::Color color_shape,
    sf::Color color_shape_clicked,
    sf::Color color_text,
    bool *hide
) {
    this->x = &binded;
    this->colors[0] = color_shape;
    this->colors[1] = color_shape_clicked;
    this->shape = sf::RectangleShape (sf::Vector2f(size_x, size_y));
    this->shape.setPosition (position_x, position_y);
    this->shape.setFillColor (colors[0]);
    this->title.setFont(font);
    this->title.setString(title);
    this->title.setCharacterSize((size_x < size_y ? shape.getSize().x/2 : shape.getSize().y/2));
    this->title.setFillColor(color_text);
    this->title.setStyle(sf::Text::Bold);
    this->title.setPosition(shape.getPosition().x + ((shape.getSize().x - this->title.getLocalBounds().width)/2), shape.getPosition().y + ((shape.getSize().y - this->title.getLocalBounds().height)/2) - 4);
    this->isClicking = false;
    this->hide = hide;
}

void Button::checkClicking (sf::Vector2i mouse) {
    if (hide != nullptr && *hide) return;
    if (shape.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse))) {
        isClicking = true;
    }
}

void Button::setIsClicking (bool v) {
    isClicking = v;
}

bool Button::updateState () {
    bool hasChanged = false;
    if (*x != isClicking) {
        hasChanged = true;
    }
    *x = isClicking;
    shape.setFillColor(colors[isClicking]);
    return hasChanged;
}

void Button::draw(sf::RenderWindow &window) {
    if (hide != nullptr && *hide) return;
    window.draw(shape);
    window.draw(title);
}

void Button::changePosition(float new_x, float new_y) {
    shape.setPosition (new_x, new_y);
    title.setPosition(shape.getPosition().x + ((shape.getSize().x - title.getLocalBounds().width)/2), shape.getPosition().y + ((shape.getSize().y - title.getLocalBounds().height)/2) - 4);
}

void Button::changeTitle(wstring new_title) {
    title.setString(new_title);
    this->title.setPosition(shape.getPosition().x + ((shape.getSize().x - this->title.getLocalBounds().width)/2), shape.getPosition().y + ((shape.getSize().y - this->title.getLocalBounds().height)/2) - 4);
}

sf::Vector2f Button::getSize() {
    return shape.getSize();
}

sf::Vector2f Button::getPosition() {
    return shape.getPosition();
}