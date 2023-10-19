// Header de librería Lightning (Clases Point y Lightning)
// Autor: Mauricio Alcántar Dueñas
// Fecha: 14 de octubre de 2023
// Versión 1.0

#include <iostream>
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
    vector<vector<Point>> grid;
    float leeway;
    float branch;
    int fractality = 0.00;

    // store the end of a branch
    // ... reverse head if you'd like to call them that
    // size of vector determines number of branches
    // also allows us to later determine branch size
    vector<pair<int, int>> branches;
public:
    Lightning(int hei = 60, int wid = 30, float leeway = 0.28, float branch = 0.12);
    ~Lightning();

    void setLeeway(float);
    void setBranch(float);
    float getLeeway(void);
    float getBranch(void);
    int getHei(void);
    int getWid(void);
    vector<vector<Point>>& getGrid(void);
    int getN(void);
    int getFractality(void);

    void randomize(void);
    void show(void);

    void traverse(int, int);
};

#endif