#include "Lightning.h"

using namespace std;

Field::Field(void){
    this->len = 40;
    this->wid = 40;
}
Field::~Field(void){
    cout << "Porno" << endl;
}

void Field::setLen(int len){ this->len = len; }
void Field::setWid(int wid){ this->wid = wid; }
int Field::getLen(void){ return len; }
int Field::getWid(void){ return wid; }

void Field::randomize(void){
    srand(time(NULL));
    float randy = 0;
    for(int i=0; i<wid; i++){
        for(int j=0; j<len; j++){
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