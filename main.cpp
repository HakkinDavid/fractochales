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

#define WINDOW_W 1920
#define WINDOW_H 1080

vector<sf::Vertex> generateLighting (Lightning& mcqueen) { // objeto lightning por defecto, si quieres cambiar los valores usados en el programa, ve a la llamada
    // kachow
    mcqueen.randomize(); // aleatorizar los valores resistivos en el entorno
    mcqueen.superTraverse(); // generar el trazo de luz con coordenada inicial (0, 0)
    vector<sf::Vertex> thunder; // crear el vector de vértices a renderizar

    for (int i = mcqueen.getHei()-1; i >= 0; i--) {
        for (int j = mcqueen.getWid()-1; j >= 0; j--) {
            if (mcqueen.getGrid()[i][j].getIsLight()) {
                int i0 = mcqueen.getGrid()[i][j].getPrevX();
                int j0 = mcqueen.getGrid()[i][j].getPrevY();
                thunder.emplace_back(sf::Vector2f(WINDOW_W/3 + j*15, i*15 + 1));
                thunder.emplace_back(sf::Vector2f(WINDOW_W/3 + j0*15, i0*15 + 1));
            }
        }
    }
    return thunder;
}

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

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Fractochales", sf::Style::Default);
    if (icon.loadFromFile("images/fractochales.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    // TIME slider
    sf::RectangleShape timeSlider(sf::Vector2f(250,15));
    timeSlider.setPosition(window.getSize().x * 0.04f, window.getSize().y * 0.8f);
    timeSlider.setFillColor(sf::Color::White);

    // TIME slider handle
    sf::RectangleShape timeSliderHandle(sf::Vector2f(20,25));
    timeSliderHandle.setPosition(timeSlider.getSize().x/1.5,timeSlider.getPosition().y-2);
    timeSliderHandle.setFillColor(sf::Color::Black);

    // BUTTON TIME.
    sf::RectangleShape lightButt(sf::Vector2f(100,50));
    lightButt.setPosition(window.getSize().x*0.4f,window.getSize().y*0.85f);
    lightButt.setFillColor(sf::Color(47,45,194,255));

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
    sf::Text timeSliderTitle; // Position is dependent on the Time Slider's position
    sf::Text timeSliderHandleT; // Slides with the slider; says what % it is at
    sf::Text lightButtText; // PRESS TO GENERATE LIGHTNING

    // Inicializacion de fonts y strings para Labels
    text.setFont(font);
    timeSliderTitle.setFont(font);
    timeSliderTitle.setString("currently obsolete slider");
    timeSliderHandleT.setFont(font);
    lightButtText.setFont(font);
    lightButtText.setString("ZAP!!!");

    // Setting the character size
    text.setCharacterSize(24);
    timeSliderTitle.setCharacterSize(18);
    timeSliderHandleT.setCharacterSize(12);
    lightButtText.setCharacterSize(20);

    // Setting the color of the text to either pre-sets or customs (R,G,B, Opacity)
    text.setFillColor(sf::Color::White);
    timeSliderTitle.setFillColor(sf::Color::White);
    timeSliderHandleT.setFillColor(sf::Color::White);
    lightButtText.setFillColor(sf::Color(230,230,230,255));

    // Stylization of text (Bold, Itallic, etc.)
    text.setStyle(sf::Text::Bold);
    timeSliderTitle.setStyle(sf::Text::Bold);
    lightButtText.setStyle(sf::Text::Bold);

    // Placing them on specific locations in the window (X,Y)
    timeSliderTitle.setPosition(timeSlider.getPosition().x/2+35,timeSlider.getPosition().y - (timeSliderTitle.getCharacterSize()*2));
    timeSliderHandleT.setPosition(timeSliderHandle.getPosition().x-2,timeSlider.getPosition().y + (timeSliderHandleT.getCharacterSize()*2.15));
    lightButtText.setPosition(lightButt.getPosition().x+20,lightButt.getPosition().y+16);

    // Idk what this does tbh
    auto start_time = std::chrono::system_clock::from_time_t((time_t) 0);

    bool isDragging=false; // Determines whether the user is dragging a slider handle or not
    bool isClicking=false; // Determines stuff for buttons

    // Open Window Cycle (Program = running)
    while (window.isOpen())
    {
        auto exec_time = std::chrono::system_clock::now();
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        // Time Slider handling; si el usuario esta clickeando izq.
        if(event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mousePos=sf::Mouse::getPosition(window);
                sf::FloatRect timeBounds = timeSliderHandle.getGlobalBounds(); // ghost rectangle...
                if(timeBounds.contains(static_cast<sf::Vector2f>(mousePos))){ // Si el click izq. esta dentro del slider handle
                    isDragging=true;
                }
            }
            
        }
        // Si el usuario deja de mantener click izq.
        else if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                isDragging=false;
            }
        }
        //Si el usuario esta holding click izq.
        if(isDragging){
            sf::Vector2i mousePos=sf::Mouse::getPosition(window);
            // If out of bounds (below 0; left overboard)
            if(mousePos.x - (timeSliderHandle.getLocalBounds().width / 2) < timeSlider.getPosition().x){
                timeSliderHandle.setPosition(timeSlider.getPosition().x,timeSliderHandle.getPosition().y);
            }
            // If out of bounds (above 100; right overboard)
            else if (mousePos.x + (timeSliderHandle.getLocalBounds().width / 2) > timeSlider.getPosition().x + timeSlider.getSize().x){
                timeSliderHandle.setPosition(timeSlider.getPosition().x + timeSlider.getSize().x - timeSliderHandle.getSize().x, timeSliderHandle.getPosition().y);
            }
            // Inside (No esta Afuera)
            else{
                timeSliderHandle.setPosition(mousePos.x - timeSliderHandle.getSize().x/2, timeSliderHandle.getPosition().y);
            }
        }

        // Slider #1 (our currently only one) values from 0% to 100% ; make sure to divide this by 100 in calculations
        float timeSliderPercentage=((timeSliderHandle.getPosition().x - timeSlider.getPosition().x)/(timeSlider.getSize().x - timeSliderHandle.getLocalBounds().width))*100.0f;
        stringstream thP;
        thP << setprecision(3) << timeSliderPercentage;
        timeSliderHandleT.setString(thP.str() + "%");

        // Clicking Button Zap
        if(event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mousePos=sf::Mouse::getPosition(window);
                sf::FloatRect buttBounds = lightButt.getGlobalBounds(); // ghost rectangle... [2]
                if(buttBounds.contains(static_cast<sf::Vector2f>(mousePos))){ // Si el click izq. esta dentro del slider handle
                    isClicking=true;
                }
            }
        }
            // makes the button light up when clicking it
            if(isClicking==true){
                lightButt.setFillColor(sf::Color(67,65,224,255));
            }
            if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                isClicking=false;
                lightButt.setFillColor(sf::Color(47,45,194,255));
            }
        }

        window.clear(sf::Color(6, 0, 64));
        window.draw(s_bg);
        Lightning storm (109, 65);
        vector<sf::Vertex> thunder = generateLighting(storm);

        wstringstream thunder_data;
        thunder_data << "Rayo con altura: " << fixed << setprecision(2) << thunder.at(0).position.y << endl;
        thunder_data << "Ramas: " << storm.getN() << endl;
        thunder_data << L"Dimensión fractal: " << fixed << setprecision(4) << storm.fractalComp() << endl;
        text.setString((wstring) thunder_data.str());
        window.draw(&thunder[0], thunder.size(), sf::Lines);
        window.draw(timeSlider);
        window.draw(timeSliderHandle);
        window.draw(text);
        window.draw(timeSliderTitle);
        window.draw(timeSliderHandleT);
        window.draw(lightButt);
        window.draw(lightButtText);
        if (thunder.at(0).position.y >= WINDOW_H*30/100) {
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
        window.display();
        start_time = std::chrono::system_clock::now();
    }
    return 0;
}