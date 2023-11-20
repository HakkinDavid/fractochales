/*
    nombre: Librería de Deslizador (Encabezado)
    autor: David Emmanuel Santana Romero, con scrapping del código de Diego Emilio Putricio Casta Valle
    fecha: 2 de noviembre de 2023
    versión: 1.0
*/
#include <iostream>
#include <string>
#include <iomanip>
#include <sstream>
#include "../SFML/Graphics.hpp"
using namespace std;

#ifndef SLIDER_H
#define SLIDER_H

class Slider {
    private:
        float * x;
        float percent, lowerBound, upperBound;
        stringstream handle_text_percentage;
        sf::RectangleShape shape;
        sf::RectangleShape handle;
        sf::Text title; // su posición depende del slider
        sf::Text handle_percent; // se desliza con el deslizador ; muestra cuánto %
        bool isDragging;
        bool swapToUnits;
        int omit;
        bool * hide;
    public:
        Slider (float &binded, float lowerBound, float upperBound, float position_x, float position_y, int omit, sf::Font &font, wstring title = wstring(), bool swapToUnits = false, sf::Color color_shape = sf::Color::White, sf::Color color_handle = sf::Color::Black, bool * hide = nullptr);
        void checkDragging (sf::Vector2i mouse);
        void setIsDragging (bool v);
        bool updatePercentage (sf::Vector2i mouse);
        void draw (sf::RenderWindow &window);
};

#endif