#ifndef CONTROLLER_H
#define CONTROLLER_H

#include "../SFML/Window.hpp"
#include <string>

enum DS5 {
    SQUARE,
    CROSS,
    CIRCLE,
    TRIANGLE,
    L1,
    R1,
    L2,
    R2,
    CREATE,
    OPTIONS,
    L3,
    R3,
    PS,
    PADBUTTON
};

class Controller : public sf::Joystick {
    private:
        enum ID {
            DS5 = 3302
        };
    public:
        static std::wstring getName(short int index);
        static bool isPressed(short int button, short int index = 0);
};

#endif