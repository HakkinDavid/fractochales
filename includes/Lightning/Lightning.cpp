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

    this->lightPoints = 0;
    this->lightGrid = new bool** [this->hei];
    for (int i = 0; i < this->hei; i++) {
		this->lightGrid[i] = new bool* [this->wid];
        for (int j = 0; j < this->wid; j++) {
            this->lightGrid[i][j] = new bool[this->dep];
        }
	}
    this->randGrid = new float** [this->hei];
    for (int i = 0; i < this->hei; i++) {
		this->randGrid[i] = new float* [this->wid];
        for (int j = 0; j < this->wid; j++) {
            this->randGrid[i][j] = new float[this->dep];
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
vector<vector<int>>* Lightning::getCanonVertices(void){ return &canonVertices; }
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
    while(x >= 0 && x < hei && y >= 0 && y < wid && z >= 0 && z < dep){
        int diff[3] = { x - prevxyz[0], y - prevxyz[1], z - prevxyz[2] }; 
        vector<vector<int>> neighbors;
        float neighborVal[10] = {0};
        int key = 9, min = 9, min2 = 9;

        if(!lightGrid[x][y][z]){ lightPoints++; }
        lightGrid[x][y][z] = true;

        // Determine valid neighbors based on direction
        int piece = 0;
        for(int i=0; i<3; i++){
            if(diff[i] != 0) piece++;  
        }
        switch(piece){
            case 1:
            {
                int a = -1, b = 0;
                for(int i=0; i<9; i++){ neighbors.emplace_back(x+diff[0], y+diff[1], z+diff[2]); }
                for(int i=0; i<3; i++){
                    if(diff[i] == 0){
                        if(a != -1){ b = i; break; }
                        else{ a = i; }
                    }
                }
                neighbors[0][a] += 1; neighbors[1][a] += 1; neighbors[2][a] += 1;
                neighbors[6][a] -= 1; neighbors[7][a] -= 1; neighbors[8][a] -= 1;
                neighbors[0][b] += 1; neighbors[3][b] += 1; neighbors[6][b] += 1;
                neighbors[2][b] -= 1; neighbors[5][b] -= 1; neighbors[8][b] -= 1;
                break;
            }
            case 2:
            {
                int a = 0, b = -1, c = 0;
                for(int i=0; i<9; i++){ neighbors.emplace_back(x+diff[0], y+diff[1], z+diff[2]); }
                for(int i=0; i<3; i++){
                    if(diff[i] == 0){ a = i; }
                    else if(b != -1){ c = i; }
                    else{ b = i; }
                }
                neighbors[0][a] += 1; neighbors[1][a] += 1; neighbors[2][a] += 1;
                neighbors[6][a] -= 1; neighbors[7][a] -= 1; neighbors[8][a] -= 1;
                neighbors[0][b] -= diff[b]; neighbors[3][b] -= diff[b]; neighbors[6][b] -= diff[b]; 
                neighbors[2][c] -= diff[c]; neighbors[5][c] -= diff[c]; neighbors[8][c] -= diff[c]; 
                break;
            }
            case 3:
            {
                neighbors.emplace_back(x+diff[0], y+diff[1], z+diff[2]);
                neighbors.emplace_back(x, y+diff[1], z+diff[2]);
                neighbors.emplace_back(x+diff[0], y, z+diff[2]);
                neighbors.emplace_back(x+diff[0], y+diff[1], z);
                neighbors.emplace_back(x, y, z+diff[2]);
                neighbors.emplace_back(x+diff[0], y, z);
                neighbors.emplace_back(x, y+diff[1], z);
                neighbors.emplace_back(x-diff[0], y+diff[1], z+diff[2]);
                neighbors.emplace_back(x+diff[0], y-diff[1], z+diff[2]);
                neighbors.emplace_back(x+diff[0], y+diff[1], z-diff[2]);
                key++; min++; min2++;
                break;
            }
            default:
            break;
        }

        // Find the path of least resistance
        for(int i=0; i<key; i++){
            if(lightGrid[neighbors[i][0]][neighbors[i][0]][neighbors[i][0]] || neighbors[i][0] < 0 || neighbors[i][0] >= hei ||
               neighbors[i][1] < 0 || neighbors[i][1] >= wid || neighbors[i][2] < 0 || neighbors[i][2] >= dep){
                neighbors[i--].swap(neighbors[--key]);
            }
            else if(tag && (neighbors[i][0] - x) < 1){
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
                    if((piece != 3 && min == 9) || (piece == 3 && min == 10) || neighborVal[i] > neighborVal[min]){ min = i; }
                }
            }
        }
        if((piece != 3 && min == 9) || (piece == 3 && min == 10)) {
            auto ending = make_tuple(x, y, z);
            branches.push_back(ending);
            return;
        } // End of branch

        // Check for branching opportunities
        for(int i=0; i<key; i++){
            if(i != min && neighborVal[i] >= neighborVal[min]-branch){
                if((piece != 3 && min2 == 9) || (piece == 3 && min2 == 10) || neighborVal[i] > neighborVal[min2]){ min2 = i; }
            }
        }

        // Next Step
        prevxyz[0] = x; prevxyz[1] = y; prevxyz[2] = z;
        canonVertices.emplace_back(x, y, z);
        canonVertices.emplace_back(neighbors[min][0], neighbors[min][1], neighbors[min][2]);
        if((piece != 3 && min2 == 9) || (piece == 3 && min2 == 10)){ // No branching
            x = neighbors[min][0];
            y = neighbors[min][1];
            z = neighbors[min][2];
        }
        else{                                                        // Yes branching >:)
            traverse(neighbors[min][0], neighbors[min][1], neighbors[min][2], prevxyz);
            canonVertices.emplace_back(x, y, z);
            canonVertices.emplace_back(neighbors[min2][0], neighbors[min2][1], neighbors[min2][2]);
            traverse(neighbors[min2][0], neighbors[min2][1], neighbors[min2][2], prevxyz);
            return;
        }

    } // end while loop

}

