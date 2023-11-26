/*
    nombre: Librería de Deslizador (Implementación)
    autor: David Emmanuel Santana Romero, con scrapping del código de Diego Emilio Putricio Casta Valle
    fecha: 2 de noviembre de 2023
    versión: 1.0
*/

#include "slider.h"

Slider::Slider
(
        float &binded, // variable externa a modificar
        float lowerBound, // valor mínimo
        float upperBound, // valor máximo
        float position_x, // ubicación en la ventana (x)
        float position_y, // ubicación en la ventana (y)
        int omit, // omitir [0] nada, [1] título, [2] porcentaje, [3] título y porcentaje
        sf::Font &font, // objeto fuente de letra (por referencia)
        wstring title, // título del deslizador
        bool swapToUnits, // mostrar las unidades en lugar del porcentaje
        sf::Color color_shape, // color de la barra deslizable
        sf::Color color_handle, // color de la manija deslizable
        bool * hide
) {
    this->x = &binded;
    this->lowerBound = lowerBound;
    this->upperBound = upperBound;
    this->shape = sf::RectangleShape (sf::Vector2f(250, 15));
    this->shape.setPosition (position_x, position_y);
    this->omit = omit;
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
    this->title.setPosition(shape.getPosition().x, shape.getPosition().y - (this->title.getCharacterSize()*2));
    this->handle_percent.setPosition(handle.getPosition().x - (handle.getSize().x - handle_percent.getLocalBounds().width)/2,shape.getPosition().y + (this->handle_percent.getCharacterSize()*2.15));
    this->isDragging = false;
    this->swapToUnits = swapToUnits;
    this->hide = hide;
}

void Slider::checkDragging (sf::Vector2i mouse) {
    if (hide != nullptr && *hide) return;
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
            handle.setPosition(shape.getPosition().x, handle.getPosition().y);
        }
        // fuera del límite (sobre 100; muy a la derecha)
        else if (mouse.x + (handle.getLocalBounds().width / 2) > shape.getPosition().x + shape.getSize().x) {
            handle.setPosition(shape.getPosition().x + shape.getSize().x - handle.getSize().x, handle.getPosition().y);
        }
        // está adentro (killing you for that joke, david)
        else {
            handle.setPosition(mouse.x - handle.getSize().x/2, handle.getPosition().y);
        }
        percent = ((handle.getPosition().x - shape.getPosition().x)/(shape.getSize().x - handle.getLocalBounds().width))*100.0f;
        *x = ((upperBound - lowerBound)/100) * percent + lowerBound;
    }
    else {
        percent = (*x - lowerBound)/(upperBound - lowerBound)*100.0f;
        if (percent != oldPercent) {
            int x_based_pos = shape.getPosition().x + ((shape.getSize().x - handle.getLocalBounds().width)/100) * ((*x - lowerBound)/((upperBound - lowerBound)/100));
            // fuera del límite (bajo 0; muy a la izquierda)
            if (x_based_pos < shape.getPosition().x) {
                handle.setPosition(shape.getPosition().x, handle.getPosition().y);
            }
            // fuera del límite (sobre 100; muy a la derecha)
            else if (x_based_pos > shape.getPosition().x + shape.getSize().x) {
                handle.setPosition(shape.getPosition().x + shape.getSize().x - handle.getSize().x, handle.getPosition().y);
            }
            // está adentro (shut UP)
            else {
                handle.setPosition(x_based_pos, handle.getPosition().y);
            }
        }
    }
    handle_percent.setPosition(handle.getPosition().x + (handle.getSize().x - handle_percent.getLocalBounds().width)/2,shape.getPosition().y + (this->handle_percent.getCharacterSize()*2.15));
    handle_text_percentage.str(string());
    if (swapToUnits) handle_text_percentage << fixed << setprecision(0) << *x;
    else handle_text_percentage << fixed << setprecision(2) << percent << "%";
    handle_percent.setString(handle_text_percentage.str());
    if (percent != oldPercent) return true;
    else return false;
}

void Slider::draw (sf::RenderWindow &window) {
    if (hide != nullptr && *hide) return;
    window.draw(shape);
    window.draw(handle);
    if (omit != 1 && omit != 3) window.draw(title);
    if (omit != 2 && omit != 3) window.draw(handle_percent);
}

void Slider::changeUpperBound (float newUpperBound) {
    upperBound = newUpperBound;
}