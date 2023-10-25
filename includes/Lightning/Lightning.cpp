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
Lightning::Lightning(int hei, int wid, float leeway, float branch) {

    //FUCK AROUND WITH THESE FOUR NUMBERS AND FIND OUT
    this->hei = hei;
    this->wid = wid;

    this->grid = new Point* [this->hei];
    for (int i = 0; i < this->hei; i++) {
		this->grid[i] = new Point[this->wid];
	}
    
    this->leeway = leeway;
    this->branch = branch;
}

Lightning::~Lightning(){
}

void Lightning::setLeeway(float l){ this->leeway = l; }
void Lightning::setBranch(float b){ this->branch = b; }
float Lightning::getLeeway(void){ return leeway; }
float Lightning::getBranch(void){ return branch; }
float Lightning::getFractality(void) { return fractality; }

void Lightning::randomize(void){
    srand(time(NULL));
    float randy = 0;
    for (int i=0; i<hei; i++) {
        for (int j=0; j<wid; j++) {
            randy = (rand()%(101-(10*i/wid)))/(float)100;
            grid[i][j].setPotential(randy);
        }
    }
}

void Lightning::show(void){             //only for testing purposes
    for(int i=0; i<hei; i++){           //wont be used in practice
        for(int j=0; j<wid; j++){
            if(grid[i][j].getIsLight()){ cout << "x "; }
            else{ cout << "  "; }
        }
        cout << endl;
    }
}

void Lightning::traverse(int x, int y){
    while(x >= 0 && x < hei && y >= 0 && y < wid){
        int difX = x - grid[x][y].getPrevX();
        int difY = y - grid[x][y].getPrevY();
        tuple<int,int> neighbors[3];
        float neighborVal[3] = {0};
        int key = 0, min = 3, min2 = 3;

        grid[x][y].setIsLight(true);

        // Find the accessible neighbors
        if(x == 0 && y == 0){
            neighbors[key++] = make_tuple(0, 1); 
            neighbors[key++] = make_tuple(1, 0); 
            neighbors[key++] = make_tuple(1, 1); 
        }
        if(difY != 0){
            if(y+difY >= 0 && y+difY < wid){ 
                neighbors[key++] = make_tuple(x, y+difY); 
                if(difX == 0){ 
                    if(x+1 < hei){ neighbors[key++] = make_tuple(x+1, y+difY); }
                    if(x-1 >= 0){ neighbors[key++] = make_tuple(x-1, y+difY); }
                }
                else if(x+difX >= 0 && x+difX < hei){
                    neighbors[key++] = make_tuple(x+difX, y+difY); 
                }
            }
        }
        if(difX != 0){
            if(x+difX >= 0 && x+difX < hei){ 
                neighbors[key++] = make_tuple(x+difX, y); 
                if(difY == 0){ 
                    if(y+1 < wid){ neighbors[key++] = make_tuple(x+difX, y+1); }
                    if(y-1 >= 0){ neighbors[key++] = make_tuple(x+difX, y-1); }
                }
            }
        }

        // Find the path of least resistance
        for(int i=0; i<key; i++){
            if(grid[get<0>(neighbors[i])][get<1>(neighbors[i])].getIsLight()){
                neighbors[i--].swap(neighbors[--key]);
            }
            else{
                neighborVal[i] = grid[x][y].getPotential() + leeway;
                neighborVal[i] -= grid[get<0>(neighbors[i])][get<1>(neighbors[i])].getPotential();
                if(neighborVal[i] > 0){
                    if(min == 3 || neighborVal[i] > neighborVal[min]){ min = i; }
                }
            }
        }
        if(min == 3) {
            auto ending = make_tuple(x, y);
            branches.push_back(ending);
            return;
        } // End of branch

        // Check for branching opportunities
        for(int i=0; i<key; i++){
            if(i != min && neighborVal[i] >= neighborVal[min]-branch){
                if(min2 == 3 || neighborVal[i] > neighborVal[min2]){ min2 = i; }
            }
        }

        // Next Step
        grid[get<0>(neighbors[min])][get<1>(neighbors[min])].setPrevX(x);
        grid[get<0>(neighbors[min])][get<1>(neighbors[min])].setPrevY(y);
        if(min2 == 3){                  // No branching
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

Point** Lightning::getGrid(void) {
    return grid;
}

int Lightning::getHei(void) {
    return hei;
}
int Lightning::getWid(void) {
    return wid;
}

int Lightning::getN(void)
{
    return branches.size();
}