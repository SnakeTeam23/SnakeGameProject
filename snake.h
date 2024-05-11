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
	int apple = 0;
	int  poison = 0;
	void setDirection(int d);
	void move_snake_head(int map[40][50]);
	void move_snake_body();
	char* change_List(int map[40][50]);
	void setEnd(bool e);
	int gateDirection(Point gate, int map[40][50]);
	char getDirection();
	bool isEnd();
	int getSpeed();
	int getRow();
	int getCol();
	
	void initGate(int map[40][50]);
	void remove_gate(int map[40][50]);
	void set_gate_pass_cnt(int i);
	int get_gate_pass_cnt();
	int get_level();
	int get_snake_length();
	
	int get_size();
	void set_level(int new_level);
	void decrease_snake(WINDOW *win1);
	void break_item(WINDOW *win1);
	position plus_head();
	void resize(int new_size);
	void change_snake_length();
};
