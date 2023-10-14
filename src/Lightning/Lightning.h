#include <iostream>
#include <cstdlib>
#include <ctime>
#include <tuple>
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
    int len;
    int wid;
    Point grid[40][40];
    float leeway;
    float branch;

public:
    Lightning();
    ~Lightning();

    void setLeeway(float);
    void setBranch(float);
    float getLeeway(void);
    float getBranch(void);

    void randomize(void);
    void show(void);

    void traverse(int, int);
};

#include "Lightning.cpp"
#endif