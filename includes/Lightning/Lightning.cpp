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
Lightning::Lightning(int hei, int wid, float leeway, float branch, float downWeight, float forcedHeight, float gridHeightInMeters) {

    //FUCK AROUND WITH THESE NUMBERS AND FIND OUT
    this->hei = hei;
    this->wid = wid;
    this->leeway = leeway;
    this->branch = branch;
    this->gridHeightInMeters = gridHeightInMeters;
    this->forcedHeight = forcedHeight;
    this->downWeight = downWeight;

    this->lightPoints = 0;
    this->grid = new Point* [this->hei];
    for (int i = 0; i < this->hei; i++) {
		this->grid[i] = new Point[this->wid];
	}
}

Lightning::~Lightning(){
}

void Lightning::setLeeway(float l){ this->leeway = l; }
void Lightning::setBranch(float b){ this->branch = b; }
float Lightning::getLeeway(void){ return leeway; }
float Lightning::getBranch(void){ return branch; }
int Lightning::getHei(void){ return hei; }
int Lightning::getWid(void){ return wid; }
int Lightning::getLightPoints(void){ return lightPoints; }
Point** Lightning::getGrid(void){ return grid; }
int Lightning::getN(void){ return branches.size(); }
vector<float>* Lightning::getFracs(void){ return &fracs; }

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

        if(!grid[x][y].getIsLight()){ lightPoints++; }
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
                // Interesting question: should we add the downWeight bonus before or after the next test? we may never knoe....
                if(downWeight > 0 && get<0>(neighbors[i]) - x == 1){
                    neighborVal[i] += downWeight;
                }
                else if(downWeight < 0 && get<0>(neighbors[i]) - x == -1){
                    neighborVal[i] -= downWeight;
                }
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
    int min = 3, key = 0, x = 0, y = 0;
    int middle = wid/2;

    // Start of lightning at top center of screen
    grid[0][middle].setIsLight(true);
    grid[0][middle].setPrevY(middle);
    lightPoints++;
    origins[0] = make_tuple(1, middle-1);
    origins[1] = make_tuple(1, middle);
    origins[2] = make_tuple(1, middle+1);
    for(int i=0; i < 3; i++){
        originVal[i] = grid[0][middle].getPotential() + leeway;
        originVal[i] -= grid[get<0>(origins[i])][get<1>(origins[i])].getPotential();
        if(min == 3 || originVal[i] > originVal[min]){ min = i; }
    }
    x = get<0>(origins[min]);
    y = get<1>(origins[min]);
    grid[x][y].setPrevY(middle);

    // Traversing loop
    do{
        traverse(x, y);

        // Find lowest and closest to center lightning point
        x = -1; y = -1;
        for(int i = hei-1; i >= 0; i--){
            for(int j = wid-1; j >=0; j--){
                if(grid[i][j].getIsLight()){ 
                    x = i; 
                    if((abs(j-middle)) < (abs(y-middle))){ y = j; }
                    else{ break; }
                }
            }
            if(x != -1){ break; }
        }

        // Lowest point isn't low enough
        if(x < hei*forcedHeight){
            // If lowest point is a branch ending, delete branch
            int dex = branches.size();
            for(int i=0; i < branches.size(); i++){
                if(get<0>(branches[i]) == x && get<1>(branches[i]) == y){ dex = i; }
            }
            if(dex != branches.size()){
                branches[dex].swap(branches.back());
                branches.pop_back();
            }

            // Force to continue going down
            int difX = x - grid[x][y].getPrevX();
            int difY = y - grid[x][y].getPrevY();
            key = 0; min = 3;
            if(difX != 0){ origins[key++] = make_tuple(x+1, y); }
            if(difY <= 0 && y-1 >= 0){ origins[key++] = make_tuple(x+1, y-1); }
            if(difY >= 0 && y+1 < wid){ origins[key++] = make_tuple(x+1, y+1); }
            for(int i=0; i < key; i++){
                originVal[i] = grid[x][y].getPotential() + leeway;
                originVal[i] -= grid[get<0>(origins[i])][get<1>(origins[i])].getPotential();
                if(min == 3 || originVal[i] > originVal[min]){ min = i; }
            }
            grid[get<0>(origins[min])][get<1>(origins[min])].setPrevX(x);
            grid[get<0>(origins[min])][get<1>(origins[min])].setPrevY(y);
            x += 1;
            y = get<1>(origins[min]);
            grid[x][y].setIsLight(true);
            lightPoints++;
        }

    } while(x < hei*forcedHeight); // Loop if not low enough
    
}

//directionComp should be called before fractalComp
//or the main branch wont be accounted for
float* Lightning::directionComp(void){
    int n = branches.size();
    int xi = 0, yi = 0;
    int Ex = 0, Ey = 0, Ex2 = 0, Ey2 = 0, Exy = 0;
    float numer = 0;
    float* direction = new float[3]; // Array pointer to return

    for(int i=0; i < n; i++){   // Calculate sums
        xi = get<0>(branches[i]);
        yi = get<1>(branches[i]);
        Ex += xi;
        Ey += yi;
        Ex2 += xi * xi;
        Ey2 += yi * yi;
        Exy += xi * yi;
    }

    numer = (n*Exy - Ex*Ey);
    direction[0] = numer / (float)(n*Ex2 - Ex*Ex); // a1, slope
    direction[1] = ((float)Ey/n) - direction[0]*((float)Ex/n); // a0, displacement
    direction[2] = numer / (sqrt(n*Ex2 - Ex*Ex) * sqrt(n*Ey2 - Ey*Ey)); // r, correlation coefficient (THIS SHOULD NOT BE SQUARED YET) ... its square is a different number (determination coefficient)

    return direction; // a1, a0, r
}

