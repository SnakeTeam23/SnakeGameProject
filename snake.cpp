#include "snake.h"
using namespace std;

extern int map[4][40][50];
extern void makeNerf(int stage, WINDOW *win1);
extern void makeBuff(int stage, WINDOW *win1);
extern void removeBuff(int stage, WINDOW *win1);
extern void removeNerf(int stage, WINDOW *win1);

extern WINDOW *win1;

Snake::Snake(int r, int c) : row(r), col(c){
		for(int i=0; i<3; i++)
			snake_vec.push_back(Point(col/2, row/2+i));
		setDirection(0);
		end = false;
		speed = 120000;
		map_list = new char[row*col];
		level=1;
		snakeLen=3;
		setGateCnt(0);
	}

//소멸자
Snake::~Snake(){ delete [] map_list; }

void Snake::setDirection(int d){ //스네이크의 방향을 설정하는 함수 구현
	switch(d)
	{
		case 0 : direction = Point(0, -1); break;
		case 1 : direction = Point(1, 0); break;
		case 2 : direction = Point(0, 1); break;
		case 3 : direction = Point(-1, 0); break;
	}
}

int Snake::gateDirection(Point gate, int map[40][50]){ //게이트에 따라 뭔가를 설정
	Point A = gate + Point(0,-1) ;
	if(map[A.getY()][A.getX()]==0) return 0;
	Point B = gate + Point(1,0) ;
	if(map[B.getY()][B.getX()]==0) return 1;
	Point C = gate + Point(0,1) ;
	if(map[C.getY()][C.getX()]==0) return 2;
	Point D = gate + Point(-1,0) ;
	if(map[D.getY()][D.getX()]==0) return 3;

	return -1;
}


void Snake::mvSnakeHead(int map[40][50]){ //스네이크를 설정한 방향으로 바꿈
	snake_vec[0] += direction; //가는 방향으로 계속 증가하게
	for(int i=0; i<wall.size(); i++) { //벽의 벡터만큼 for문
		if(snake_vec[0] == wall[i]) { //벽일경우
			if(snake_vec[0] == gate[1]) { //gate[1]과만나면
				snake_vec[0].setX(gate[0].getX()); //snake의 head부분을 gate[0]위치로 변경
				snake_vec[0].setY(gate[0].getY());
				setDirection(gateDirection(gate[0], map)); //snake의 head 의 방향 바꾸어줌
				setGateCnt(1);
				break;
			}

			else if(snake_vec[0] == gate[0]) { //gate[0]과 만나면
				snake_vec[0].setX(gate[1].getX());
				snake_vec[0].setY(gate[1].getY());
				setDirection(gateDirection(gate[1], map));
				setGateCnt(1);
				break;
			}

			else {
				setEnd(true); //gate가 아닌 벽을 만났을 경우는 exit을 true로 변경하고 게이트를 지움
				rmGate(map);
			}
		}
	}
}


void Snake::mvSnakeBody(){ //head에 따라 body도 함께 변경하게
	for(unsigned int i=snake_vec.size()-1; i>0; --i) snake_vec[i] = snake_vec[i - 1];
}


char* Snake::setMaptoList(int map[40][50]){ //2차원배열을 리스트로 변경함
	memset(map_list, ' ', row * col);
	for(unsigned int i=0; i<40; i++) {
		for(int j=0; j<50; j++){
			switch(map[i][j]) { //int를 캐릭터 값으로 변경 + 1차원 배열로 변경
				case 0 : map_list[i*col+j] = '0'; break;
				case 1 : //게이트가 생길 수 있는 벽일 경우
					map_list[i*col+j] = '1';
					wall.push_back(Point(j,i)); //wall벡터에 값 추가
					break;
				case 2 : map_list[i*col+j] = '2'; break;
				case 3 : map_list[i*col+j] = '3'; break;
				case 4 : map_list[i*col+j] = '4'; break;
				case 6 : map_list[i*col+j] = '6'; break;
				case 98 : map_list[i*col+j] = '8'; break;
				case 99 : map_list[i*col+j] = '9'; break;
			}
		}

	}
	map_list[snake_vec[0].getY()*col+snake_vec[0].getX()] = 'h'; //snake head의 위치 설정
	for(unsigned int i=1; i<snake_vec.size(); ++i)
		map_list[snake_vec[i].getY()*col+snake_vec[i].getX()] = 'b'; //snake body의 위치 설정
	return map_list;
}


char Snake::getDirection() { //방향 설정
	if(direction.getX()==1) return 'r'; //오른쪽
	else if(direction.getX()==-1) return 'l'; //왼쪽
	else if(direction.getY()==-1) return 'u'; //윗쪽
	else return 'd'; //아랫쪽
}


void Snake::setEnd(bool e) {end = e;}
bool Snake::getEnd() {return end;}
int Snake::getSpeed() {return speed;}
int Snake::getRow() {return row;}
int Snake::getCol() {return col;}

int Snake::getLevel() {return level;}
int Snake::getSnakeLen(){return snakeLen;}


void Snake::setGate(int map[40][50]) { //gate설정
	int randWall = rand() % wall.size(); //랜덤함수 추출
	int randWall2 = rand() % wall.size();
	if(randWall == randWall2) setGate(map); //추출한 두 랜덤 값이 같으면 다시 호출
	gate[0] = wall[randWall]; //벡터에 대입
	gate[1] = wall[randWall2];
	map[gate[0].getY()][gate[0].getX()] = 98; //map표시를 위해 바꾸어줌
	map[gate[1].getY()][gate[1].getX()] = 99;
}


void Snake::rmGate(int map[40][50])
{
	map[gate[0].getY()][gate[0].getX()] = 1; //전 gate의 map 다시 바꾸어줌
	map[gate[1].getY()][gate[1].getX()] = 1;
	gate[0].setX(0);
	gate[0].setY(0);
	gate[1].setX(0);
	gate[1].setY(0);
}

void Snake::setGateCnt(int i) {
	if (i==0){
		gateCnt = 0;
	}
	else{
		gateCnt += 1;
	}
}
int Snake::getGateCnt() {return gateCnt;}

//item
int Snake::getSize(){return snake_vec.size();}
void Snake::setLevel(int new_level){
	level = new_level;
}

void Snake::decreaseSnake(WINDOW *win1){
	removeNerf(level-1,win1);
	snake_vec.pop_back();
	makeBuff(level-1,win1);
}

void Snake::breakItem(WINDOW *win1){
	removeBuff(level -1,win1);
	snake_vec.push_back(snake_vec.back());
	makeNerf(level-1,win1);
}

position Snake::plusHead(){ //머리 position 타입으로 바꿔주기
	position head(snake_vec[0].getX(), snake_vec[0].getY());
	return head;
}

void Snake::resize(int new_size){
	snake_vec.resize(new_size);
}

void Snake::changeSnakeLen(){snakeLen = snake_vec.size();}
