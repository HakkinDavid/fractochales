#include "Lightning.h"
using namespace std;

// FIELD CLASS
Field::Field(){
    this->len = 40;
    this->wid = 40;
}
<<<<<<< Updated upstream:src/Lightning/Lightning.cpp
Field::~Field(void){
    cout << "Te voy a castear." << endl;
=======
Field::~Field(){
>>>>>>> Stashed changes:Lightning/Lightning.cpp
}

void Field::setLen(int len){ this->len = len; }
void Field::setWid(int wid){ this->wid = wid; }
int Field::getLen(void){ return len; }
int Field::getWid(void){ return wid; }

void Field::randomize(void){
    srand(time(NULL));
    float randy = 0;
    for (int i=0; i<wid; i++) {
        for (int j=0; j<len; j++) {
            randy = (rand()%101)/(float)100;
            grid[i][j] = randy;
        }
    }
}

void Field::show(void){             //only for testing purposes
    for(int i=0; i<wid; i++){       //wont be used in practice
        for(int j=0; j<len; j++){
            cout << grid[i][j] << " ";
        }
        cout << endl;
    }
}

// POINT CLASS
Point::Point(){
    this->isLight = false;
    this->prevX = 0;
    this->prevY = 0;
}
Point::~Point(){
}

void Point::setIsLight(bool light){ this->isLight = light; }
void Point::setPrevX(int x){ this->prevX = x; }
void Point::setPrevY(int y){ this->prevY = y; }
bool Point::getIsLight(void){ return isLight; }
int Point::getPrevX(void){ return prevX; }
int Point::getPrevY(void){ return prevY; }

// LIGHTNING CLASS
Lightning::Lightning(){
    this->leeway = 0.2;
    this-> branch = 0.05;
}
Lightning::~Lightning(){
}

void Lightning::setLeeway(float l){ this->leeway = l; }
void Lightning::setBranch(float b){ this->branch = b; }
float Lightning::getLeeway(void){ return leeway; }
float Lightning::getBranch(void){ return branch; }