void Lightning::fractalComp(void){
    float avgLen = 0, S = exp(1), N = exp(1);
    int mainLen = 0, dex = 0;
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
        while(currX != 0 || currY != wid/2){
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
    while(currX != 0 || currY != wid/2){
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

    //Calculate fractality using Maclaurin Series
    float er = 100, logN = 0, logS = 0;
    int i = 1;
    N = (1.0/N) - 1;
    S = (1.0/S) - 1;
    while(er > 0.00001){
        logN += (pow(-1, i+1)) * (pow(N, i)/i);
        logS += (pow(-1, i+1)) * (pow(S, i)/i);
        fracs.emplace_back(logN/logS);
        if(i > 1){
            er = abs((fracs[i-1] - fracs[i-2]) * 100 / fracs[i-1]);
        }
        i++;
    }
}

float Lightning::getGridHeightInMeters () {
    return gridHeightInMeters;
}

unsigned long Lightning::getInvolvedElectrons (float &environmental_factor) {
    // MAURICIO'S APPROACH, IMPLEMENTED BY DAVID
    return
    (
        (gridHeightInMeters / (float) hei) // get height of a node in meters
        *
        (
            // for reference, see (A)
            /*
            cbrt(26521541178535914242048000) // there are 26521541178535914242048000 molecules in a cubic meter of air, so its cubic root should be density per meter
            *
            (
                (
                    0.79 // 79% of air is nitrogen molecules
                    *
                    2 // 2 atoms per nitrogen molecule
                    *
                    5 // 5 valence electrons per nitrogen atom
                )
                +
                (
                    0.21 // 21% of air is oxygen molecules
                    *
                    2 // 2 atoms per oxygen molecule
                    *
                    6 // 6 valence electrons per oxygen atom
                )
            )
            */
            // (A) distributive law allows us to precalculate this parentheses (aka environmental factor) without affecting the result
            // 3107424876.30374896651003593080636010231246734962391002 electrons per linear meter of air
            environmental_factor
        )
        *
        lightPoints // multiply by all nodes that have been lit up
    );


    /*
    // UNUSED DAVID APPROACH (IT'S NOT STRICTLY WRONG BUT DOESN'T ACCOUNT FOR ACTUAL DENSITY)
    // unlike the approach we talked about in class, i realized a lighting is unidimensional and by no means uses a cubic meter ... let alone 16 m^3 or shit
    // so here is a non volume-dependant approach
    // this COULD BE adapted into a more dynamic [no pun intended] calc so we can consider different ambients and not just air structure and proportions
    // if someone wants to do it, it's the same idea : node height in meters * environmental factor
    // environmental factor = (proportion of element / element atomic height * valence electrons per atom) + ...
    // since we wanna precalc these values (because they are constant and use quite big numbers), different environmental factors can be stored in an array i guess, then just multiply by height of a node in meters
    // then you just multiply by number of nodes that were lit up
    return (
        // this parentheses should account for all electrons in each node
        (
            (gridHeightInMeters / (float) hei) // get SI height of a node
            *
            // for reference see (A)
            (
                (
                    0.79 // proportion of nitrogen in air || if this is confusing your mind out, consider that our lighting is kinda unidimensional, so strictly all we have are individual atoms stacked up, therefore, the height is composed of this percentage of nitrogen atoms
                    / (0.000000000092 * 2) // 0.000000000092 m is nitrogen radius (double it and you get the atom height) ... if you divide total height that corresponds to this kind of atom, you should get the number of atoms involved
                    * 5 // valence electrons of each nitrogen atom
                )
                +
                (
                    0.21 // proportion of oxygen in air || if this is confusing your mind out, consider that our lighting is kinda unidimensional, so strictly all we have are individual atoms stacked up, therefore, the height is composed of this percentage of oxygen atoms
                    / (0.000000000073 * 2) // 0.000000000073 m is oxygen radius (double it and you get the atom height) ... if you divide total height that corresponds to this kind of atom, you should get the number of atoms involved
                    * 6 // valence electrons of each oxygen atom
                )
            )
           // (A) distributive law allows us to precalculate the parentheses (environmental factor) without affecting the result
           // result is 30 097 528 290 . 649 197, so we just multiply it by SI height
           // 30097528290.649197
        )
        *
        lightPoints // multiply by all nodes that have been lit up
    );
    */
}

long double Lightning::getElectronicMass (float &environmental_factor) {
    return ((float) getInvolvedElectrons(environmental_factor) * (float) 0.00000000000000000000000000000091);  // un electrón tiene masa de 9.1x(10^(-31)) kg
}