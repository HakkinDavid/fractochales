/*
    nombre: Interfaz del proyecto e implementación del ejecutable final
    autores: David Emmanuel Santana Romero, Mauricio Alcántar Dueñas y Diego Emilio Casta Valle
*/
#include <iostream>
#include <string>
#include <chrono>
#include <vector>
#include <iomanip>
#include <sstream>
#include "includes/Lightning/Lightning.h"
#include "includes/SFML/Graphics.hpp"
#include "includes/SFML/Audio.hpp"
#include "includes/Slider/slider.h"
#include "includes/Button/button.h"

#define WINDOW_W 1920
#define WINDOW_H 1080

// ¡¡¡¡FRACTALES!!!! - Nota de David Emmanuel Santana Romero
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

// F r a  c t al e eeEEEes s s - Nota de Mauricio Putricio Alcántar Dueñas
// He estado pensando en cómo chingados computar la mierda esta y me estoy volviendo chango
// Hay algunos puntos que quiero tocar:
//   1. No podemos tomar el factor escala en base a la segunda rama más extensa y luego
//      tomar el número total de piezas pequeñas como el número de líneas, porque esas dos
//      mediciones no son consistentes entre sí y nos podrían dar resultados como
//      que la dimensión fractal de un rayo es 7.62. Which makes no sense.
//   2. La manera en la que encontraríamos la segunda rama más extensa es algo complicada y
//      me tomaría mil años explicarlo pero. La manera en la que lo haríamos también implicaría 
//      encontrar el tamaño de las demás ramas así que En Teoría podríamos hacer el valor más
//      exacto al tomar un promedio de la extensión de las ramas.
//   3. Pero eso luego me lleva a lo complicado que es contar las ramas, verificar qué rama es 
//      en realidad una subrama de otra, tomar las longitudes como la línea más extensa o el total 
//      de líneas en el subárbol Man I Don't Fuckin Know. 
// Por ahora el plan sería encontrar las ramas más largas que se desprendan de la rama principal,
// hacer un promedio de sus longitudes y usar ESO (cantidad de subramas y promedio de longitudes)
// para calcular la dimensión.
//      tl;dr this shit sucks man i think i need an int matrix and a 12 pack of beer

