// Implementación de librería Lightning (Clases Point y Lightning)
// Autor: Mauricio Alcántar Dueñas
// Fecha: 14 de octubre de 2023
// Versión 1.0

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
Lightning::Lightning(){ //FUCK AROUND WITH THESE FOUR NUMBERS AND FIND OUT
    this->len = 60; //change the grid size in the header
    this->wid = 30; //if you mess with len and wid
    this->leeway = 0.23;
    this->branch = 0.15;
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
            if(grid[i][j].getIsLight()){ cout << "x "; }
            else{ cout << "  "; }
        }
        cout << endl;
    }
}

void Lightning::traverse(int x, int y){
    while(x >= 0 && x < wid && y >= 0 && y < len){
        int difX = x - grid[x][y].getPrevX();
        int difY = y - grid[x][y].getPrevY();
        tuple<int,int> neighbors[5];
        float neighborVal[5] = {0};
        int key = 0, min = 5, min2 = 5;

        grid[x][y].setIsLight(true);

        // Find the accessible neighbors
        if(difY == 0){
            if(y+1 < len){ neighbors[key++] = make_tuple(x, y+1); }
            if(y-1 >= 0){ neighbors[key++] = make_tuple(x, y-1); }
        }
        else if(y+difY >= 0 && y+difY < len){
            neighbors[key++] = make_tuple(x, y+difY);
            if(x+1 < wid){ neighbors[key++] = make_tuple(x+1, y+difY); }
            if(x-1 >= 0){ neighbors[key++] = make_tuple(x-1, y+difY); }
        }
        if(difX == 0){
            if(x+1 < wid){ neighbors[key++] = make_tuple(x+1, y); }
            if(x-1 >= 0){ neighbors[key++] = make_tuple(x-1, y); }
        }
        else if(x+difX >= 0 && x+difX < wid){
            neighbors[key++] = make_tuple(x+difX, y);
            if(x != 0 && y != 0 && y-difY >= 0 && y-difY < len){
                neighbors[key++] = make_tuple(x+difX, y-difY);
            }
            else{
                if(y+1 < len){ neighbors[key++] = make_tuple(x+difX, y+1); }
                if(y-1 >= 0){ neighbors[key++] = make_tuple(x+difX, y-1); }
            }
        }
        if(x == 0 && y == 0){ neighbors[key++] = make_tuple(1, 1); }

        // Find the path of least resistance
        for(int i=0; i<key; i++){
            if(grid[get<0>(neighbors[i])][get<1>(neighbors[i])].getIsLight()){
                neighbors[i--].swap(neighbors[--key]);
            }
            else{
                neighborVal[i] = grid[x][y].getPotential() + leeway;
                neighborVal[i] -= grid[get<0>(neighbors[i])][get<1>(neighbors[i])].getPotential();
                if(neighborVal[i] > 0){
                    if(min == 5 || neighborVal[i] > neighborVal[min]){ min = i; }
                }
            }
        }
        if(min == 5){ return; } // End of branch

        // Check for branching opportunities
        for(int i=0; i<5; i++){
            if(i != min && neighborVal[i] >= neighborVal[min]-branch){
                if(min2 == 5 || neighborVal[i] > neighborVal[min2]){ min2 = i; }
            }
        }

        // Next Step
        grid[get<0>(neighbors[min])][get<1>(neighbors[min])].setPrevX(x);
        grid[get<0>(neighbors[min])][get<1>(neighbors[min])].setPrevY(y);
        if(min2 == 5){                  // No branching
            x = get<0>(neighbors[min]);
            y = get<1>(neighbors[min]);
        }
        else{                           // Yes branching >:)
            grid[get<0>(neighbors[min2])][get<1>(neighbors[min2])].setPrevX(x);
            grid[get<0>(neighbors[min2])][get<1>(neighbors[min2])].setPrevY(y);
            traverse(get<0>(neighbors[min]), get<1>(neighbors[min]));
            traverse(get<0>(neighbors[min2]), get<1>(neighbors[min2]));
            return;
        }

    } // end while loop

}