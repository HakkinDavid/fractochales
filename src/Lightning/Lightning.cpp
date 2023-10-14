#include "Lightning.h"
using namespace std;

// POINT CLASS
Point::Point(){
    this->isLight = false;
    this->potential = 0;
    this->prevX = 0;
    this->prevY = 0;
}
Point::~Point(){
}

void Point::setIsLight(bool light){ this->isLight = light; }
void Point::setPotential(float p){ this->potential = p; }
void Point::setPrevX(int x){ this->prevX = x; }
void Point::setPrevY(int y){ this->prevY = y; }
bool Point::getIsLight(void){ return isLight; }
float Point::getPotential(void){ return potential; }
int Point::getPrevX(void){ return prevX; }
int Point::getPrevY(void){ return prevY; }

// LIGHTNING CLASS
Lightning::Lightning(){
    this->len = 40;
    this->wid = 40;
    this->leeway = 0.2;
    this->branch = 0.05;
}
Lightning::~Lightning(){
}

void Lightning::setLeeway(float l){ this->leeway = l; }
void Lightning::setBranch(float b){ this->branch = b; }
float Lightning::getLeeway(void){ return leeway; }
float Lightning::getBranch(void){ return branch; }

void Lightning::randomize(void){
    srand(time(NULL));
    float randy = 0;
    for (int i=0; i<wid; i++) {
        for (int j=0; j<len; j++) {
            randy = (rand()%101)/(float)100;
            grid[i][j].setPotential(randy);
        }
    }
}

void Lightning::show(void){             //only for testing purposes
    for(int i=0; i<wid; i++){           //wont be used in practice
        for(int j=0; j<len; j++){
            cout << grid[i][j].getPotential() << " ";
        }
        cout << endl;
    }
}

void Lightning::traverse(int x, int y){
    grid[x][y].setIsLight(true);
    int difX = x - grid[x][y].getPrevX();
    int difY = y - grid[x][y].getPrevY();
    tuple<int,int> neighbors[5];
    float neighborVal[5];
    int key = 0;

    if(difY == 0){
        if(y+1 < len){ neighbors[key++] = make_tuple(x, y+1); }
        if(y-1 >= 0){ neighbors[key++] = make_tuple(x, y-1); }
    }
    else{
        if(y+difY >= 0 && y+difY < len){
            neighbors[key++] = make_tuple(x, y+difY);
            if(x+1 < wid){ neighbors[key++] = make_tuple(x+1, y+difY); }
            if(x-1 >= 0){ neighbors[key++] = make_tuple(x-1, y+difY); }
        }
    }
    if(difX == 0){
        if(x+1 < wid){ neighbors[key++] = make_tuple(x+1, y); }
        if(x-1 >= 0){ neighbors[key++] = make_tuple(x-1, y); }
    }
    else{
        if(x+difX >= 0 && x+difX < wid){
            neighbors[key++] = make_tuple(x+difX, y);
            if(x != 0 && y != 0 && y-difY >= 0 && y-difY < len){
                neighbors[key++] = make_tuple(x+difX, y-difY);
            }
            else{
                if(y+1 < len){ neighbors[key++] = make_tuple(x+difX, y+1); }
                if(y-1 >= 0){ neighbors[key++] = make_tuple(x+difX, y-1); }
            }
        }
    }


}