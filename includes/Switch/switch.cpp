/*
    nombre: Librería de Interruptor (Implementación)
    autor: David Emmanuel Santana Romero
    fecha: 8 de noviembre de 2023
    versión: 1.0
*/

#include "switch.h"

Switch::Switch(bool &binded, float position_x, float position_y, sf::Font &font, wstring title_while_off, wstring title_while_on, float size_x, float size_y, sf::Color color_shape, sf::Color color_shape_active, sf::Color color_text, bool * hide, std::function<bool()> isEnabled, std::function<void()> onUpdate) : 
        Button (binded, position_x, position_y, font, binded ? title_while_on : title_while_off, size_x, size_y, color_shape, color_shape_active, color_text, hide, isEnabled, onUpdate) {
    this->alreadyDispatched = false;
    this->hide = hide;
    this->title[0] = title_while_off;
    this->title[1] = title_while_on;
}

bool Switch::updateState () {
    if (isEnabled() && isClicking && !alreadyDispatched) {
        *x = !(*x);
        shape.setFillColor(colors[*x]);
        changeTitle(title[*x]);
        onUpdate();
        return true;
    }
    return false;
}

bool Switch::checkClicking (sf::Vector2i mouse, int index) {
    if (hide != nullptr && *hide) return false;
    if (!isClicking) {
        alreadyDispatched = false;
        Button::checkClicking(mouse, index);
    }
    else alreadyDispatched = true;
    return isClicking;
}
