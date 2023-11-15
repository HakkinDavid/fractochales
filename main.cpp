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
#include <regex>
#include "includes/Lightning/Lightning.h"
#include "includes/SFML/Graphics.hpp"
#include "includes/SFML/Audio.hpp"
#include "includes/Slider/slider.h"
#include "includes/Switch/switch.h"
#include "includes/Button/button.h"
#include "includes/ThickLine/thickline.h"
#include "includes/Physics/physics.h"

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

wstringstream console;

wstring to_super (wstring num) {
    const wchar_t super [10] = {
        L'⁰', L'¹', L'²', L'³', L'⁴', L'⁵', L'⁶', L'⁷', L'⁸', L'⁹'
    };
    for (int i = 0; i < num.length(); i++) {
        num[i] = super[num.at(i) - L'0'];
    }
    return num;
}

void format (wstringstream & s) {
    std::wstring temporal (s.str());
    const std::wregex exponential (L"e([+-])([0-9]+)");
    std::match_results<std::wstring::iterator> match;
    int substart = 0;

    // super efficient regex (each iteration, string is only analyzed after first match)
    while (regex_search(temporal.begin() + substart, temporal.end(), match, exponential)) {
        wstring replacement = L"×10" + (match[1].compare(L"-") ? wstring(L"⁻") : wstring(L"")) + to_super(match[2].str());
        temporal.replace(temporal.begin() + substart + match.position(0), temporal.begin() + substart + match.position(0) + match.length(0), replacement);
        substart += match.position(0) + replacement.length();
    }

    s.str(temporal);
}

