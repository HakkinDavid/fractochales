// Header de librería Lightning (Clases Point y Lightning)
// Autor: Mauricio Alcántar Dueñas
// Fecha: 14 de octubre de 2023
// Versión 1.0

#include <iostream>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <tuple>
#include <vector>
#include <utility>
using namespace std;

#ifndef LIGHTNING_H
#define LIGHTNING_H

class Point{
private:
    bool isLight;
    float potential;
    int prevX;
    int prevY;

public:
    Point();
    ~Point();

    void setIsLight(bool);
    void setPotential(float);
    void setPrevX(int);
    void setPrevY(int);
    bool getIsLight(void);
    float getPotential(void);
    int getPrevX(void);
    int getPrevY(void);
};

class Lightning{
private:
    int hei;
    int wid;
    Point ** grid = nullptr;
    float leeway;
    float branch;
    int lightPoints;
    vector<tuple<int, int>> branches;
    vector<float> fracs;

    float gridHeightInMeters;
    float forcedHeight;
    float downWeight;

public:
    Lightning(int hei = 181, int wid = 257, float leeway = 0.24, float branch = 0.12, float gridHeightInMeters = 5000, float forcedHeight = 0.75, float downWeight = 0);
    ~Lightning();

    void setLeeway(float);
    void setBranch(float);
    float getLeeway(void);
    float getBranch(void);
    int getHei(void);
    int getWid(void);
    int getLightPoints(void);
    Point** getGrid(void);
    int getN(void);
    vector<float>* getFracs(void);

    void randomize(void);
    void show(void);

    void traverse(int, int);
    void superTraverse(void);
    
    float* directionComp(void);
    void fractalComp(void);
    float getGridHeightInMeters(void);

    unsigned long getInvolvedElectrons (float &environmental_factor);
    long double getElectronicMass (float &environmental_factor);
};

#endif