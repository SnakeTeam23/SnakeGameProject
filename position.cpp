#include "position.h"
#define COL 50
#define ROW 40
using namespace std;

//Constructor
position::position(){
    x = 0;
    y = 0;
}

position::position(int tempx, int tempy){
    x = tempx;
    y = tempy;
}

position& position::operator=(const position& p){
    (*this).x = p.x;
    (*this).y = p.y;
    return *this;
}

position& position::randomPosition(){
    srand(time(NULL));
    (*this).x = rand() % COL;
    (*this).y = rand() % ROW;
    return *this;
}

bool position::operator==(const position p){
    if((x == p.x) && (y == p.y)) return true;
    return false;
}
