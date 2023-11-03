/*
    nombre: Librería de Deslizador (Implementación)
    autor: David Emmanuel Santana Romero, con scrapping del código de Diego Emilio Putricio Casta Valle
    fecha: 2 de noviembre de 2023
    versión: 1.0
*/

#include "slider.h"

Slider::Slider(float &binded, float lowerBound, float upperBound, float position_x, float position_y, sf::Font &font, wstring title, bool swapToUnits, sf::Color color_shape, sf::Color color_handle) {
    this->x = &binded;
    this->lowerBound = lowerBound;
    this->upperBound = upperBound;
    this->shape = sf::RectangleShape (sf::Vector2f(250, 15));
    this->shape.setPosition (position_x, position_y);
    this->shape.setFillColor (color_shape);
    this->handle = sf::RectangleShape (sf::Vector2f(20, 35));
    this->handle.setPosition(shape.getPosition().x + ((shape.getSize().x - handle.getLocalBounds().width)/100) * ((*(this->x) - lowerBound)/((upperBound - lowerBound)/100)), shape.getPosition().y + ((shape.getSize().y - handle.getLocalBounds().height)/2));
    this->handle.setFillColor(color_handle);
    this->title.setFont(font);
    this->handle_percent.setFont(font);
    this->title.setString(title);
    this->handle_percent.setString(handle_text_percentage.str());
    this->title.setCharacterSize(18);
    this->handle_percent.setCharacterSize(16);
    this->title.setFillColor(sf::Color::White);
    this->handle_percent.setFillColor(sf::Color::White);
    this->title.setStyle(sf::Text::Bold);
    this->title.setPosition(shape.getPosition().x/2+35, shape.getPosition().y - (this->title.getCharacterSize()*2));
    this->handle_percent.setPosition(handle.getPosition().x-2,shape.getPosition().y + (this->handle_percent.getCharacterSize()*2.15));
    this->isDragging = false;
    this->swapToUnits = swapToUnits;
}

void Slider::checkDragging (sf::Vector2i mouse) {
    if (handle.getGlobalBounds().contains(static_cast<sf::Vector2f>(mouse))) {
        isDragging = true;
    }
}

void Slider::setIsDragging (bool v) {
    isDragging = v;
}

bool Slider::updatePercentage (sf::Vector2i mouse) {
    float oldPercent = percent;
    if (isDragging) {
        // fuera del límite (bajo 0; muy a la izquierda)
        if (mouse.x - (handle.getLocalBounds().width / 2) < shape.getPosition().x) {
            handle.setPosition(shape.getPosition().x,handle.getPosition().y);
        }
        // fuera del límite (sobre 100; muy a la derecha)
        else if (mouse.x + (handle.getLocalBounds().width / 2) > shape.getPosition().x + shape.getSize().x) {
            handle.setPosition(shape.getPosition().x + shape.getSize().x - handle.getSize().x, handle.getPosition().y);
        }
        // está adentro (nyaaaaa~)
        else {
            handle.setPosition(mouse.x - handle.getSize().x/2, handle.getPosition().y);
        }
    }
    percent = ((handle.getPosition().x - shape.getPosition().x)/(shape.getSize().x - handle.getLocalBounds().width))*100.0f;
    *x = ((upperBound - lowerBound)/100) * percent + lowerBound;
    handle_text_percentage.str(string());
    if (swapToUnits) handle_text_percentage << fixed << setprecision(2) << *x;
    else handle_text_percentage << fixed << setprecision(2) << percent << "%";
    handle_percent.setString(handle_text_percentage.str());
    if (percent != oldPercent) return true;
    else return false;
}

void Slider::draw(sf::RenderWindow &window) {
    window.draw(shape);
    window.draw(handle);
    window.draw(title);
    window.draw(handle_percent);
}