int main() {
    sf::Image icon;
    icon.loadFromFile("images/fractochales.png");

    sf::RenderWindow window(sf::VideoMode(WINDOW_W, WINDOW_H), "Fractochales", sf::Style::Fullscreen);
    if (icon.loadFromFile("images/fractochales.png")) {
        window.setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    sf::Texture splash;
    splash.loadFromFile("images/fractochales.png");

    sf::Sprite splash_screen(splash);
    splash_screen.setPosition(sf::Vector2f((window.getSize().x - splash_screen.getLocalBounds().width)/2, (window.getSize().y - splash_screen.getLocalBounds().height)/2));

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
    font.loadFromFile("fonts/arial.ttf"); // THERE A VERY LIMITED SET OF FONTS SUPPORTING SUPERSCRIPT PLUS/MINUS SIGNS

    sf::Text text;
    sf::Text physicsOutput;
    text.setFont(font);
    physicsOutput.setFont(font);
    text.setCharacterSize(20);
    physicsOutput.setCharacterSize(20);
    text.setFillColor(sf::Color::White);
    physicsOutput.setFillColor(sf::Color::White);
    text.setStyle(sf::Text::Bold);
    physicsOutput.setStyle(sf::Text::Bold);
    text.setPosition(sf::Vector2f(window.getSize().x*0.01, window.getSize().y*0.01));
    physicsOutput.setPosition(window.getSize().x*0.99 - physicsOutput.getLocalBounds().getSize().x, window.getSize().y*0.01 + 50);

    sf::RectangleShape dim_text_bg;
    sf::RectangleShape dim_physicsOutput_bg;
    dim_text_bg.setFillColor(sf::Color(0, 0, 0, 50));
    dim_physicsOutput_bg.setFillColor(sf::Color(0, 0, 0, 50));
    dim_text_bg.setPosition(text.getPosition().x - 5, text.getPosition().y - 5);

    sf::RectangleShape loading_percentage;
    loading_percentage.setFillColor(sf::Color::Cyan);

    const float defaultLeeway = 0.24F;
    const float defaultBranch = 0.12F;
    auto t0 = std::chrono::system_clock::now();
    long double time;
    float leeway = defaultLeeway;
    float branch = defaultBranch;
    float lightning_width = 257;
    float lightning_height = 181;
    float lightning_scale = 6;
    float lightning_color [3] = {245, 230, 83};
    float alignmentOffset = (window.getSize().x - lightning_width*lightning_scale)/2;
    float* direction = nullptr;
    bool zap = false;
    bool linear_adjustment_line = false;
    bool switchingBG = false;
    bool attemptClose = false;

    int renderIndex = 0;
    int bgIndex = 0;

    // inicializar fondos
    sf::Texture city;
    city.loadFromFile("images/city.png");

    sf::Texture water;
    water.loadFromFile("images/water.jpg");

    sf::Texture wood;
    wood.loadFromFile("images/wood.jpg");

    sf::Texture shrek;
    shrek.loadFromFile("images/shrek.png");

    sf::Texture space;
    space.loadFromFile("images/space.jpg");

    sf::Texture black;
    black.loadFromFile("images/black.png");

    // fondos a iterar con el botón "alternar fondo"
    // esto podría sincronizarse con otro arreglo de valores de variable para entornos
    sf::Texture * bg [] = {&city, &water, &wood, &shrek, &space, &black, nullptr};

    // valores de entorno para el rayo
    // el primero se toma como el valor PREDETERMINADO o EJE
    float environmental_factors [] = {
        3107424876.30374896651003593080636010231246734962391002, // ciudad / aire [cbrt(26521541178535914242048000)*(0.79*2*5 + 0.21*2*6)] : 26521541178535914242048000 moléculas en un metro cúbico de aire, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del nitrógeno molecular (79%) y oxígeno (21%) para obtener los electrones en un metro lineal
        25746383589.4834602095982075849351178030077195309528665478627, // agua [cbrt((10/3)*(10^28))*(8)] : 3.333x10^28 moléculas en un metro cúbico de agua, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del agua como molécula para obtener los electrones en un metro lineal
        81206048428.74334182814399729120705789177218382976918208, // madera [cbrt(2042797000000000000000000000)*(64)] : 2042797000000000000000000000 moléculas en un metro cúbico de madera ( 550 [kg/m^3] / 12.011*6+1.00784*10+15.999*5 [g/mol] * Número de Avogadro ), obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" de la celulosa C6H10O5 (4*6+10+6*5) como molécula para obtener los electrones en un metro lineal
        2683956227.13375630504705598630107878318735183748706240, // pantano [cbrt(26521541178535914242048000)*(0.5*8 + 0.5*10)] : 26521541178535914242048000 moléculas en un metro cúbico de aire, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del nitrógeno molecular (50%) y metano (50%) para obtener los electrones en un metro lineal
        100, // vacío interestelar [cbrt(10^6)*1] : 10^6 átomos de hidrógeno por metro cúbico de espacio interestelar, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del hidrógeno singular para obtener los electrones en un metro lineal
        1.0f, // negro ... valor por defecto
        0.0f // fin del arreglo ... aunque este no lo requiere, se escribe para mantener la consistencia con el arreglo de fondos
    };

    float current_environmental_factor = environmental_factors[0];

    sf::Sprite background (*bg[bgIndex]);

    // inicializar interfaz
    // deslizadores
    Slider alignmentSlider (alignmentOffset, 0, window.getSize().x - lightning_width*lightning_scale, window.getSize().x * 0.04f, window.getSize().y * 0.47f, 2, font, L"Alineación", false, sf::Color::Black, sf::Color::White);
    Slider envfactorSlider (current_environmental_factor, 1, 10000000000, window.getSize().x * 0.04f, window.getSize().y * 0.53f, 0, font, L"Electrones por metro de alcance", true, sf::Color::Yellow, sf::Color::White);
    Slider branchSlider (branch, 0.0f, 0.5f, window.getSize().x * 0.04f, window.getSize().y * 0.62f, 0, font, L"Bifurcación", false, sf::Color::Magenta, sf::Color::White);
    Slider leewaySlider (leeway, 0.0f, 0.5f, window.getSize().x * 0.04f, window.getSize().y * 0.71f, 0, font, L"Libertad de acción", false, sf::Color::Cyan, sf::Color::White);
    Slider redSlider (lightning_color[0], 0.0f, 255.0f, window.getSize().x * 0.04f, window.getSize().y * 0.80f, 2, font, L"Matiz", true, sf::Color::Red, sf::Color::White);
    Slider greenSlider (lightning_color[1], 0.0f, 255.0f, window.getSize().x * 0.04f, window.getSize().y * 0.84f, 3, font, std::wstring(), true, sf::Color::Green, sf::Color::White);
    Slider blueSlider (lightning_color[2], 0.0f, 255.0f, window.getSize().x * 0.04f, window.getSize().y * 0.88f, 3, font, std::wstring(), true, sf::Color::Blue, sf::Color::White);
    // botones
    Button zapping (zap, window.getSize().x*0.05f, window.getSize().y*0.92f, font, L"Generar", 200, 50, sf::Color(47,45,194), sf::Color(67,65,224));
    Button backgroundButton (switchingBG, window.getSize().x*0.95f - 220, window.getSize().y*0.92f, font, L"Cambiar entorno", 220, 50, sf::Color(179, 125, 46), sf::Color(252, 210, 146));
    Button closeButton (attemptClose, window.getSize().x-75, 0, font, L"X", 75, 50, sf::Color::Red, sf::Color::Red);
    // interruptores
    Switch linear_adjustment_switch (linear_adjustment_line, window.getSize().x*0.075f, window.getSize().y*0.40f, font, L"Ajuste lineal");

    // colocar los deslizadores que recibirán eventos en grupo
    Slider * all_sliders [] = {&alignmentSlider, &branchSlider, &leewaySlider, &redSlider, &greenSlider, &blueSlider, &envfactorSlider};
    // colocar los botones que recibirán eventos en grupo
    Button * all_buttons [] = {&zapping, &closeButton, &backgroundButton};
    // colocar los interruptores que recibirán eventos en grupo
    Switch * all_switches [] = {&linear_adjustment_switch};

    // agrupar y ejecutar los eventos correspondientes a los sliders
    auto UI_events = [&] (int type, sf::Vector2i *mouse = nullptr) {
        for (auto &i : all_sliders) {
            switch (type) {
                case 0: // cuando usuario clickea
                    i->checkDragging(*mouse);
                break;
                case 1: // cuando usuario desclickea
                    i->setIsDragging(false);
                break;
                case 2: // cuando hay que actualizar el estado ... NO SE USA PORQUE ALGUNOS EVENTOS REQUIEREN REDIBUJADO O TRATAMIENTO ESPECIAL
                    i->updatePercentage(*mouse);
                break;
                case 3: // dibujar
                    i->draw(window);
                break;
                default:
                break;
            }
        }
        for (auto &i : all_buttons) {
            switch (type) {
                case 0: // cuando usuario clickea
                    i->checkClicking(*mouse);
                break;
                case 1: // cuando usuario desclickea
                    i->setIsClicking(false);
                break;
                case 2: // cuando hay que actualizar el estado ... NO SE USA PORQUE ALGUNOS EVENTOS REQUIEREN REDIBUJADO O TRATAMIENTO ESPECIAL
                    i->updateState();
                break;
                case 3: // dibujar
                    i->draw(window);
                break;
                default:
                break;
            }
        }
        for (auto &i : all_switches) {
            switch (type) {
                case 0: // cuando usuario clickea
                    i->checkClicking(*mouse);
                break;
                case 1: // cuando usuario desclickea
                    i->setIsClicking(false);
                break;
                case 2: // cuando hay que actualizar el estado ... NO SE USA PORQUE ALGUNOS EVENTOS REQUIEREN REDIBUJADO O TRATAMIENTO ESPECIAL
                    i->updateState();
                break;
                case 3: // dibujar
                    i->draw(window);
                break;
                default:
                break;
            }
        }
    };

    Lightning storm;
    wstringstream thunder_data, thunder_physics_data;
    vector<thickLine> thunder; // crear el vector de vértices a renderizar

    // función lambda que permite trabajar con las variables de main () por referencia
    // por lo que se llama sin parámetros

    auto retypeInfo = [&] () {
        long double e_mass = storm.getElectronicMass(current_environmental_factor);
        thunder_data.str(std::wstring());
        thunder_physics_data.str(std::wstring());
        console.str(std::wstring());
        //thunder_data << "Altura: " << fixed << setprecision(4) << ((thunder.back().position.y-1)/lightning_scale) * (storm.getGridHeightInMeters() / (float) storm.getHei()) << "m" << endl;
        thunder_data << "Ramas: " << storm.getN() << endl;
        thunder_data << "Electrones involucrados: " << storm.getInvolvedElectrons(current_environmental_factor) << endl;
        thunder_data << L"Masa electrónica total: " << scientific << setprecision(std::numeric_limits<long double>::digits10 + 1) << e_mass << "kg" << endl;
        thunder_data << L"Ajuste de mínimos cuadrados: x = " << fixed << setprecision(4) << direction[1] << " " << (direction[0] > 0 ? "+" : "-") << " " << fixed << setprecision(4) << abs(direction[0]) << "y" << endl;
        thunder_data << L"Coeficiente de correlación (R): " << fixed << setprecision(4) << direction[2] << endl;
        thunder_data << L"Coeficiente de determinación (R²): " << fixed << setprecision(4) << direction[2]*direction[2] << endl;
        thunder_data << L"Dimensión fractal: " << fixed << setprecision(4) << storm.getFracs()->back() << endl;

        thunder_physics_data << L"W = " << scientific << setprecision(4) << e_mass << L"kg × 9.81 m/s² = " << Physics::W(e_mass) << L"N" << endl;
        thunder_physics_data << L"t = " << time << L"ns" << endl;

        format(thunder_data);
        format(thunder_physics_data);

        text.setString(thunder_data.str());
        physicsOutput.setString(thunder_physics_data.str() + console.str());

        physicsOutput.setPosition(window.getSize().x*0.99 - physicsOutput.getLocalBounds().getSize().x, physicsOutput.getPosition().y);
        dim_physicsOutput_bg.setPosition(physicsOutput.getPosition().x - 5, physicsOutput.getPosition().y - 5);
        
        dim_text_bg.setSize(sf::Vector2f(text.getLocalBounds().getSize().x + 10, text.getLocalBounds().getSize().y + 10));
        dim_physicsOutput_bg.setSize(sf::Vector2f(physicsOutput.getLocalBounds().getSize().x + 10, physicsOutput.getLocalBounds().getSize().y + 10));
    };

    auto recalculateLightningVertex = [&] (bool skipRedraw = false) {
        thunder.clear();
        for (int i = storm.getHei()-1; i >= 0; i--) {
            for (int j = storm.getWid()-1; j >= 0; j--) {
                if (storm.getGrid()[i][j].getIsLight()) {
                    int i0 = storm.getGrid()[i][j].getPrevX();
                    int j0 = storm.getGrid()[i][j].getPrevY();
                    thunder.emplace_back(sf::Vector2f(alignmentOffset + j*lightning_scale, i*lightning_scale + 1), sf::Vector2f(alignmentOffset + j0*lightning_scale, i0*lightning_scale + 1), sf::Color(lightning_color[0], lightning_color[1], lightning_color[2]), 2.f);
                    thunder.emplace_back(sf::Vector2f(alignmentOffset + j*lightning_scale, i*lightning_scale + 1), sf::Vector2f(alignmentOffset + j0*lightning_scale, i0*lightning_scale + 1), sf::Color(255, 255, 255), 4.f);
                }
            }
        }
        if (linear_adjustment_line) {
            thunder.emplace_back(sf::Vector2f(alignmentOffset + direction[1]*lightning_scale, 1), sf::Vector2f(alignmentOffset + (lightning_height*direction[0] + direction[1])*lightning_scale, lightning_height*lightning_scale + 1), sf::Color(255 - lightning_color[0], 255 - lightning_color[1], 255 - lightning_color[2]), 1.f);
        }
        reverse(thunder.begin(), thunder.end());
        if (!skipRedraw) renderIndex = 0;
        else renderIndex = thunder.size();
    };

    auto generateLightning = [&] () {
        t0 = std::chrono::system_clock::now();
        if (direction != nullptr) delete [] direction; // liberar memoria usada por el direction previo
        storm = Lightning(lightning_height, lightning_width, leeway, branch);
        storm.randomize(); // aleatorizar los valores resistivos en el entorno
        storm.superTraverse(); // generar el trazo de luz con coordenada inicial (0, 0)
        direction = storm.directionComp();
        storm.fractalComp();
        recalculateLightningVertex();
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - t0).count();
        retypeInfo();
    };

    generateLightning();

    auto start_time = std::chrono::system_clock::now();
    bool yetToBoot = true;

    while (window.isOpen())
    {
        sf::Event event;
        
        while (window.pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        if (yetToBoot) {
            auto elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(std::chrono::system_clock::now() - start_time).count();
            if (elapsed < 1000) {
                loading_percentage.setPosition(sf::Vector2f((window.getSize().x - loading_percentage.getLocalBounds().width)/2, splash_screen.getLocalBounds().height + (window.getSize().y - splash_screen.getLocalBounds().height)/2));
                loading_percentage.setSize(sf::Vector2f(500.0f * ((float) elapsed/1000.0f), 5));
                window.clear(sf::Color::White);
                window.draw(splash_screen);
                window.draw(loading_percentage);
                window.display();
                continue;
            }
            else {
                yetToBoot = false;
            }
        }

        // renderizar un solo punto a la vez (hasta 120), para dar la ilusión de que el rayo "cae"
        if (renderIndex+1 <= thunder.size()) renderIndex++;
        if (renderIndex != thunder.size() && renderIndex >= 12) renderIndex = thunder.size();

        window.clear(); // CLEARS CONTENT OF WINDOW
        window.draw(background);

        // si el usuario está haciendo click izquierdo
        if(event.type == sf::Event::MouseButtonPressed){
            if(event.mouseButton.button == sf::Mouse::Left){
                sf::Vector2i mousepos = sf::Mouse::getPosition(window);
                UI_events(0, &mousepos);
            }
            
        }

        // si el usuario deja de mantener click izquierdo
        else if(event.type == sf::Event::MouseButtonReleased){
            if(event.mouseButton.button == sf::Mouse::Left){
                UI_events(1);
            }
        }

        sf::Vector2i mousepos_update = sf::Mouse::getPosition(window);

        if (backgroundButton.updateState() && switchingBG) {
            bgIndex++;
            if (bg[bgIndex] == nullptr) bgIndex = 0;
            background.setTexture(*bg[bgIndex]);
            current_environmental_factor = environmental_factors[bgIndex];
            leeway = defaultLeeway * (current_environmental_factor)/(environmental_factors[0]); // if more electrons, more leeway
            branch = defaultBranch * (environmental_factors[0])/(current_environmental_factor); // if more electrons, less branching
        }

        if (envfactorSlider.updatePercentage(mousepos_update)) {
            retypeInfo();
        }
        
        if (linear_adjustment_switch.updateState() || alignmentSlider.updatePercentage(mousepos_update) || redSlider.updatePercentage(mousepos_update) || greenSlider.updatePercentage(mousepos_update) || blueSlider.updatePercentage(mousepos_update)) {
            recalculateLightningVertex(true);
        }
        leewaySlider.updatePercentage(mousepos_update);
        branchSlider.updatePercentage(mousepos_update);
        
        if (closeButton.updateState() && attemptClose) window.close();
        
        if (zapping.updateState() && zap){
            generateLightning();
            window.clear(sf::Color::White);
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
        for (int i = 0; i < renderIndex; i++) {
            thunder.at(i).draw(window);
        }
        window.draw(dim_text_bg);
        window.draw(dim_physicsOutput_bg);
        window.draw(text);
        window.draw(physicsOutput);
        UI_events(3);
        window.display();
    }
    // liberar memoria
    delete [] direction;
    return 0;
}