#include "Lightning.h"

using namespace std;

Field::Field(void){
    this->len = 10;
    this->wid = 10;
    grid = new float[10, 10];
}

Field::Field(int l, int w){
    this->len = l;
    this->wid = w;
    grid = new float[l, w];
}

Field::~Field(void){
    delete[] grid;
    cout << "Porno" << endl;
}