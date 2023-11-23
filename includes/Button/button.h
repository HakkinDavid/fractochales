/*
    nombre: Librería de Botón (Encabezado)
    autor: David Emmanuel Santana Romero, con scrapping del código de Diego Emilio Putricio Casta Valle
    fecha: 2 de noviembre de 2023
    versión: 1.0
*/
#include <iostream>
#include <string>
#include "../SFML/Graphics.hpp"
using namespace std;

#ifndef BUTTON_H
#define BUTTON_H

class Button {
    protected:
        bool * x;
        sf::RectangleShape shape;
        sf::Text title; // su posición depende del slider
        bool isClicking;
        sf::Color colors [2];
        bool * hide;
    public:
        Button (bool &binded, float position_x, float position_y, sf::Font &font, wstring title = wstring(), float size_x = 100, float size_y = 50, sf::Color color_shape = sf::Color::Black, sf::Color color_shape_clicked = sf::Color::Green, sf::Color color_text = sf::Color::White, bool *hide = nullptr);
        void checkClicking (sf::Vector2i mouse);
        void setIsClicking (bool v);
        bool updateState ();
        void draw (sf::RenderWindow &window);
        void changePosition (float new_x, float new_y);
        void changeTitle (wstring new_title);
};

#endif