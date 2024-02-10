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
#include "includes/ThickLine/thickline.cpp"
#include "includes/Physics/physics.h"
#include "includes/Achieve/achieve.h"

#define MOBILE false
#if defined(WIN32)
    #define VERSION_KIND L"Windows"
#elif __APPLE__
    #include <TargetConditionals.h>
    #if TARGET_OS_IPHONE
        #define VERSION_KIND L"iOS"
        #define MOBILE true
    #else
        #define VERSION_KIND L"macOS"
    #endif
#elif __ANDROID__
    #define VERSION_KIND L"Android"
    #define MOBILE true
#elif __linux__
    #define VERSION_KIND L"GNU/Linux"
#else
    #define VERSION_KIND L"Unknown"
#endif

#ifndef VERSION
    #define VERSION 100
#endif

sf::RenderWindow * window = nullptr;
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
        wstring replacement = (match[2].str() != L"00" ? L"×10" + (match[1].str() == L"-" ? wstring(L"⁻") : wstring(L"")) + (match[2].str() == L"01" && match[1] == L"+" ? wstring(L"") : to_super(match[2].str())) : L"");
        temporal.replace(temporal.begin() + substart + match.position(0), temporal.begin() + substart + match.position(0) + match.length(0), replacement);
        substart += match.position(0) + replacement.length();
    }

    if (temporal.back() == L'\n') {
        temporal.pop_back();
    }

    s.str(temporal);
}

