/*
    nombre: Librería de Logros (Implementación)
    autor: Mauricio Alcántar Dueñas
    fecha: 11 de diciembre de 2023
    versión: 1.0
*/

#include "achieve.h"

Achieve::Achieve 
(
    int &drawPile, 
    float position_x,
    float position_y,
    sf::Texture &thumbnail_texture,
    sf::Font &font, 
    wstring title, 
    float size_x, float size_y,
    int time,
    std::function<bool()> condition
) {
    this->drawPile = &drawPile;
    this->shape = sf::RectangleShape(sf::Vector2f(size_x, size_y));
    this->shape.setPosition(position_x, position_y);
    this->shape.setFillColor(sf::Color(150, 150, 150, 255));
    this->thumbnail = sf::Sprite(thumbnail_texture);
    this->thumbnail.setScale(0.1, 0.1);
    this->thumbnail.setPosition(shape.getPosition().x + 5, shape.getPosition().y + 5);
    this->title.setFont(font);
    this->title.setString(title);
    this->title.setCharacterSize(size_y*0.3);
    this->title.setFillColor(sf::Color::White);
    this->title.setStyle(sf::Text::Bold);
    this->title.setPosition(thumbnail.getPosition().x + thumbnail.getLocalBounds().getSize().x * thumbnail.getScale().x + (shape.getLocalBounds().getSize().x - 10 - thumbnail.getLocalBounds().getSize().x * thumbnail.getScale().x - this->title.getLocalBounds().getSize().x)/2, shape.getPosition().y + (shape.getLocalBounds().getSize().y - this->title.getLocalBounds().getSize().y)/2);
    this->condition = condition;
    this->prevPile = 0;
    this->drawOffset = 0;
    if(condition()) this->time = 0;
    else this->time = time;
}

void Achieve::draw (sf::RenderWindow &window) {
    if (!condition() || time == 0) return;

    if (timestamp.time_since_epoch() == std::chrono::system_clock::time_point::duration::zero()) {
        timestamp = std::chrono::system_clock::now();
        drawOffset = (*drawPile)++; 
        if (drawOffset > 0) {
            shape.setPosition(shape.getPosition().x, shape.getPosition().y - drawOffset*window.getSize().y*0.1f);
            title.setPosition(title.getPosition().x, title.getPosition().y - drawOffset*window.getSize().y*0.1f);
            thumbnail.setPosition(thumbnail.getPosition().x, thumbnail.getPosition().y - drawOffset*window.getSize().y*0.1f);
        }
    }

    auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - timestamp).count();

    if ((*drawPile) < prevPile) {
        drawOffset = (prevPile - (*drawPile));
        shape.setPosition(shape.getPosition().x, shape.getPosition().y + drawOffset*window.getSize().y*0.1f);
        title.setPosition(title.getPosition().x, title.getPosition().y + drawOffset*window.getSize().y*0.1f);
        thumbnail.setPosition(thumbnail.getPosition().x, thumbnail.getPosition().y + drawOffset*window.getSize().y*0.1f);
    }

    if ((time - elapsed) <= 1000) {
        shape.setFillColor(sf::Color(shape.getFillColor().r, shape.getFillColor().g, shape.getFillColor().b, 255.f * ((float)(time - elapsed)/1000.f)));
        title.setFillColor(sf::Color(title.getFillColor().r, title.getFillColor().g, title.getFillColor().b, 255.f * ((float)(time - elapsed)/1000.f)));
        thumbnail.setColor(sf::Color(255.f, 255.f, 255.f, 255.f * ((float)(time - elapsed)/1000.f)));
    }

    prevPile = *drawPile;
    if ((time - elapsed) <= 0) { (*drawPile)--; time = 0; }
    else {
        window.draw(shape);
        window.draw(title);
        window.draw(thumbnail);
    }
}

int Achieve::getTime (void) {
    return time;
}