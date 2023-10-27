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
int Lightning::getHei(void){ return hei; }
int Lightning::getWid(void){ return wid; }
Point** Lightning::getGrid(void){ return grid; }
int Lightning::getN(void){ return branches.size(); }

void Lightning::randomize(void){
    srand(time(NULL));
    float randy = 0;
    for (int i=0; i<hei; i++) {
        for (int j=0; j<wid; j++) {
            randy = (rand()%101)/(float)100;
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
        if(difY != 0){
            if(y+difY >= 0 && y+difY < wid){ 
                neighbors[key++] = make_tuple(x, y+difY); 
                if(difX == 0){ 
                    if(x+1 < hei){ 
                        if(!grid[x+1][y].getIsLight() || !grid[x][y+difY].getIsLight()){
                            neighbors[key++] = make_tuple(x+1, y+difY); 
                        }
                    }
                    if(x-1 >= 0){ 
                        if(!grid[x-1][y].getIsLight() || !grid[x][y+difY].getIsLight()){
                            neighbors[key++] = make_tuple(x-1, y+difY); 
                        }
                    }
                }
                else if(x+difX >= 0 && x+difX < hei){
                    if(!grid[x+difX][y].getIsLight() || !grid[x][y+difY].getIsLight()){
                        neighbors[key++] = make_tuple(x+difX, y+difY); 
                    }
                }
            }
        }
        if(difX != 0){
            if(x+difX >= 0 && x+difX < hei){ 
                neighbors[key++] = make_tuple(x+difX, y); 
                if(difY == 0){ 
                    if(y+1 < wid){ 
                        if(!grid[x+difX][y].getIsLight() || !grid[x][y+1].getIsLight()){
                            neighbors[key++] = make_tuple(x+difX, y+1); 
                        }
                    }
                    if(y-1 >= 0){ 
                        if(!grid[x+difX][y].getIsLight() || !grid[x][y-1].getIsLight()){
                            neighbors[key++] = make_tuple(x+difX, y-1); 
                        }
                    }
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

void Lightning::superTraverse(){
    tuple<int,int> origins[3];
    float originVal[3] = {0};
    int min = 3, x = 0, y = 0; 

    grid[0][0].setIsLight(true);
    origins[0] = make_tuple(0, 1);
    origins[1] = make_tuple(1, 0);
    origins[2] = make_tuple(1, 1);
    for(int i=0; i < 3; i++){
        originVal[i] = grid[0][0].getPotential() + leeway;
        originVal[i] -= grid[get<0>(origins[i])][get<1>(origins[i])].getPotential();
        if(min == 3 || originVal[i] > originVal[min]){ min = i; }
    }
    x = get<0>(origins[min]);
    y = get<1>(origins[min]);

    do{
        traverse(x, y);

        x = -1; y = -1;
        for(int i = hei-1; i >= 0; i--){
            for(int j = wid-1; j >=0; j--){
                if(grid[i][j].getIsLight()){ 
                    x = i;
                    y = j;
                    break;
                }
            }
            if(x != -1){ break; }
        }

        if(x < hei/2){
            int dex = branches.size();
            for(int i=0; i < branches.size(); i++){
                if(get<0>(branches[i]) == x && get<1>(branches[i]) == y){ dex = i; }
            }
            if(dex != branches.size()){
                branches[dex].swap(branches.back());
                branches.pop_back();
            }
            int difY = y - grid[x][y].getPrevY();
            if(y+difY >=0 && y+difY < wid){
                grid[x+1][y+difY].setPrevX(x);
                grid[x+1][y+difY].setPrevY(y);
                y += difY;
            }
            else{
                grid[x+1][y].setPrevX(x);
                grid[x+1][y].setPrevY(y);
            }
            x += 1;
            grid[x][y].setIsLight(true);
        }
    } while(x < hei/2);

}

float Lightning::fractalComp(void){
    float avgLen = 0, frac = 0, S = 2;
    int mainLen = 0, dex = 0, N = 2;
    tuple<int, int> mainB = make_tuple(0, 0);
    vector<int> branLens;

    // Auxiliary int grid
    int ** fractalGrid = new int* [hei];
    for(int i=0; i < hei; i++){
		fractalGrid[i] = new int[wid];
        for(int j=0; j < wid; j++){ fractalGrid[i][j] = 0; }
	}

    // Find main branch and main branch length
    for(int i=0; i < branches.size(); i++){
        int currX = get<0>(branches[i]);
        int currY = get<1>(branches[i]);
        int currLen = 0, antX = 0, antY = 0;
        while(currX != 0 || currY != 0){
            antX = grid[currX][currY].getPrevX();
            antY = grid[currX][currY].getPrevY();
            currX = antX; currY = antY;
            currLen++;
        }
        if(currLen > mainLen){ 
            mainLen = currLen;
            mainB = branches[i];
            dex = i;
        }
    }

    // Trace the main branch path in fractalGrid
    branches[dex].swap(branches.back());
    branches.pop_back();
    int currX = get<0>(mainB);
    int currY = get<1>(mainB);
    int antX = 0, antY = 0;
    fractalGrid[currX][currY] = -1;
    while(currX != 0 || currY != 0){
        antX = grid[currX][currY].getPrevX();
        antY = grid[currX][currY].getPrevY();
        currX = antX; currY = antY;
        fractalGrid[currX][currY] = -1;
    }

    // Find secondary branches and calculate average length
    for(int i=0; i < branches.size(); i++){
        currX = get<0>(branches[i]);
        currY = get<1>(branches[i]);
        int currLen = 0;
        antX = 0; antY = 0;
        while(fractalGrid[currX][currY] > -1){
            fractalGrid[currX][currY] = currLen++;
            antX = grid[currX][currY].getPrevX();
            antY = grid[currX][currY].getPrevY();
            currX = antX; currY = antY;
            if(fractalGrid[currX][currY] >= currLen){ break; }
            if(fractalGrid[currX][currY] < 0){ 
                fractalGrid[currX][currY] -= 1;
                branLens.push_back(currLen);
            }
        }
    }
    for(int i=0; i < branLens.size(); i++){ avgLen += branLens[i]; }
    if(branLens.size() != 0){ avgLen /= (float)branLens.size(); } // Dont divide by zero

    // Set values of N and S
    if(branLens.size() > 1){ N = branLens.size(); }
    if(avgLen != 0){ 
        if((mainLen/avgLen) > 1){ S = mainLen/avgLen; } // Never divide by zero
    }
    //frac = getFractality(N, S);
    frac = log(N)/log(S);
    return frac;
}