void Lightning::superTraverse(){
    vector<vector<int>> origins;
    float originVal[9] = {0};
    int min = 9, key = 0, x = 0, y = 0, z = 0;
    int midwid = wid/2, middep = dep/2;

    // Start of lightning at top center of screen
    lightGrid[0][midwid][middep] = true;
    canonVertices.emplace_back(0, midwid, middep);
    lightPoints++;
    for(int i=0; i < 9; i++){ origins.emplace_back(0, midwid, middep); }
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
    canonVertices.emplace_back(x, y, z);

    traverse(x, y, z, prevxyz);

    // Traversing loop
    do{
        // Find lowest and closest to center lightning point
        x = -1; y = -1; z = -1;
        for(int i = hei-1; i >= 0; i--){
            for(int j = wid-1; j >= 0; j--){
                for(int k = dep-1; k >= 0; k--){
                    if(lightGrid[i][j][k]){ 
                        x = i; 
                        if(sqrt((j-midwid)^2 + (k-middep)^2) < sqrt((y-midwid)^2 + (z-middep)^2)){ y = j; z = k; }
                    }
                }
            }
            if(x != -1){ break; }
        }

        // Lowest point isn't low enough
        if(x < hei*forcedHeight){
            // If lowest point is a branch ending, delete branch
            int dex = branches.size();
            for(int i=0; i < branches.size(); i++){
                if(get<0>(branches[i]) == x && get<1>(branches[i]) == y && get<2>(branches[i]) == z){ dex = i; }
            }
            if(dex != branches.size()){
                branches[dex].swap(branches.back());
                branches.pop_back();
            }

            // Find its estranged dad
            for(int i=canonVertices.size(); i>0; i--){
                if(i%2 == 1){
                    if(canonVertices[i][0] == x && canonVertices[i][1] == y && canonVertices[i][2] == z){
                        for(int j=0; j<3; j++){ prevxyz[j] = canonVertices[i-1][j]; }
                        break;
                    }
                }
            }

            // Force to continue going down
            randGrid[x][y][z] = 2;
            canonVertices.emplace_back(prevxyz[0], prevxyz[1], prevxyz[2]);
            canonVertices.emplace_back(x, y, z);
            traverse(x, y, z, prevxyz, true);
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
    tuple<int, int, int> mainB = make_tuple(0, 0, 0);
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
            //antX = grid[currX][currY].getPrevX();
            //antY = grid[currX][currY].getPrevY();
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
        //antX = grid[currX][currY].getPrevX();
        //antY = grid[currX][currY].getPrevY();
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