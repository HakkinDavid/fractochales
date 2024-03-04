#include "controller.h"

std::wstring Controller :: getName (short int index) {
    int id = sf::Joystick::getIdentification(index).productId;
    switch (id)
    {
    case ID::DS5:
        return L"DualSense Controller";
        break;
    default:
        return L"Unrecognized";
        break;
    }
}

bool Controller :: isPressed (short int button, short int index) {
    return sf::Joystick::isButtonPressed(index, button);
}