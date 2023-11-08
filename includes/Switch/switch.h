/*
    nombre: Librería de Interruptor (Encabezado)
    autor: David Emmanuel Santana Romero
    fecha: 8 de noviembre de 2023
    versión: 1.0
*/
#include <iostream>
#include <string>
#include "../SFML/Graphics.hpp"
#include "../Button/button.h"
using namespace std;

#ifndef SWITCH_H
#define SWITCH_H

class Switch : public Button {
    private:
        bool alreadyDispatched; // ¿ya había recibido este evento?
        wstring rawTitle;
    public:
        Switch (bool &binded, float position_x, float position_y, sf::Font &font, wstring title = wstring(), float size_x = 100, float size_y = 50, sf::Color color_shape = sf::Color::Black, sf::Color color_shape_clicked = sf::Color::Green, sf::Color color_text = sf::Color::White);
        bool updateState ();
        void checkClicking (sf::Vector2i mouse);
        wstring formatTitle(wstring value);
};

#endif