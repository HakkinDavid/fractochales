#include <iostream>
#include <chrono>
#include "includes/Lightning/Lightning.h"
#include <fstream>

int main () {
    Lightning storm;
    vector<array<int, 3>> * canonVertices = storm.getCanonVertices();
    vector<float> * fracs = storm.getFracs();
    const float defaultLeeway = 0.24F;
    const float defaultBranch = 0.12F;

    auto t0 = std::chrono::system_clock::now();
    long double time;
    long double e_mass;
    float vf = 100000000.0F;
    float acceleration, force, delta_y, Ecf, work, Pf;

    float leeway = defaultLeeway;
    float branch = defaultBranch;
    float forcedHeight = 0.75F;
    float downWeight = 0;
    float crystallizate = 0;
    float humidity = 0.9F;
    float temperature = 15.0F;

    float lightning_width = 257;
    float lightning_height = 181;
    float lightning_depth = 181;

    float* direction = nullptr;

    std::cout << "Iniciando modo de depuración para Fractochales. Incluyendo Lightning.cpp";
    
    std::cout << " ✅\nRegenerando objeto tipo Lightning";
    storm = Lightning(lightning_height, lightning_width, lightning_depth, leeway-(crystallizate*0.15625f)+((humidity-0.9)*0.0416f), branch-(crystallizate*0.3125f)+(temperature*0.00066f), downWeight+(crystallizate*humidity), forcedHeight+((temperature-15)*0.02f));
    std::cout << " ✅\nObteniendo el puntero a los vértices canónicos";
    canonVertices = storm.getCanonVertices();
    std::cout << " ✅\nObteniendo el puntero a los cálculos fractales";
    fracs = storm.getFracs();
    std::cout << " ✅\nEjecutando randomize en el objeto tipo Lightning";
    storm.randomize(); // aleatorizar los valores resistivos en el entorno
    std::cout << " ✅\nEstableciendo tiempo de ejecución en 0";
    t0 = std::chrono::system_clock::now();
    std::cout << " ✅\nEjecutando superTraverse";
    storm.superTraverse(); 
    std::cout << " ✅\nEstableciendo el tiempo final de ejecución";
    time = std::chrono::duration_cast<std::chrono::nanoseconds>(std::chrono::system_clock::now() - t0).count() * 0.000000001 * 0.05; // * 0.000000001 (ns -> s) * 0.05 ajuste manual (rayo >>> pc)
    std::cout << " ✅\nObteniendo el puntero a la dirección del rayo";
    direction = storm.directionComp();
    std::cout << " ✅\nComputando dimensión fractal";
    storm.fractalComp();
    std::cout << " ✅\nImprimiendo canonVertices";
    std::ofstream output("outputtest.txt");
    for (int i=0; i<canonVertices->size(); i+=2) {
        output << " " << canonVertices->at(i)[0] << " " << canonVertices->at(i)[1] << " " << canonVertices->at(i)[2] << "; " << canonVertices->at(i+1)[0] << " " << canonVertices->at(i+1)[1] << " " << canonVertices->at(i+1)[2] << "\n";
    }
    output.close();
    std::cout << " ✅";
}