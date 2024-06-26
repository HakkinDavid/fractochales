// Header de librería Lightning 
// Autor: Mauricio Alcántar Dueñas
// Fecha: 6 de marzo de 2024
// Versión 5.1

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <vector>
#include <array>
#include <utility>
#include "../Engine/engine.h"
using namespace std;

#ifndef LIGHTNING_H
#define LIGHTNING_H

class Lightning{
private:
    int hei;
    int wid;
    int dep;
    bool *** lightGrid = nullptr;
    float *** randGrid = nullptr;
    float leeway;
    float branch;
    int lightPoints;
    vector<int> directions;
    vector<array<int, 6>> branches;
    vector<array<int, 3>> canonVertices;

    float gridHeightInMeters;
    float forcedHeight;
    float downWeight;

public:
    Lightning(int hei = 181, int wid = 257, int dep = 181, float leeway = 0.24, float branch = 0.12, float downWeight = 0, float forcedHeight = 0.75, float gridHeightInMeters = 5000);
    ~Lightning();

    void freeMemory ();

    void setLeeway(float);
    void setBranch(float);
    float getLeeway(void);
    float getBranch(void);
    int getHei(void);
    int getWid(void);
    int getDep(void);
    int getLightPoints(void);
    bool*** getLightGrid(void);
    float*** getRandGrid(void);
    int getN(void);
    vector<array<int, 3>>* getCanonVertices(void);
    vector<float>* getFracs(void);

    void randomize(void);
    // void show(void);

    void traverse(int x, int y, int z, int prevxyz[3], bool tag=false);
    void superTraverse(void);
    
    float getGridHeightInMeters(void);

    unsigned long getInvolvedElectrons (float &environmental_factor);
    long double getElectronicMass (float &environmental_factor);
};

#endif