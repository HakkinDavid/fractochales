#include <iostream>
#include <conio.h>
#include <chrono>
#include <vector>
#include "includes/Lightning/Lightning.h"
#include "includes/SFML/Graphics.hpp"
#include "includes/SFML/Audio.hpp"

#define WINDOW_W 1920
#define WINDOW_H 1080

vector<sf::Vertex> generateLighting () {
    // thunder.
    // feel the thunder.
    // lightning as a thunder.
    Lightning rasho(109, 65);
    rasho.randomize();
    rasho.traverse(0, 0);
    vector<sf::Vertex> thunder;

    for (int i = rasho.getHei()-1; i >= 0; i--) {
        for (int j = rasho.getWid()-1; j >= 0; j--) {
            if (rasho.getGrid()[i][j].getIsLight()) {
                int i0 = rasho.getGrid()[i][j].getPrevX();
                int j0 = rasho.getGrid()[i][j].getPrevY();
                thunder.emplace_back(sf::Vector2f(WINDOW_W/3 + j*25, i*25));
                thunder.emplace_back(sf::Vector2f(WINDOW_W/3 + j0*25, i0*25));
            }
        }
    }
    return thunder;
}

// ¡¡¡¡FRACTALES!!!!
// D = log(N) / log (S) | Fórmula de la dimensión fractal
// D | Dimensión fractal
// N | Número de piezas pequeñas final
//    En sentido estricto, el número de piezas final es el número de líneas o rayos triviales
//    ... es decir, el número de vértices entre dos.
// S | Factor escala
//    >> Tamaño de la pieza padre dividido por el tamaño de la pieza hija
//    >> Por ejemplo, si la pieza hija tiene la mitad de tamaño, el factor escala es 2
//    La pieza original es la rama del rayo más extensa
//      ¡¡¡NO ES LO MISMO QUE LA QUE LLEGA MÁS LEJOS!!!
//      La rama del rayo puede curvearse, enrollarse, ir en diagonal, etcétera
//    La pieza hija inmediata sería la segunda rama más extensa
//    Al tratar con líneas, nuestro factor de escala es la razón de longitudes (mayor sobre menor)

int main()
{    
    sf::Image icon;
    icon.loadFromFile("images/fractochales.png");

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Fractochales", sf::Style::Default);
    window.setKeyRepeatEnabled(false);
    window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());

    sf::Texture background;
    background.loadFromFile("images/city.png");
    sf::Sprite s_bg(background);

    sf::SoundBuffer buffer1;
    buffer1.loadFromFile("sfx/thunder1.wav");
    sf::Sound sound1;
    sound1.setBuffer(buffer1);

    sf::SoundBuffer buffer2;
    buffer2.loadFromFile("sfx/thunder2.wav");
    sf::Sound sound2;
    sound2.setBuffer(buffer2);

    sf::SoundBuffer buffer3;
    buffer3.loadFromFile("sfx/thunder3.wav");
    sf::Sound sound3;
    sound3.setBuffer(buffer3);

    int sfx_i = 1;

    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(6, 0, 64));
        window.draw(s_bg);
        auto thunder = generateLighting();
        window.draw(&thunder[0], thunder.size(), sf::Lines);
        switch (1 + (rand() % 6)) {
            case 3:
                switch (sfx_i) {
                    case 1:
                        sound1.play();
                        break;
                    case 2:
                        sound2.play();
                        break;
                    case 3:
                        sound3.play();
                        break;
                    default:
                        break;
                }
                sfx_i++;
                if (sfx_i >= 4) sfx_i = 1;
                break;
            default:
            break;
        }
        window.display();
    }
    return 0;
}