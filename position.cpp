#include "position.h"
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

position& position::randomPosition(){
    srand(time(NULL));
    this ->x = rand() % 50;
    this ->y = rand() % 40;
    return *this;
}

//operator
position& position::operator=(const position& v){
    this -> x = v.x;
    this -> y = v.y;
    return *this;
}

bool position::operator==(const position v){
    if((x == v.x) && ( y == v.y))
        return true;
    else
        return false;
}
