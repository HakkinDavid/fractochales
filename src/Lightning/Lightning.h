#include <iostream>
#include <cstdlib>
#include <ctime>
using namespace std;

#ifndef LIGHTNING_H
#define LIGHTNING_H

class Field{
private:
    int len;
    int wid;
    float grid[40][40];

public:
    Field();
    ~Field();

    void setLen(int);
    void setWid(int);
    int getLen(void);
    int getWid(void);

    void randomize(void);
    void show(void);
};

class Point{
private:
    bool isLight;
    int prevX;
    int prevY;

public:
    Point();
    ~Point();

    void setIsLight(bool);
    void setPrevX(int);
    void setPrevY(int);
    bool getIsLight(void);
    int getPrevX(void);
    int getPrevY(void);
};

class Lightning{
private:
    Point lightGrid[40][40];
    float leeway;
    float branch;

public:
    Lightning();
    ~Lightning();

    void setLeeway(float);
    void setBranch(float);
    float getLeeway(void);
    float getBranch(void);
};

#include "Lightning.cpp"
#endif