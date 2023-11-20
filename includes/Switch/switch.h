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
        sf::RectangleShape handle;
        bool * hide;
    public:
        Switch (bool &binded, float position_x, float position_y, sf::Font &font, wstring title = wstring(), sf::Color color_shape = sf::Color(84,0,14), sf::Color color_shape_active = sf::Color(0,84,46), sf::Color color_handle = sf::Color::White, sf::Color color_text = sf::Color::White, bool * hide = nullptr);
        bool updateState ();
        void checkClicking (sf::Vector2i mouse);
        void draw (sf::RenderWindow &window);
};

#endif