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
    Field(void);
    ~Field(void);

    void setLen(int);
    void setWid(int);
    int getLen(void);
    int getWid(void);

    void randomize(void);
    void show(void);
};

#include "Lightning.cpp"
#endif