int main() {
    window = new sf::RenderWindow (sf::VideoMode::getDesktopMode(), "Fractochales", sf::Style::Fullscreen);

    sf::Image icon;
    icon.loadFromFile("images/fractochales.png");

    if (icon.loadFromFile("images/fractochales.png")) {
        window->setIcon(icon.getSize().x, icon.getSize().y, icon.getPixelsPtr());
    }

    sf::Texture splash;
    splash.loadFromFile("images/fractochales.png");

    sf::Texture nowin3d;
    nowin3d.loadFromFile("images/3d.png");

    sf::Texture watermark_texture;
    watermark_texture.loadFromFile("images/waterchales.png");

    sf::Sprite splash_screen(splash);
    sf::Sprite splash_3d(nowin3d);

    sf::Sprite watermark_logo(watermark_texture);
    splash_3d.setScale(0.125, 0.125);
    watermark_logo.setScale(0.15, 0.15);
    watermark_logo.setColor(sf::Color(255, 255, 255, 80));

    sf::SoundBuffer buffer0;
    buffer0.loadFromFile("sfx/startup.wav");
    sf::Sound startup;
    startup.setBuffer(buffer0);

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
    font.loadFromFile("fonts/arial.ttf"); // THERE A VERY LIMITED SET OF FONTS SUPPORTING SUPERSCRIPT

    sf::Text text;
    sf::Text physicsOutput;
    sf::Text currentTitle;
    sf::Text watermarkText;
    sf::Text loading_text;

    text.setFont(font);
    physicsOutput.setFont(font);
    currentTitle.setFont(font);
    watermarkText.setFont(font);
    loading_text.setFont(font);

    text.setCharacterSize(16 * (MOBILE ? 2 : 1));
    physicsOutput.setCharacterSize(16 * (MOBILE ? 2 : 1));
    currentTitle.setCharacterSize(32 * (MOBILE ? 2 : 1));
    watermarkText.setCharacterSize(16 * (MOBILE ? 2 : 1));
    loading_text.setCharacterSize(16 * (MOBILE ? 2 : 1));

    text.setFillColor(sf::Color::White);
    physicsOutput.setFillColor(sf::Color::White);
    currentTitle.setFillColor(sf::Color(255, 255, 255, 150));
    watermarkText.setFillColor(sf::Color(255, 255, 255, 122));
    loading_text.setFillColor(sf::Color::White);

    text.setStyle(sf::Text::Bold);
    physicsOutput.setStyle(sf::Text::Bold);
    currentTitle.setStyle(sf::Text::Bold);
    watermarkText.setStyle(sf::Text::Bold);
    loading_text.setStyle(sf::Text::Bold);

    wstring advice_text[] = {
        L"Cargando las nubes",
        L"Sintonizando a Fimbres",
        L"Esperando el convivio",
        L"Quitando el tercer nombre de Casta",
        L"Comprobando los pararrayos",
        L"Mi mamá es una figura fractal",
        L"La velocidad máxima de un relámpago es de 100,000 km/s",
        L"Un espectador percibe primero el relámpago y luego el trueno",
        L"Realizamos cálculos estequiométricos para obtener las masas de los rayos",
        L"Una figura fractal se encuentra entre dos dimensiones enteras",
        L"¿Sabías que tenemos " + to_wstring(VERSION) + L" contribuciones de código en este programa?",
        L"En la práctica, ningún rayo es idéntico",
        L"Fractochales funciona en Windows, Mac, Linux, Android y iOS",
        L"El motor 3D es enteramente de nuestra autoría",
        L"Hirales estaría orgulloso de nuestras proyecciones 3D sobre un plano 2D",
        L"Técnicamente emulamos el trazado de rayos",
        L"Mi mamá es un vector ortogonal",
        L"Fractochales es la contracción de Fractales Tochos",
        L"En cada relámpago simulamos cerca de 3 billones de electrones, a escala macroscópica y en 3D"
    };

    int advice_max_num = sizeof(advice_text) / sizeof(wstring);

    loading_text.setString(advice_text[rand() % advice_max_num]);
    
    wstringstream watermarkTextStream;

    watermarkTextStream
        << L"Fractochales ("
        << wstring(VERSION_KIND)
        << L" v"
        << fixed
        << setprecision(2)
        << (float) VERSION / 100
        << L")\nMauricio Alcántar Dueñas\nDavid Emmanuel Santana Romero\nDiego Emilio Casta Valle";
    watermarkText.setString(watermarkTextStream.str());

    sf::RectangleShape dim_text_bg;
    sf::RectangleShape dim_physicsOutput_bg;
    dim_text_bg.setFillColor(sf::Color(0, 0, 0, 50));
    dim_physicsOutput_bg.setFillColor(sf::Color(0, 0, 0, 50));

    sf::RectangleShape left_menu_bg;
    sf::RectangleShape right_menu_bg;
    left_menu_bg.setFillColor(sf::Color(120, 120, 120, 120));
    right_menu_bg.setFillColor(sf::Color(120, 120, 120, 120));

    sf::RectangleShape loading_percentage;
    loading_percentage.setFillColor(sf::Color::Cyan);

    int bgIndex = 0;

    // inicializar fondos
    sf::Texture city;
    city.loadFromFile("images/city.jpg");

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
    // nombres a mostrar
    const wstring bgTitle [] = {L"Aire", L"Agua", L"Madera", L"Pantano", L"Espacio", L"Libre", L""};

    // valores de entorno para el rayo
    // el primero se toma como el valor PREDETERMINADO o EJE
    const float environmental_factors [] = {
        3107424876.30374896651003593080636010231246734962391002, // ciudad / aire [cbrt(26521541178535914242048000)*(0.79*2*5 + 0.21*2*6)] : 26521541178535914242048000 moléculas en un metro cúbico de aire, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del nitrógeno molecular (79%) y oxígeno (21%) para obtener los electrones en un metro lineal
        25746383589.4834602095982075849351178030077195309528665478627, // agua [cbrt((10/3)*(10^28))*(8)] : 3.333x10^28 moléculas en un metro cúbico de agua, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del agua como molécula para obtener los electrones en un metro lineal
        81206048428.74334182814399729120705789177218382976918208, // madera [cbrt(2042797000000000000000000000)*(64)] : 2042797000000000000000000000 moléculas en un metro cúbico de madera ( 550 [kg/m^3] / 12.011*6+1.00784*10+15.999*5 [g/mol] * Número de Avogadro ), obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" de la celulosa C6H10O5 (4*6+10+6*5) como molécula para obtener los electrones en un metro lineal
        2683956227.13375630504705598630107878318735183748706240, // pantano [cbrt(26521541178535914242048000)*(0.5*8 + 0.5*10)] : 26521541178535914242048000 moléculas en un metro cúbico de aire, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del nitrógeno molecular (50%) y metano (50%) para obtener los electrones en un metro lineal
        100, // vacío interestelar [cbrt(10^6)*1] : 10^6 átomos de hidrógeno por metro cúbico de espacio interestelar, obtenemos su raíz cúbica y lo multiplicamos por el número de electrones "externos" del hidrógeno singular para obtener los electrones en un metro lineal
        1.0f, // negro ... valor por defecto
        0.0f // fin del arreglo ... aunque este no lo requiere, se escribe para mantener la consistencia con el arreglo de fondos
    };

    // mismo orden: aire, agua, madera, pantano, espacio, vacio, cero al final
    const float leeway_in_environment [] = { 0.24, 0.4, 0.19, 0.24, 0.025, 0.24, 0 }; // valores de pantano y espacio subject to change
    const float branch_in_environment [] = { 0.12, 0.05, 0.07, 0.1, 0.5, 0.12, 0 }; // valores de pantano y putricio subject to change
    const float weight_in_environment [] = { 0, -0.1, 0.27, -0.05, 0, 0, 0 };
    const float height_in_environment [] = { 0.75, 0.3, 0.95, 0.75, 0.5, 0.55, 0 };

    const int voidIndex = sizeof(environmental_factors)/sizeof(const float) - 2;

    float current_environmental_factor = environmental_factors[0];

    sf::Sprite background (*bg[bgIndex]);

    float bg_scale;

    auto scaleBG = [&] () {
        background.setScale(1.f, 1.f);
        bg_scale = (window->getSize().x > window->getSize().y ? window->getSize().x / (*bg[bgIndex]).getSize().x : window->getSize().y / (*bg[bgIndex]).getSize().y);
        bg_scale *= (MOBILE ? 1.5 : 1);
        if (bg_scale != 1.f) {
            background.setScale(bg_scale, bg_scale);
        }
        background.setPosition((window->getSize().x - ((*bg[bgIndex]).getSize().x * bg_scale))/2, (window->getSize().y - ((*bg[bgIndex]).getSize().y * bg_scale))/2);
    };

    scaleBG();

    Lightning storm;
    wstringstream thunder_data, thunder_physics_data, title_data;
    vector<thickLine> thunder; // crear el vector de vértices a renderizar
    Point ** grid = storm.getGrid();
    vector<float> * fracs = storm.getFracs();

    const float defaultLeeway = 0.24F;
    const float defaultBranch = 0.12F;

    sf::Vector3f centroid;
    auto t0 = std::chrono::system_clock::now();
    long double time;
    long double e_mass;
    float vf = 100000000.0F;
    float acceleration, force, delta_y, Ecf, work, Pf;

    float leeway = defaultLeeway;
    float branch = defaultBranch;
    float forcedHeight = 0.75F;
    float downWeight = 0;
    float fractalStep = 0;
    float crystallizate = 0;
    float humidity = 0.9F;
    float temperature = 15.0F;

    float lightning_width = 257;
    float lightning_height = 181;
    float lightning_scale;
    float lightning_color [3] = {245, 230, 83};
    float alignmentOffset;
    float* direction = nullptr;
    float x_rotation = 0.f;
    float y_rotation = 0.f;
    float z_rotation = 0.f;
    float spin_rads = 0.f;
    bool zap = false;
    bool linear_adjustment_line = false;
    bool switchingBG = false;
    bool attemptClose = false;
    bool hide_left = true;
    bool hide_right = true;
    bool isMobileLandscape = false;
    bool do_spin = false;

    int renderIndex = 0;
    int drawPile = 0;

    Slider
        * alignmentSlider = nullptr, 
        * fractalStepSlider = nullptr, 
        * redSlider = nullptr, 
        * greenSlider = nullptr, 
        * blueSlider = nullptr, 
        * crystallizateSlider = nullptr, 
        * humiditySlider = nullptr, 
        * temperatureSlider = nullptr, 
        * leewaySlider = nullptr, 
        * branchSlider = nullptr, 
        * downWeightSlider = nullptr, 
        * forcedHeightSlider = nullptr, 
        * envfactorSlider = nullptr,
        * xRotationSlider = nullptr,
        * yRotationSlider = nullptr,
        * zRotationSlider = nullptr;
    Button
        * zapping = nullptr,
        * backgroundButton = nullptr,
        * closeButton = nullptr;
    Switch
        * linear_adjustment_switch = nullptr,
        * hide_left_switch = nullptr,
        * hide_right_switch = nullptr,
        * spin_switch = nullptr;
    Achieve
        * chievo1 = nullptr,
        * chievo2 = nullptr;
    
    // colocar los deslizadores que recibirán eventos en grupo
    Slider ** all_sliders [] = {&alignmentSlider, &branchSlider, &leewaySlider, &redSlider, &greenSlider, &blueSlider, &envfactorSlider, &downWeightSlider, &forcedHeightSlider, &crystallizateSlider, &humiditySlider, &temperatureSlider, &fractalStepSlider, &xRotationSlider, &yRotationSlider, &zRotationSlider};
    // colocar los botones que recibirán eventos en grupo
    Button ** all_buttons [] = {&zapping, &backgroundButton, &closeButton};
    // colocar los interruptores que recibirán eventos en grupo
    Switch ** all_switches [] = {&linear_adjustment_switch, &hide_left_switch, &hide_right_switch, &spin_switch};
    // colocar los logros que recibirán eventos en grupo
    Achieve ** all_chievos [] = {&chievo1, &chievo2};

    bool achieve_vars [2] = {false};

    // agrupar y ejecutar los eventos correspondientes a los sliders
    auto UI_events = [&] (int type, sf::Vector2i *mouse = nullptr) {
        for (auto &i : all_sliders) {
            switch (type) {
                case 0: // cuando usuario clickea
                    (*i)->checkDragging(*mouse);
                break;
                case 1: // cuando usuario desclickea
                    (*i)->setIsDragging(false);
                break;
                case 2: // cuando hay que actualizar el estado ... NO SE USA PORQUE ALGUNOS EVENTOS REQUIEREN REDIBUJADO O TRATAMIENTO ESPECIAL
                    (*i)->updatePercentage(*mouse);
                break;
                case 3: // dibujar
                    (*i)->draw(*window);
                break;
                case -999:
                    delete (*i);
                break;
                default:
                break;
            }
        }
        for (auto &i : all_buttons) {
            switch (type) {
                case 0: // cuando usuario clickea
                    (*i)->checkClicking(*mouse);
                break;
                case 1: // cuando usuario desclickea
                    (*i)->setIsClicking(false);
                break;
                case 2: // cuando hay que actualizar el estado ... NO SE USA PORQUE ALGUNOS EVENTOS REQUIEREN REDIBUJADO O TRATAMIENTO ESPECIAL
                    (*i)->updateState();
                break;
                case 3: // dibujar
                    (*i)->draw(*window);
                break;
                case -999:
                    delete (*i);
                break;
                default:
                break;
            }
        }
        for (auto &i : all_switches) {
            switch (type) {
                case 0: // cuando usuario clickea
                    (*i)->checkClicking(*mouse);
                break;
                case 1: // cuando usuario desclickea
                    (*i)->setIsClicking(false);
                break;
                case 2: // cuando hay que actualizar el estado ... NO SE USA PORQUE ALGUNOS EVENTOS REQUIEREN REDIBUJADO O TRATAMIENTO ESPECIAL
                    (*i)->updateState();
                break;
                case 3: // dibujar
                    (*i)->draw(*window);
                break;
                case -999:
                    delete (*i);
                break;
                default:
                break;
            }
        }
        int j = 0;
        for (auto &i : all_chievos) {
            switch (type) {
                case 3: // dibujar
                    (*i)->draw(*window);
                break;
                case -999:
                    delete (*i);
                break;
                default:
                break;
            }
        }
    };

    auto leftMenuState = [&] () {
        if (hide_left) {
            left_menu_bg.setPosition(sf::Vector2f(0, window->getSize().y + 1));
            hide_left_switch->changePosition(0, hide_left_switch->getPosition().y);
        }
        else {
            left_menu_bg.setPosition(sf::Vector2f(0, 0));
            if (!MOBILE) hide_left_switch->changePosition(left_menu_bg.getSize().x, hide_left_switch->getPosition().y);
        }
        if (MOBILE && !hide_right) {
            hide_left_switch->changePosition(-hide_left_switch->getSize().x, hide_left_switch->getPosition().y);
        }
    };

    auto rightMenuState = [&] () {
        if (hide_right) {
            right_menu_bg.setPosition(sf::Vector2f(window->getSize().x-right_menu_bg.getSize().x, window->getSize().y + 1));
            hide_right_switch->changePosition(window->getSize().x - hide_right_switch->getSize().x, hide_right_switch->getPosition().y);
        }
        else {
            right_menu_bg.setPosition(sf::Vector2f(window->getSize().x-right_menu_bg.getSize().x, 0));
            if (!MOBILE) hide_right_switch->changePosition(window->getSize().x - right_menu_bg.getSize().x - hide_right_switch->getSize().x, hide_right_switch->getPosition().y);
        }
        if (MOBILE && !hide_left) {
            hide_right_switch->changePosition(window->getSize().x + hide_right_switch->getSize().x, hide_right_switch->getPosition().y);
        }
    };

    auto init_ui = [&] () {
        isMobileLandscape = MOBILE && (window->getSize().x > window->getSize().y);
        splash_screen.setPosition(sf::Vector2f((window->getSize().x - splash_screen.getLocalBounds().width)/2, (window->getSize().y - splash_screen.getLocalBounds().height)/2));
        splash_3d.setPosition(sf::Vector2f(splash_screen.getPosition().x + splash_screen.getLocalBounds().width * (27.f/32.f) - (splash_3d.getLocalBounds().width * splash_3d.getScale().x)/2, splash_screen.getPosition().y + splash_screen.getLocalBounds().height * (7.f/8.f) - (splash_3d.getLocalBounds().height * splash_3d.getScale().y)/2));
        watermark_logo.setPosition(window->getSize().x - (watermark_logo.getLocalBounds().getSize().x * watermark_logo.getScale().x + (MOBILE ? 40 : 10)), window->getSize().y - (watermark_logo.getLocalBounds().getSize().y * watermark_logo.getScale().y + (MOBILE ? 40 : 10)));
        watermarkText.setPosition((MOBILE ? 40 : 10), window->getSize().y - (watermarkText.getLocalBounds().getSize().y + (MOBILE ? 40 : 10)));
        left_menu_bg.setSize(sf::Vector2f(window->getSize().x * (MOBILE ? 1.f : 0.2f), window->getSize().y));
        right_menu_bg.setSize(sf::Vector2f(window->getSize().x * (MOBILE ? 1.f : 0.2f), window->getSize().y));
        UI_events(-999); // delete all UI elements
        lightning_scale = window->getSize().y/lightning_height;
        alignmentOffset = (window->getSize().x - lightning_width*lightning_scale)/2;
        // inicializar interfaz
        const float left_slider_x_pos = left_menu_bg.getPosition().x + left_menu_bg.getSize().x*(1.f/6.f);
        const float left_slider_x_size = left_menu_bg.getSize().x*(2.f/3.f);
        const float left_slider_y_size = left_menu_bg.getSize().y*(1.f/135.f);
        const float handle_x_size = (MOBILE ? 120 : 10);
        const float handle_y_size = (MOBILE ? 40 : 20);
        // para los deslizadores, estamos usando de posición (left_slider_x_pos, window->getSize().y * [-0.06 respecto al que está por debajo]f)
        // deslizadores constantes
        bool shouldInvertAlignment = alignmentOffset < 0;
        alignmentSlider = new Slider (alignmentOffset, (shouldInvertAlignment ? window->getSize().x - lightning_width*lightning_scale : 0), (shouldInvertAlignment ? 0 : window->getSize().x - lightning_width*lightning_scale), left_slider_x_pos, window->getSize().y * 0.78f, 2, font, L"Alineación", false, sf::Color::Black, sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left);
        fractalStepSlider = new Slider (fractalStep, 1.0f, 2.0f, window->getSize().x - right_menu_bg.getSize().x*(5.f/6.f), (isMobileLandscape ? right_menu_bg.getPosition().y + right_menu_bg.getSize().y*(1.f/6.f) : window->getSize().y * 0.39f), 0, font, L"Términos de MacLaurin", true, sf::Color(217, 189, 165), sf::Color::White, right_menu_bg.getSize().x*(2.f/3.f), right_menu_bg.getSize().y*(1.f/135.f), handle_x_size, handle_y_size, &hide_right);
        redSlider = new Slider (lightning_color[0], 0.0f, 255.0f, left_slider_x_pos, window->getSize().y * 0.07f, 2, font, L"Matiz", true, sf::Color::Red, sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left);
        greenSlider = new Slider (lightning_color[1], 0.0f, 255.0f, left_slider_x_pos, window->getSize().y * 0.09f, 3, font, std::wstring(), true, sf::Color::Green, sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left);
        blueSlider = new Slider (lightning_color[2], 0.0f, 255.0f, left_slider_x_pos, window->getSize().y * 0.11f, 3, font, std::wstring(), true, sf::Color::Blue, sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left);
        xRotationSlider = new Slider (x_rotation, 0.f, 2*Physics::PI, left_slider_x_pos, window->getSize().y * 0.825f, 2, font, L"Rotación", true, sf::Color(240, 100, 100), sf::Color::White, left_slider_x_size/2, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return !do_spin; });
        yRotationSlider = new Slider (y_rotation, 0.f, 2*Physics::PI, left_slider_x_pos, window->getSize().y * 0.845f, 3, font, std::wstring(), true, sf::Color(100, 240, 100), sf::Color::White, left_slider_x_size/2, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return !do_spin; });
        zRotationSlider = new Slider (z_rotation, 0.f, 2*Physics::PI, left_slider_x_pos, window->getSize().y * 0.865f, 3, font, std::wstring(), true, sf::Color(100, 100, 240), sf::Color::White, left_slider_x_size/2, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return !do_spin; });
        // deslizadores de aire
        crystallizateSlider = new Slider (crystallizate, 0.0f, 0.16f, left_slider_x_pos, window->getSize().y*0.22f, 0, font, L"Cristalización (σ)", false, sf::Color(128, 210, 255), sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == 0; });
        humiditySlider = new Slider (humidity, 0.6f, 1.2f, left_slider_x_pos, window->getSize().y*0.29f, 0, font, L"Humedad", false, sf::Color(9, 232, 128), sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == 0; });
        // deslizadores de agua
        temperatureSlider = new Slider (temperature, 0.0f, 30.0f, left_slider_x_pos, window->getSize().y*0.36f, 0, font, L"Temperatura (°C)", true, sf::Color(255, 142, 56), sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == 1; });
        // deslizadores de vacío
        leewaySlider = new Slider (leeway, 0.0f, 0.5f, left_slider_x_pos, window->getSize().y*0.43f, 0, font, L"Libertad de acción", false, sf::Color::Cyan, sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == voidIndex; });
        branchSlider = new Slider (branch, 0.0f, 0.5f, left_slider_x_pos, window->getSize().y*0.50f, 0, font, L"Bifurcación", false, sf::Color::Magenta, sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == voidIndex; });
        downWeightSlider = new Slider (downWeight, -0.4f, 0.4f, left_slider_x_pos, window->getSize().y*0.57f, 0, font, L"Conductividad vertical", false, sf::Color(104, 139, 204), sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == voidIndex; });
        forcedHeightSlider = new Slider (forcedHeight, 0.15f, 0.95f, left_slider_x_pos, window->getSize().y*0.64f, 0, font, L"Altura mínima", false, sf::Color(104, 200, 204), sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == voidIndex; });
        envfactorSlider = new Slider (current_environmental_factor, 1, 10000000000, left_slider_x_pos, window->getSize().y*0.71f, 0, font, L"Electrones por metro de alcance", true, sf::Color::Yellow, sf::Color::White, left_slider_x_size, left_slider_y_size, handle_x_size, handle_y_size, &hide_left, [&] () { return bgIndex == voidIndex; });
        // botones
        zapping = new Button (zap, left_menu_bg.getSize().x*(7.f/12.f), window->getSize().y*0.93f, font, L"Generar", left_menu_bg.getSize().x*(3.f/12.f), left_menu_bg.getSize().y*(0.75f/21.6f), sf::Color(47,45,194), sf::Color(67,65,224), sf::Color::White, &hide_left);
        backgroundButton = new Button (switchingBG, left_menu_bg.getSize().x*(1.f/6.f), window->getSize().y*0.93f, font, L"Cambiar entorno", left_menu_bg.getSize().x*(5.f/12.f), left_menu_bg.getSize().y*(0.75f/21.6f), sf::Color(179, 125, 46), sf::Color(252, 210, 146), sf::Color::White, &hide_left);
        closeButton = new Button (attemptClose, window->getSize().x-(MOBILE ? 150 : 75), 0, font, L"X", (MOBILE ? 150 : 75), (MOBILE ? 100 : 50), sf::Color::Red, sf::Color::Red, sf::Color::White);
        // interruptores
        linear_adjustment_switch = new Switch (linear_adjustment_line, left_menu_bg.getSize().x*(1.f/6.f), window->getSize().y*0.88f, font, L"Mostrar ajuste lineal", L"Ocultar ajuste lineal", left_slider_x_size, left_menu_bg.getSize().y*(1.f/21.6f), sf::Color(0, 84, 46), sf::Color(84, 0, 14), sf::Color::White, &hide_left);
        spin_switch = new Switch (do_spin, left_slider_x_pos + left_slider_x_size/2, window->getSize().y*0.825f, font, L"Giro automático", L"Giro manual", left_slider_x_size/2, left_menu_bg.getSize().y*(0.75f/21.6f), sf::Color(100, 240, 100), sf::Color(240, 100, 240), sf::Color::White, &hide_left);
        hide_left_switch = new Switch (hide_left, 0, window->getSize().y*0.375f, font, L"<", L">", (MOBILE ? 100 : 50), window->getSize().y*0.25f, sf::Color(90, 90, 90, 90), sf::Color(90, 90, 90, 90), sf::Color::White);
        hide_right_switch = new Switch (hide_right, window->getSize().x - (MOBILE ? 100 : 50), window->getSize().y*0.375f, font, L">", L"<", (MOBILE ? 100 : 50), window->getSize().y*0.25f, sf::Color(90, 90, 90, 90), sf::Color(90, 90, 90, 90), sf::Color::White);

        // logros
        chievo1 = new Achieve (drawPile, window->getSize().x*0.4f, window->getSize().y*0.9f, watermark_texture, font, L"Genera tu primer rayo", window->getSize().x*0.2f, window->getSize().y*0.08f, 5000, [&] () { return achieve_vars[0]; });
        chievo2 = new Achieve (drawPile, window->getSize().x*0.4f, window->getSize().y*0.9f, watermark_texture, font, L"Genera tu segundo rayo", window->getSize().x*0.2f, window->getSize().y*0.08f, 5000, [&] () { return achieve_vars[1]; });

        leftMenuState();
        rightMenuState();
    };
    
    init_ui();

    // función lambda que permite trabajar con las variables de main () por referencia
    // por lo que se llama sin parámetros

    auto retypeInfo = [&] () {
        acceleration = Physics::mean_a(0, vf, time);
        e_mass = storm.getElectronicMass(current_environmental_factor);
        force = Physics::F(e_mass, acceleration);
        delta_y = Physics::delta_x(acceleration, time);
        Ecf = Physics::Ec(e_mass, vf);
        work = Physics::T(force, delta_y);
        Pf = Physics::P(force, vf);

        thunder_data.str(std::wstring());
        thunder_physics_data.str(std::wstring());
        title_data.str(std::wstring());
        console.str(std::wstring());
        thunder_data << "Ramas: " << storm.getN() << endl
        << "Electrones involucrados:\n\t" << storm.getInvolvedElectrons(current_environmental_factor) << endl
        << L"Masa electrónica total:\n\t" << scientific << setprecision(std::numeric_limits<long double>::digits10 + 1) << e_mass << "kg" << endl
        << L"Ajuste de mínimos cuadrados:\n\tx = " << fixed << setprecision(4) << direction[1] << " " << (direction[0] > 0 ? "+" : "-") << " " << fixed << setprecision(4) << abs(direction[0]) << "y" << endl
        << L"Coeficiente de correlación (R): " << fixed << setprecision(4) << direction[2] << endl
        << L"Coeficiente de determinación (R²): " << fixed << setprecision(4) << direction[2]*direction[2] << endl
        << L"Dimensión fractal: " << fixed << setprecision(6) << (*(fracs))[floor(fractalStep+0.5) - 1] << endl;

        thunder_physics_data << scientific << setprecision(4)
        << L"W = " << e_mass << L"kg·9.81 m/s²\n\t= " << Physics::W(e_mass) << L"N" << endl
        << L"t = " << time << L"s" << endl
        << L"v = " << vf << L"m/s" << endl
        << L"a = " << L"(" << vf << L"m/s - 0) / " << time << L"s\n\t= " << acceleration << L"m/s²" << endl
        << L"F = " << e_mass << L"kg·" << acceleration << L"m/s²\n\t= " << force << L"N" << endl
        << L"Δy = " << L"(" << acceleration << L"m/s²·(" << time << L"s)²)/2\n\t\t+ 0m/s·(" << time << L"s) + 0m\n\t= " << delta_y << L"m" << endl
        << L"T = " << force << L"N·" << delta_y << L"m\n\t= " << work << L"J" << endl
        << L"Ec₁ = " << L"(" << e_mass << L"kg·(" << vf << L"m/s)²)/2\n\t= " << Ecf << L"J" << endl
        << L"P = " << force << L"N·" << vf << L"m/s\n\t= " << Pf << L"kgm/s" << endl;

        title_data << bgTitle[bgIndex];

        format(thunder_data);
        format(thunder_physics_data);
        format(title_data);
        
        text.setString(thunder_data.str());
        currentTitle.setString(title_data.str());
        physicsOutput.setString(thunder_physics_data.str() + (console.str().empty() ? L"" : L"\n\nCONSOLA\n" + console.str()));

        physicsOutput.setPosition(right_menu_bg.getPosition().x + (right_menu_bg.getSize().x - physicsOutput.getLocalBounds().getSize().x)/2, window->getSize().y*0.45);
        text.setPosition(right_menu_bg.getPosition().x + (right_menu_bg.getSize().x - text.getLocalBounds().getSize().x)/2, window->getSize().y * 0.15);
        currentTitle.setPosition((MOBILE ? (left_menu_bg.getSize().x - currentTitle.getLocalBounds().getSize().x)/2 : left_menu_bg.getSize().x + 20), window->getSize().y*0.01);
        
        dim_text_bg.setSize(sf::Vector2f(text.getLocalBounds().getSize().x + 10, text.getLocalBounds().getSize().y + 15));
        dim_physicsOutput_bg.setSize(sf::Vector2f(physicsOutput.getLocalBounds().getSize().x + 10, physicsOutput.getLocalBounds().getSize().y + 15));

        // this shit is cluttering itself because fuck i know -- fixed by adding a boolean flag isMobileLandscape
        if (isMobileLandscape) {
            const float offset_x = right_menu_bg.getPosition().x + ((right_menu_bg.getSize().x - (dim_text_bg.getSize().x + dim_physicsOutput_bg.getSize().x + 10))/2.f);
            const float offset_y = right_menu_bg.getPosition().y + right_menu_bg.getSize().y*(1.f/6.f) + right_menu_bg.getSize().y*(1.f/135.f) + 50;
            text.setPosition(offset_x + 5, offset_y + 5);
            physicsOutput.setPosition(offset_x + dim_text_bg.getSize().x + 10 + 5, offset_y + 5);
        }

        dim_text_bg.setPosition(text.getPosition().x - 5, text.getPosition().y - 5);
        dim_physicsOutput_bg.setPosition(physicsOutput.getPosition().x - 5, physicsOutput.getPosition().y - 5);
    };

    auto recalculateLightningVertex = [&] (bool skipRedraw = false) {
        thunder.clear();
        centroid.x = 0.0f;
        centroid.y = 0.0f;
        centroid.z = 0.0f;
        int n_points = 0;
        const int z_index = 0;
        for (int i = storm.getHei()-1; i >= 0; i--) {
            for (int j = storm.getWid()-1; j >= 0; j--) {
                if (grid[i][j].getIsLight()) {
                    const int i0 = grid[i][j].getPrevX();
                    const int j0 = grid[i][j].getPrevY();
                    const float start_x = alignmentOffset + j*lightning_scale;
                    const float start_y = i*lightning_scale + 1;
                    const float end_x = alignmentOffset + j0*lightning_scale;
                    const float end_y = i0*lightning_scale + 1;
                    centroid.x += start_x + end_x;
                    centroid.y += start_y + end_y;
                    centroid.z += z_index;
                    n_points += 2;
                    thunder.emplace_back(sf::Vector3f(start_x, start_y, z_index), sf::Vector3f(end_x, end_y, z_index), sf::Color(lightning_color[0], lightning_color[1], lightning_color[2]), 2.f);
                    thunder.emplace_back(sf::Vector3f(start_x + (j-j0)*2, start_y + (i-i0)*2, z_index), sf::Vector3f(end_x, end_y, z_index), sf::Color(lightning_color[0], lightning_color[1], lightning_color[2], 64), 4.f);
                    thunder.emplace_back(sf::Vector3f(start_x + (j-j0), start_y + (i-i0), z_index), sf::Vector3f(end_x, end_y, z_index), sf::Color(lightning_color[0], lightning_color[1], lightning_color[2], 64), 6.f);
                }
            }
        }
        centroid.x /= n_points;
        centroid.y /= n_points;
        centroid.z /= n_points;
        if (linear_adjustment_line) {
            thunder.emplace_back(sf::Vector3f(alignmentOffset + direction[1]*lightning_scale, 1, z_index), sf::Vector3f(alignmentOffset + (lightning_height*direction[0] + direction[1])*lightning_scale, lightning_height*lightning_scale + 1, z_index), sf::Color(255 - lightning_color[0], 255 - lightning_color[1], 255 - lightning_color[2]), 2.f);
            thunder.emplace_back(centroid - sf::Vector3f(0, 5, 0), centroid + sf::Vector3f(0, 5, 0), sf::Color::Red, 32.f); // centro geométrico del rayo
        }
        reverse(thunder.begin(), thunder.end());
        if (!skipRedraw) renderIndex = 0;
        else renderIndex = thunder.size();
    };

    auto generateLightning = [&] () {
        if (direction != nullptr) delete [] direction; // liberar memoria usada por el direction previo
        for (int i = 0; i < storm.getHei(); i++) {
            delete [] grid[i]; // liberar memoria usada en el rayo anterior
        }
        delete [] grid;
        fracs->clear();
        storm = Lightning(lightning_height, lightning_width, leeway-(crystallizate*0.15625f)+((humidity-0.9)*0.0416f), branch-(crystallizate*0.3125f)+(temperature*0.00066f), downWeight+(crystallizate*humidity), forcedHeight+((temperature-15)*0.02f));
        grid = storm.getGrid();
        fracs = storm.getFracs();
        storm.randomize(); // aleatorizar los valores resistivos en el entorno
        t0 = std::chrono::system_clock::now();
        storm.superTraverse(); 
        time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - t0).count() * 0.000000001 * 0.05; // * 0.000000001 (ns -> s) * 0.05 ajuste manual (rayo >>> pc)
        direction = storm.directionComp();
        storm.fractalComp();
        fractalStep = fracs->size();
        fractalStepSlider->setUpperBound(fractalStep);
        recalculateLightningVertex();
        retypeInfo();
    };

    generateLightning();

    auto start_time = std::chrono::system_clock::now();
    auto start_fps = start_time;
    auto current_timestamp = start_time;
    int64_t elapsed = 0;
    bool yetToBoot = true;
    int zapCount = 0;

    startup.play();

    while (window->isOpen())
    {

        sf::Event event;
        
        while (window->pollEvent(event))
        {
            if (event.type == sf::Event::Closed)
                window->close();
            if (event.type == sf::Event::Resized)
            {
                // la ventana se reajustó (probablemente cambió de orientación en Android/iOS)
                window->setView(sf::View(sf::FloatRect(0, 0, event.size.width, event.size.height)));
                scaleBG();
                init_ui();
                recalculateLightningVertex();
                retypeInfo();
            }
        }

        if (yetToBoot) {
            current_timestamp = std::chrono::system_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - start_time).count();
            if (elapsed < 1000) {
                background.setColor(sf::Color((255.f * ((float) elapsed)/1000.f), (255.f * ((float) elapsed)/1000.f), (255.f * ((float) elapsed)/1000.f)));
                loading_percentage.setSize(sf::Vector2f(500.0f * ((float) elapsed/1000.0f), 5));
                loading_percentage.setPosition(sf::Vector2f((window->getSize().x - loading_percentage.getLocalBounds().width)/2, splash_screen.getLocalBounds().height + (window->getSize().y - splash_screen.getLocalBounds().height)/2));
                loading_text.setPosition(sf::Vector2f((window->getSize().x - loading_text.getLocalBounds().width)/2, loading_percentage.getPosition().y + 10));
                elapsed >= 500 ? splash_3d.setColor(sf::Color(255, 255, 255, 255.f * (elapsed - 500.f) / 500.f)) : splash_3d.setColor(sf::Color(255, 255, 255, 0));
                window->clear();
                window->draw(background);
                window->draw(splash_screen);
                window->draw(splash_3d);
                window->draw(loading_percentage);
                window->draw(loading_text);
                window->display();
                continue;
            }
            else {
                yetToBoot = false;
            }
        }

        if (attemptClose) {
            current_timestamp = std::chrono::system_clock::now();
            elapsed = std::chrono::duration_cast<std::chrono::milliseconds>(current_timestamp - start_time).count();
            if (elapsed < 500) {
                background.setColor(sf::Color(255-(255.f * ((float) elapsed)/500.f), 255-(255.f * ((float) elapsed)/500.f), 255-(255.f * ((float) elapsed)/500.f)));
                window->clear();
                window->draw(background);
                window->draw(splash_screen);
                window->display();
                continue;
            }
            else {
                window->close();
            }
        }

        // renderizar un solo punto a la vez (hasta 120)
        if (renderIndex+1 <= thunder.size()) renderIndex++;
        if (renderIndex > 120 && renderIndex < thunder.size()) renderIndex = thunder.size();
        if (do_spin) {
            if (spin_rads < 2*Physics::PI && spin_rads >= 0) spin_rads += Physics::PI/720;
            else {
                spin_rads = 0;
            }
        }

        sf::Vector2i mousepos_update;

        if (sf::Touch::isDown(0)) {
            mousepos_update = sf::Touch::getPosition(0, *window);
        }
        else {
            mousepos_update = sf::Mouse::getPosition(*window);
        }

        // si el usuario está haciendo click izquierdo o tocando
        if(event.type == sf::Event::MouseButtonPressed && event.mouseButton.button == sf::Mouse::Left || event.type == sf::Event::TouchBegan){
            UI_events(0, &mousepos_update);            
        }

        // si el usuario deja de mantener click izquierdo o de tocar 
        else if(event.type == sf::Event::MouseButtonReleased && event.mouseButton.button == sf::Mouse::Left || event.type == sf::Event::TouchEnded){
            UI_events(1);
        }

        if (backgroundButton->updateState() && switchingBG) {
            bgIndex++;
            if (bg[bgIndex] == nullptr) bgIndex = 0;
            background.setTexture(*bg[bgIndex]);

            scaleBG();

            current_environmental_factor = environmental_factors[bgIndex];
            leeway = leeway_in_environment[bgIndex];
            branch = branch_in_environment[bgIndex];
            downWeight = weight_in_environment[bgIndex];
            forcedHeight = height_in_environment[bgIndex];
            crystallizate = 0;
            humidity = 0.9F;
            temperature = 15.0F;
        }

        if (envfactorSlider->updatePercentage(mousepos_update) || fractalStepSlider->updatePercentage(mousepos_update)) {
            retypeInfo();
        }
        
        if (linear_adjustment_switch->updateState() || alignmentSlider->updatePercentage(mousepos_update) || redSlider->updatePercentage(mousepos_update) || greenSlider->updatePercentage(mousepos_update) || blueSlider->updatePercentage(mousepos_update)) {
            recalculateLightningVertex(true);
        }
        leewaySlider->updatePercentage(mousepos_update);
        branchSlider->updatePercentage(mousepos_update);
        downWeightSlider->updatePercentage(mousepos_update);
        forcedHeightSlider->updatePercentage(mousepos_update);
        crystallizateSlider->updatePercentage(mousepos_update);
        humiditySlider->updatePercentage(mousepos_update);
        temperatureSlider->updatePercentage(mousepos_update);
        xRotationSlider->updatePercentage(mousepos_update);
        yRotationSlider->updatePercentage(mousepos_update);
        zRotationSlider->updatePercentage(mousepos_update);

        if (spin_switch->updateState() && do_spin) {
            x_rotation = 0.f;
            y_rotation = 0.f;
            z_rotation = 0.f;
            spin_rads = 0.f;
        }

        if (hide_left_switch->updateState() || hide_right_switch->updateState()) {
            leftMenuState();
            rightMenuState();
        }

        if (closeButton->updateState() && attemptClose) {
            start_time = std::chrono::system_clock::now();
        }
        
        if (zapping->updateState() && zap){
            generateLightning();
            zapCount++;
            if (zapCount == 1) achieve_vars[0] = true;
            if (zapCount == 2) achieve_vars[1] = true; // the actual numbers should be more like 1, 15, 50, 100, or something. this is just a test
            window->clear(sf::Color::White);
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

        window->clear();
        window->draw(background);

        for (int i = 0; i < renderIndex; i++) {
            thunder.at(i).draw(*window, &centroid, (do_spin ? 0 : x_rotation), (do_spin ? spin_rads : y_rotation), (do_spin ? 0 : z_rotation));
        }

        window->draw(left_menu_bg);
        window->draw(right_menu_bg);

        if (!hide_right) {
            window->draw(dim_text_bg);
            window->draw(dim_physicsOutput_bg);
            window->draw(text);
            window->draw(physicsOutput);
        }
        else if (!MOBILE || hide_left) {
            window->draw(watermark_logo);
        }

        if (!hide_left) {
            window->draw(currentTitle);
        }
        else if (!MOBILE || hide_right) {
            window->draw(watermarkText);
        }
        UI_events(3);
        window->display();
    }
    // liberar memoria
    UI_events(-999); // delete all UI elements
    delete [] direction;
    // delete fracs somehow
    delete window;
    return 0;
}