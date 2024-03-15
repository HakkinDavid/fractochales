// Implementación de librería Lightning 
// Autor: Mauricio Alcántar Dueñas
// Fecha: 6 de marzo de 2024
// Versión 5.1

#include "Lightning.h"
using namespace std;

// LIGHTNING CLASS
Lightning::Lightning(int hei, int wid, int dep, float leeway, float branch, float downWeight, float forcedHeight, float gridHeightInMeters) {

    //FUCK AROUND WITH THESE NUMBERS AND FIND OUT
    this->hei = hei;
    this->wid = wid;
    this->dep = dep;
    this->leeway = leeway;
    this->branch = branch;
    this->gridHeightInMeters = gridHeightInMeters;
    this->forcedHeight = forcedHeight;
    this->downWeight = downWeight;
    //this->recSteps = 0;

    this->lightPoints = 0;
    this->lightGrid = new bool** [this->hei];
    for (int i = 0; i < this->hei; i++) {
		this->lightGrid[i] = new bool* [this->wid];
        for (int j = 0; j < this->wid; j++) {
            this->lightGrid[i][j] = new bool[this->dep];
            for (int k = 0; k < this->dep; k++){
                lightGrid[i][j][k] = false;
            }
        }
	}
    this->randGrid = new float** [this->hei];
    for (int i = 0; i < this->hei; i++) {
		this->randGrid[i] = new float* [this->wid];
        for (int j = 0; j < this->wid; j++) {
            this->randGrid[i][j] = new float[this->dep];
            for (int k = 0; k < this->dep; k++){
                randGrid[i][j][k] = 0.f;
            }
        }
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
int Lightning::getDep(void){ return dep; }
int Lightning::getLightPoints(void){ return lightPoints; }
bool*** Lightning::getLightGrid(void){ return lightGrid; }
float*** Lightning::getRandGrid(void){ return randGrid; }
int Lightning::getN(void){ return branches.size(); }
vector<array<int, 3>>* Lightning::getCanonVertices(void){ return &canonVertices; }
vector<float>* Lightning::getFracs(void){ return &fracs; }

void Lightning::randomize(void){
    srand(time(NULL));
    float randy = 0;
    for (int i=0; i<hei; i++) {
        for (int j=0; j<wid; j++) {
            for (int k=0; k<dep; k++){ 
                randy = (rand()%101)/(float)100;
                randGrid[i][j][k] = randy;
            }
        }
    }
}

/* void Lightning::show(void){             //only for testing purposes
    for(int i=0; i<hei; i++){           //wont be used in practice
        for(int j=0; j<wid; j++){
            if(grid[i][j].getIsLight()){ cout << "x "; }
            else{ cout << "  "; }
        }
        cout << endl;
    }
} */

void Lightning::traverse(int x, int y, int z, int prevxyz[3], bool tag){
    //if (++recSteps >= 4500) return;
    while(x >= 0 && x < hei && y >= 0 && y < wid && z >= 0 && z < dep){
        int diff[3] = { x - prevxyz[0], y - prevxyz[1], z - prevxyz[2] }; 
        vector<array<int, 3>> neighbors;
        array<int, 3> minVal, min2Val;
        float neighborVal[10] = {0};
        int key = 0, min = 0, min2 = 0;

        if(!lightGrid[x][y][z]){ lightPoints++; }
        lightGrid[x][y][z] = true;

        // Determine valid neighbors based on direction
        for(int i=-1; i<=1; i++){
            for(int j=-1; j<=1; j++){
                for(int k=-1; k<=1; k++){
                    if((diff[0]*i + diff[1]*j + diff[2]*k) > 0){
                        neighbors.push_back({x+i, y+j, z+k});
                    }
                }
            }
        }
        key = neighbors.size();
        min = key; min2 = key;

        // Find the path of least resistance
        for(int i=0; i<key; i++){
            if(neighbors[i][0] < 0 || neighbors[i][0] >= hei || 
               neighbors[i][1] < 0 || neighbors[i][1] >= wid || 
               neighbors[i][2] < 0 || neighbors[i][2] >= dep){
                neighbors[i--].swap(neighbors[--key]);
            }
            else if((tag && (neighbors[i][0] - x) < 1) || lightGrid[neighbors[i][0]][neighbors[i][1]][neighbors[i][2]]){
                neighbors[i--].swap(neighbors[--key]);
            }
            else{
                neighborVal[i] = randGrid[x][y][z] + leeway;
                neighborVal[i] -= randGrid[neighbors[i][0]][neighbors[i][1]][neighbors[i][2]];
                // BAD!! question: should we add the downWeight bonus before or after the next test? we may never knoe....
                if(downWeight > 0 && neighbors[i][0] - x == 1){
                    neighborVal[i] += downWeight;
                }
                else if(downWeight < 0 && neighbors[i][0] - x == -1){
                    neighborVal[i] -= downWeight;
                }
                if(neighborVal[i] > 0){
                    if((min >= key) || neighborVal[i] > neighborVal[min]){ min = i; }
                }
            }
        }
        if((min >= key)) {
            return;
        } // End of branch


        // Check for branching opportunities
        for(int i=0; i<key; i++){
            if(i != min && neighborVal[i] >= neighborVal[min]-branch){
                if((min2 >= key) || neighborVal[i] > neighborVal[min2]){ min2 = i; }
            }
        }

        // Wipe neighbors
        minVal[0] = neighbors[min][0]; minVal[1] = neighbors[min][1]; minVal[2] = neighbors[min][2]; 
        if((min2 < key)){
            min2Val[0] = neighbors[min2][0]; min2Val[1] = neighbors[min2][1]; min2Val[2] = neighbors[min2][2];
        }
        neighbors.clear();

        // Next Step
        prevxyz[0] = x; prevxyz[1] = y; prevxyz[2] = z;
        canonVertices.push_back({x, y, z});
        canonVertices.push_back(minVal);
        if((min2 < key)){ 
            branches.push_back({min2Val[0], min2Val[1], min2Val[2], x, y, z});
        }
        x = minVal[0];
        y = minVal[1];
        z = minVal[2];

    } // end while loop

}

void Lightning::superTraverse(){
    vector<array<int, 3>> origins;
    float originVal[9] = {0};
    int min = 9, x = 0, y = 0, z = 0;
    int midwid = wid/2, middep = dep/2;

    // Start of lightning at top center of screen
    lightGrid[0][midwid][middep] = true;
    canonVertices.push_back({0, midwid, middep});
    lightPoints++;
    for(int i=0; i < 9; i++){ origins.push_back({1, midwid, middep}); }
    origins[0][1] += 1; origins[1][1] += 1; origins[2][1] += 1;
    origins[6][1] -= 1; origins[7][1] -= 1; origins[8][1] -= 1;
    origins[0][2] += 1; origins[3][2] += 1; origins[6][2] += 1;
    origins[2][2] -= 1; origins[5][2] -= 1; origins[8][2] -= 1;
    for(int i=0; i < 9; i++){
        originVal[i] = randGrid[0][midwid][middep] + leeway;
        originVal[i] -= randGrid[origins[i][0]][origins[i][1]][origins[i][2]];
        if(min == 9 || originVal[i] > originVal[min]){ min = i; }
    }
    x = origins[min][0];
    y = origins[min][1];
    z = origins[min][2];
    int prevxyz[3] = {0, midwid, middep};
    canonVertices.push_back({x, y, z});

    traverse(x, y, z, prevxyz);

    // Traversing loop
    do{
        // Check last point
        x = canonVertices.back()[0];

        // Last point isn't low enough
        if(x < hei*forcedHeight){
            //Backtrack
            canonVertices.pop_back();
            canonVertices.pop_back();
            lightPoints--;

            if(canonVertices.size() == 0){
                randomize();
                superTraverse();
                std::cout << "FUCK!!" << std::endl;
                return;
            }

            x = canonVertices.back()[0];
            y = canonVertices.back()[1];
            z = canonVertices.back()[2];
            prevxyz[0] = canonVertices[canonVertices.size()-2][0];
            prevxyz[1] = canonVertices[canonVertices.size()-2][1];
            prevxyz[2] = canonVertices[canonVertices.size()-2][2];

            if(x == branches.back()[3] && y == branches.back()[4] && z == branches.back()[5]){
                branches.pop_back();
            }

            // Force to continue going down
            randGrid[x][y][z] = 2;
            traverse(x, y, z, prevxyz);
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
        xi = branches[i][0];
        yi = branches[i][1];
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
    array<int, 6> mainB = {0,0,0};
    vector<int> branLens;

    // Auxiliary int grid
    int ** fractalGrid = new int* [hei];
    for(int i=0; i < hei; i++){
		fractalGrid[i] = new int[wid];
        for(int j=0; j < wid; j++){ fractalGrid[i][j] = 0; }
	}

    // Find main branch and main branch length
    for(int i=0; i < branches.size(); i++){
        int currX = branches[i][0];
        int currY = branches[i][1];
        int currLen = 0, antX = 0, antY = 0;
        /* tf was this
        while(currX != 0 || currY != wid/2){
            //antX = grid[currX][currY].getPrevX();
            //antY = grid[currX][currY].getPrevY();
            currX = antX; currY = antY;
            currLen++;
        }
        */
        if(currLen > mainLen){ 
            mainLen = currLen;
            mainB = branches[i];
            dex = i;
        }
    }

    // Trace the main branch path in fractalGrid
    //branches[dex].swap(branches.back()); // this is broken
    //branches.pop_back(); // this is broken
    int currX = mainB[0];
    int currY = mainB[1];
    int antX = 0, antY = wid/2;
    fractalGrid[currX][currY] = -1;
    // while loop was stuck, make sure to adapt this back
    while(currX != 0 || currY != wid/2){
        //antX = grid[currX][currY].getPrevX();
        //antY = grid[currX][currY].getPrevY();
        currX = antX; currY = antY;
        fractalGrid[currX][currY] = -1;
    }

    // Find secondary branches and calculate average length
    for(int i=0; i < branches.size(); i++){
        currX = branches[i][0];
        currY = branches[i][1];
        int currLen = 0;
        antX = 0; antY = 0;
        while(fractalGrid[currX][currY] > -1){
            fractalGrid[currX][currY] = currLen++;
            //antX = grid[currX][currY].getPrevX();
            //antY = grid[currX][currY].getPrevY();
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