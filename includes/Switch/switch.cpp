/*
    nombre: Librería de Interruptor (Implementación)
    autor: David Emmanuel Santana Romero
    fecha: 8 de noviembre de 2023
    versión: 1.0
*/

#include "switch.h"

Switch::Switch(bool &binded, float position_x, float position_y, sf::Font &font, wstring title, float size_x, float size_y, sf::Color color_shape, sf::Color color_shape_active, sf::Color color_text, bool * hide) : 
        Button (binded, position_x, position_y, font, title, size_x, size_y, color_shape, color_shape_active, color_text, hide) {
    this->alreadyDispatched = false;
    this->hide = hide;
}

bool Switch::updateState () {
    if (isClicking && !alreadyDispatched) {
        *x = !(*x);
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
    }
    else alreadyDispatched = true;
}