int main(){
    sf::Image icon;
    icon.loadFromFile("images/fractochales.png");

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Fractochales", sf::Style::Fullscreen);
    if (icon.loadFromFile("images/fractochales.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

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

    sf::Font font;  // Sets our font to this
    font.loadFromFile("fonts/ComicSansMS3.ttf");

    sf::Text text;

    // Inicializacion de fonts y strings para Labels
    text.setFont(font);

    // Setting the character size
    text.setCharacterSize(24);

    // Setting the color of the text to either pre-sets or customs (R,G,B, Opacity)
    text.setFillColor(sf::Color::White);

    // Stylization of text (Bold, Itallic, etc.)
    text.setStyle(sf::Text::Bold);

    text.setPosition(sf::Vector2f(window.getSize().x*0.01, window.getSize().y*0.01));

    float leeway = 0.24F;
    float branch = 0.12F;
    float lightning_width = 257;
    float lightning_height = 181;
    float lightning_scale = 6;
    float alignmentOffset = (window.getSize().x - lightning_width*lightning_scale)/2;
    bool zap = false;
    bool attemptClose = false;

    // inicializar interfaz
    Slider alignmentSlider (alignmentOffset, 0, window.getSize().x - lightning_width*lightning_scale, window.getSize().x * 0.04f, window.getSize().y * 0.6f, font, L"Alineación", false, sf::Color::Black, sf::Color::White);
    Slider branchSlider (branch, 0.0f, 1.0f, window.getSize().x * 0.04f, window.getSize().y * 0.7f, font, L"Bifurcación", false, sf::Color::Magenta, sf::Color::White);
    Slider leewaySlider (leeway, 0.0f, 1.0f, window.getSize().x * 0.04f, window.getSize().y * 0.8f, font, L"Libertad de acción", false, sf::Color::Cyan, sf::Color::White);
    Button zapping (zap, window.getSize().x*0.04f, window.getSize().y*0.9f, font, L"Zap", 200, 50, sf::Color(47,45,194,255), sf::Color(67,65,224,255));
    Button closeButton (attemptClose, window.getSize().x-75, 0, font, L"X", 75, 50, sf::Color::Red, sf::Color::Red);

    Lightning storm;
    wstringstream thunder_data;
    vector<sf::Vertex> thunder; // crear el vector de vértices a renderizar

    // función lambda que permite trabajar con las variables de main () por referencia
    // por lo que se llama sin parámetros
    auto recalculateLightningVertex = [&] () {
        thunder.clear();
        for (int i = storm.getHei()-1; i >= 0; i--) {
            for (int j = storm.getWid()-1; j >= 0; j--) {
                if (storm.getGrid()[i][j].getIsLight()) {
                    int i0 = storm.getGrid()[i][j].getPrevX();
                    int j0 = storm.getGrid()[i][j].getPrevY();
                    thunder.emplace_back(sf::Vector2f(alignmentOffset + j*lightning_scale, i*lightning_scale + 1));
                    thunder.emplace_back(sf::Vector2f(alignmentOffset + j0*lightning_scale, i0*lightning_scale + 1));
                }
            }
        }
    };

    auto generateLightning = [&] () {
        storm = Lightning(lightning_height, lightning_width, leeway, branch);
        storm.randomize(); // aleatorizar los valores resistivos en el entorno
        storm.superTraverse(); // generar el trazo de luz con coordenada inicial (0, 0)
        recalculateLightningVertex();
        thunder_data.str(std::wstring());
        thunder_data << "Altura: " << (int) thunder[0].position.y << endl;
        thunder_data << "Ramas: " << storm.getN() << endl;
        thunder_data << L"Dimensión fractal: " << fixed << setprecision(4) << storm.fractalComp() << endl;
    };

    generateLightning();

    // Open Window Cycle (Program = running)
    while (window.isOpen())
    {
        if (attemptClose) window.close();
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(); // CLEARS CONTENT OF WINDOW
        window.draw(s_bg);

        // si el usuario está haciendo click izquierdo
        if(event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                alignmentSlider.checkDragging(mousepos);
                leewaySlider.checkDragging(mousepos);
                branchSlider.checkDragging(mousepos);
                zapping.checkClicking(mousepos);
                closeButton.checkClicking(mousepos);
            }
            
        }

        // si el usuario deja de mantener click izquierdo
        else if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                alignmentSlider.setIsDragging(false);
                leewaySlider.setIsDragging(false);
                branchSlider.setIsDragging(false);
                zapping.setIsClicking(false);
                closeButton.setIsClicking(false);
            }
        }
        
        if (alignmentSlider.updatePercentage(sf::Mouse::getPosition(window))) {
            recalculateLightningVertex();
        }
        leewaySlider.updatePercentage(sf::Mouse::getPosition(window));
        branchSlider.updatePercentage(sf::Mouse::getPosition(window));
        if (zapping.updateState()) {
            window.clear(sf::Color::White);
        }
        closeButton.updateState();
        
        if (zap){
            generateLightning();
            switch (1 + sfx_i % 3) {
                case 1:
                    sfx_i++;
                    sound1.play();
                    break;
                case 2:
                    sfx_i++;
                    sound2.play();
                    break;
                case 3:
                    sfx_i++;
                    sound3.play();
                    break;
                default:
                    break;
            }
        }
        text.setString(thunder_data.str());
        window.draw(&thunder[0], thunder.size(), sf::Lines);
        window.draw(text);
        alignmentSlider.draw(window);
        leewaySlider.draw(window);
        branchSlider.draw(window);
        zapping.draw(window);
        closeButton.draw(window);
        window.display();
    }
    return 0;
}