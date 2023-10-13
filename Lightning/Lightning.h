#include <iostream>
using namespace std;

#ifndef LIGHTNING_H
#define LIGHTNING_H

class Field{
private:
    int len;
    int wid;
    float *grid;

public:
    Field(void);
    Field(int, int);
    ~Field(void);
};

#include "Lightning.cpp"
#endif