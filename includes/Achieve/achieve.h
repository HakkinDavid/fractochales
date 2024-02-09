/*
    nombre: Librería de Logros (Encabezado)
    autor: Mauricio Alcántar Dueñas
    fecha: 11 de diciembre de 2023
    versión: 1.0
*/
#include <iostream>
#include <string>
#include <iomanip> //dunno what this is
#include <sstream>
#include <chrono>
#include "../SFML/Graphics.hpp"
#include <functional> //dunno what this is
using namespace std;

#ifndef ACHIEVE_H
#define ACHIEVE_H

class Achieve {
    private:
        int * drawPile;
        int prevPile, drawOffset, time;
        sf::RectangleShape shape;
        sf::Text title; 
        sf::Sprite thumbnail;
        std::function<bool()> condition;
        std::chrono::system_clock::time_point timestamp;
    public:
        Achieve (int &drawPile, float position_x, float position_y, sf::Texture &thumbnail_texture, sf::Font &font, wstring title = wstring(), float size_x = 250.f, float size_y = 8.f, int time = 5000, std::function<bool()> condition = []() { return true; });
        void draw (sf::RenderWindow &window);
        int getTime (void);
};

#endif