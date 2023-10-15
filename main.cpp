#include <iostream>
#include <conio.h>
#include "Lightning/Lightning.h"

int main()
{
    Lightning rasho;
    rasho.randomize();
    rasho.traverse(0, 0);
    rasho.show();

    
    cout << "Presiona cualquier tecla para salir." << endl;
    _getch();
    return 0;
}