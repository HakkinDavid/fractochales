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
    int old_timer,
    std::function<bool()> condition
) {
    this->drawPile = &drawPile;
    this->shape = sf::RectangleShape(sf::Vector2f(size_x, size_y));
    this->shape.setPosition(position_x, position_y);
    this->shape.setFillColor(sf::Color(150, 150, 150, 255));
    this->title.setFont(font);
    this->title.setString(title);
    this->title.setCharacterSize(size_y*0.3);
    this->title.setFillColor(sf::Color::White);
    this->title.setStyle(sf::Text::Bold);
    this->title.setPosition(shape.getPosition().x, shape.getPosition().y - this->title.getCharacterSize());
    this->thumbnail = sf::Sprite(thumbnail_texture);
    this->thumbnail.setScale(0.1, 0.1);
    this->thumbnail.setPosition(shape.getPosition().x + 5, shape.getPosition().y + 5);
    this->condition = condition;
    this->prevPile = 0;
    this->drawOffset = 0;
    if(condition()) this->timer = old_timer;
    else this->timer = 6000;
}

void Achieve::draw (sf::RenderWindow &window) {
    if (!condition() || timer == 0) return;

    if (timer == 6000) {
        drawOffset = (*drawPile)++; 
        if (drawOffset > 0) {
            shape.setPosition(shape.getPosition().x, shape.getPosition().y - drawOffset*window.getSize().y*0.1f);
            title.setPosition(title.getPosition().x, title.getPosition().y - drawOffset*window.getSize().y*0.1f);
            thumbnail.setPosition(thumbnail.getPosition().x, thumbnail.getPosition().y - drawOffset*window.getSize().y*0.1f);
        }
    }

    if ((*drawPile) < prevPile) {
        drawOffset = (prevPile - (*drawPile));
        shape.setPosition(shape.getPosition().x, shape.getPosition().y + drawOffset*window.getSize().y*0.1f);
        title.setPosition(title.getPosition().x, title.getPosition().y + drawOffset*window.getSize().y*0.1f);
        thumbnail.setPosition(thumbnail.getPosition().x, thumbnail.getPosition().y + drawOffset*window.getSize().y*0.1f);
    }

    if (timer <= 1000) {
        shape.setFillColor(sf::Color(150, 150, 150, 255.f * ((float)timer/1000.f)));
        title.setFillColor(sf::Color(255, 255, 255, 255.f * ((float)timer/1000.f)));
        thumbnail.setColor(sf::Color(255.f * ((float)timer/1000.f), 255.f * ((float)timer/1000.f), 255.f * ((float)timer/1000.f)));
    }

    window.draw(shape);
    window.draw(title);
    window.draw(thumbnail);

    prevPile = *drawPile;
    timer--;
    if (timer == 0) { (*drawPile)--; }
}

int Achieve::getTimer (void) {
    return timer;
}