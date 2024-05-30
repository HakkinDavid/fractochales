#ifndef UTILS_H
#define UTILS_H

#include <iostream>
#include <string>
#include <sstream>
#include <iomanip>
#include <regex>
#include "../SFML/Graphics.hpp"
#include "../Slider/slider.h"
#include "../Switch/switch.h"
#include "../Button/button.h"
#include "../Achieve/achieve.h"

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

namespace utils {
    const std::wstring advice_text_es[] = {
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
        L"¿Sabías que tenemos " + std::to_wstring(VERSION) + L" contribuciones de código en este programa?",
        L"En la práctica, ningún rayo es idéntico",
        L"Fractochales funciona en Windows, Mac, Linux, Android y iOS",
        L"Hirales estaría orgulloso de nuestras proyecciones 3D sobre un plano 2D",
        L"Técnicamente emulamos el trazado de rayos",
        L"Mi mamá es un vector ortogonal",
        L"Fractochales es la contracción de Fractales Tochos",
        L"En cada relámpago simulamos cerca de 3 billones de electrones, a escala macroscópica y en 3D"
    };

    const std::wstring advice_text_en[] = {
        L"Loading clouds",
        L"Asking The Weather Channel",
        L"Everybody gangsta",
        L"Removing Casta's third name",
        L"Checking our lightning rods",
        L"My mom is a fractal figure",
        L"A lightning may reach 100,000 km/s",
        L"There's the lightning, before the thunder",
        L"We perform stoichiometric calculations to obtain the masses of a given simulated lightning",
        L"A fractal figure lies between two whole dimensions",
        L"Did you know? Our project has received " + std::to_wstring(VERSION) + L" code commits so far",
        L"In practice, no lightning is identical",
        L"Fractochales works across Windows, Mac, Linux, Android & iOS",
        L"We generate 3D projections over a 2D plane",
        L"This is (technically) raytracing",
        L"My mom is an orthogonal vector",
        L"Fractochales is the contraction of \"silly fractals\" in Spanish",
        L"In each lightning bolt we simulate about 3 trillion electrons, on a macroscopic scale and in 3D"
    };

    const wstring bgTitle_es [] = {L"Aire", L"Agua", L"Madera", L"Pantano", L"Espacio", L"Libre", L""};
    const wstring bgTitle_en [] = {L"Air", L"Water", L"Wood", L"Swamp", L"Space", L"Free mode", L""};
    
    const std::wstring ui_es [] = {
        L"Matiz",
        L"Rotación",
        L"Cristalización (σ)",
        L"Humedad",
        L"Temperatura (°C)",
        L"Libertad de acción",
        L"Bifurcación",
        L"Conductividad vertical",
        L"Altura mínima",
        L"Electrones por metro de alcance",
        L"Generar",
        L"Exportar OBJ",
        L"Cambiar entorno",
        L"Cambiar idioma",
        L"Giro automático",
        L"Giro manual",
        L"Generando...",
        L"Ramas",
        L"Electrones involucrados",
        L"Masa electrónica total",
        L"Cámara",
        L"FPS",
        L"Control conectado"
    };

    const std::wstring ui_en [] = {
        L"Hue",
        L"Rotation",
        L"Crystallization (σ)",
        L"Humidity",
        L"Temperature (°C)",
        L"Freedom of action",
        L"Bifurcation",
        L"Vertical conductivity",
        L"Minimum height",
        L"Electrons per meter range",
        L"Generate",
        L"Export OBJ",
        L"Change environment",
        L"Change language",
        L"Auto rotation",
        L"Manual rotation",
        L"Generating...",
        L"Branches",
        L"Electrons involved",
        L"Total electron mass",
        L"Camera",
        L"FPS",
        L"Connected controller"
    };

    const int advice_max_num = sizeof(utils::advice_text_es) / sizeof(std::wstring);

    const int max_languages = 2;

    const std::wstring & getRandomAdvice();

    const std::wstring & getBGTitle (int index);

    const std::wstring & ui_text (int index);

    extern int new_obj_index;

    const wchar_t super [10] = {
        L'⁰', L'¹', L'²', L'³', L'⁴', L'⁵', L'⁶', L'⁷', L'⁸', L'⁹'
    };

    const std::string export_types [3] = {
        "obj", "mtl", "txt"
    };

    const std::wstring export_types_localized [3] = {
        L"OBJ", L"MTL", L"METADATA"
    };

    const std::string newLightningFileName (const int type = 0);

    std::wstring getFileHeader (const int type = 0);

    std::wstring to_super (std::wstring num);

    void format_wstringstream (std::wstringstream & s);

    void scaleBG (sf::RenderTarget * window, sf::Sprite & background, sf::Texture ** bg, int & bgIndex, float & bg_scale);

    extern int settings_fields [1];

    const int settings_max_num = sizeof(utils::settings_fields) / sizeof(int);

    void load_settings (std::wstringstream & settings_stream);

    void write_settings (std::wstringstream & settings_stream);
};

#endif