/*
    nombre: Librería de Interruptor (Implementación)
    autor: David Emmanuel Santana Romero
    fecha: 8 de noviembre de 2023
    versión: 1.0
*/

#include "switch.h"

Switch::Switch(bool &binded, float position_x, float position_y, sf::Font &font, wstring title, sf::Color color_shape, sf::Color color_shape_active, sf::Color color_handle, sf::Color color_text, bool * hide) : Button (binded, position_x, position_y, font, title, 60, 15, color_shape, color_shape_active, color_text, hide) {
    this->alreadyDispatched = false;
    this->handle = sf::RectangleShape (sf::Vector2f(10, 25));
    this->handle.setPosition(shape.getPosition().x, shape.getPosition().y + ((shape.getSize().y - handle.getLocalBounds().height)/2));
    this->handle.setFillColor(color_handle);
    this->title.setCharacterSize(18);
    this->title.setStyle(sf::Text::Bold);
    this->title.setPosition(shape.getPosition().x + ((shape.getSize().x - this->title.getLocalBounds().width)/2), shape.getPosition().y - (this->title.getCharacterSize()*2));
    this->hide = hide;
}

bool Switch::updateState () {
    if (isClicking && !alreadyDispatched) {
        *x = !(*x);
        if (*x) {
            handle.setPosition(shape.getPosition().x + shape.getSize().x - handle.getSize().x, handle.getPosition().y);
        }
        else {
            handle.setPosition(shape.getPosition().x, handle.getPosition().y);
        }
        shape.setFillColor(colors[*x]);
        return true;
    }
    return false;
}

void Switch::checkClicking (sf::Vector2i mouse) {
    if (hide != nullptr && *hide) return;
    if (!isClicking) {
        alreadyDispatched = false;
        Button::checkClicking(mouse);
        if (!isClicking && handle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse))) {
            isClicking = true;
        }
    }
    else alreadyDispatched = true;
}

void Switch::draw (sf::RenderWindow &window) {
    if (hide != nullptr && *hide) return;
    Button::draw(window);
    window.draw(handle);
}