#include <string>
#include <vector>
#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
using namespace std;

class position{
public:
    int x;
    int y;
    
    position& randomPosition();
    position& operator=(const position& p);
    bool operator==(const position p);

    position();
    position(int tempx, int tempy);
};
