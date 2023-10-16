#include <iostream>
#include <conio.h>
#include "includes/Lightning/Lightning.h"
#include "includes/SFML/Graphics.hpp"

int main()
{
    sf::RenderWindow window(sf::VideoMode(1920, 1080), "Fractochales", sf::Style::Fullscreen);
    sf::Vertex thunder[] =
    {
        sf::Vertex(sf::Vector2f(10, 10)),
        sf::Vertex(sf::Vector2f(150, 150)),
        sf::Vertex(sf::Vector2f(150, 150)),
        sf::Vertex(sf::Vector2f(300, 150))
    };


    while (window.isOpen())
    {
        sf::Event event;
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(6, 0, 64));
        window.draw(thunder, 4, sf::Lines);
        window.display();
    }
    Lightning rasho;
    rasho.randomize();
    rasho.traverse(0, 0);
    rasho.show();
    
    //cout << "Presiona cualquier tecla para salir." << endl;
    //_getch();
    return 0;
}