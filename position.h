#include <string>
#include <vector>
#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
using namespace std;

class position{
public:
    //attributes
    int x;
    int y;

    //Constructor
    position();
    position(int tempx, int tempy);
    
    //fuction & opertor
    position& randomPosition();
    position& operator=(const position& v);
    bool operator==(const position v);
};
