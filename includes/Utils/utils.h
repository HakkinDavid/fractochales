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
    const std::wstring advice_text[] = {
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

    const int advice_max_num = sizeof(utils::advice_text) / sizeof(std::wstring);

    const std::wstring & getRandomAdvice();

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
};

#endif