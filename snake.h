#include <iostream>
#include <vector>
#include <string.h>
#include <cstdlib>
#include "point.h"
#include "position.h"
using namespace std;

const int snakeMaxLen = 8;

class Snake{
private:
	vector <Point> snake_vec;
	Point direction;
	const int row, col;
	vector <Point> wall;
	Point gate[2];
	bool end;
	int speed;
	char* map_list;
	int gateCnt;
	int level;
	int snakeLen;
public:
	Snake(int r, int c);
	~Snake();
	int buffItem=0;
	int nerfItem=0;
	void setDirection(int d);
	void mvSnakeHead(int map[40][50]);
	void mvSnakeBody();
	char* setMaptoList(int map[40][50]);
	void setEnd(bool e);
	int gateDirection(Point gate, int map[40][50]);
	char getDirection();
	bool isEnd();
	int getSpeed();
	int getRow();
	int getCol();
	
	void setGate(int map[40][50]);
	void rmGate(int map[40][50]);
	void setGateCnt(int i);
	int getGateCnt();
	int getLevel();
	int getSnakeLen();
	
	int getSize();
	void setLevel(int new_level);
	void decreaseSnake(WINDOW *win1);
	void breakItem(WINDOW *win1);
	position plusHead();
	void resize(int new_size);
	void changeSnakeLen();
};
