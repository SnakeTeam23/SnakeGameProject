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
		speed = 150000;
		map_list = new char[row*col];
		level=1;
		snakeLen=3;
		set_gate_pass_cnt(0);
	}

//소멸자
Snake::~Snake(){ delete [] map_list; }

void Snake::randomSpawn(int map[40][50]){
    bool validSpawn = false;
    int maxIterations = 1656; 
    int iterations = 0;
    int spawnX, spawnY;

    while (!validSpawn && iterations < maxIterations) {
        spawnX = rand() % (col - 20) + 10;
        spawnY = rand() % (row - 20) + 10;

        if (map[spawnY][spawnX] == 0 && 
            map[spawnY + 1][spawnX] == 0 && 
            map[spawnY + 2][spawnX] == 0) {

            validSpawn = true;
            snake_vec.clear();
            for (int i = 0; i < 3; i++) {
                snake_vec.push_back(Point(spawnX, spawnY + i));
            }
        }
        iterations++;
    }

    if (!validSpawn) {
        for (iterations = 0; iterations < maxIterations; ++iterations) {
            spawnX = rand() % (col - 10) + 5;
            spawnY = rand() % (row - 10) + 5;

            if (map[spawnY][spawnX] == 0 && 
                map[spawnY + 1][spawnX] == 0 && 
                map[spawnY + 2][spawnX] == 0) {
                snake_vec.clear();
                for (int i = 0; i < 3; i++) {
                    snake_vec.push_back(Point(spawnX, spawnY + i));
                }
                return;
            }
        }
    }
}


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
	int i = 0;
	switch(getDirection()){
		case 'u':
			i = 0;
			break;
		case 'r':
			i = 1;
			break;
		case 'd':
			i = 2;
			break;
		case 'l':
			i = 3;
			break;
	}
	Point points[4] = {Point(0, -1), Point(1, 0), Point(0, 1), Point(-1, 0)};
	Point A(0, 0);
	for(; i < 4; i = (i + 1) % 4){
		A = gate + points[i];
		if(map[A.get_y()][A.get_x()] == 0) return i;
	}
	return -1;
}


void Snake::move_snake_head(int map[40][50]){ //스네이크를 설정한 방향으로 바꿈
	snake_vec[0] += direction; //가는 방향으로 계속 증가하게
	for(int i=0; i<wall.size(); i++) { //벽의 벡터만큼 for문
		if(snake_vec[0] == wall[i]) { //벽일경우
			if(snake_vec[0] == gate[1]) { //gate[1]과만나면
				checkVisitWall(0, map);
				break;
			}

			else if(snake_vec[0] == gate[0]) { //gate[0]과 만나면
				checkVisitWall(1, map);
				break;
			}

			else {
				setEnd(true); //gate가 아닌 벽을 만났을 경우는 exit을 true로 변경하고 게이트를 지움
				remove_gate(map);
			}
		}
	}
}

void Snake::checkVisitWall(bool gateNum, int map[40][50]){
	snake_vec[0].set_x(gate[gateNum].get_x()); //snake의 head부분을 gate[0]위치로 변경
	snake_vec[0].set_y(gate[gateNum].get_y());
	setDirection(gateDirection(gate[gateNum], map)); //snake의 head 의 방향 바꾸어줌
	set_gate_pass_cnt(1);
}


void Snake::move_snake_body(){ //head에 따라 body도 함께 변경하게
	for(unsigned int i=snake_vec.size()-1; i>0; --i) snake_vec[i] = snake_vec[i - 1];
}


char* Snake::change_List(int map[40][50]){ //2차원배열을 리스트로 변경함
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
				case 8 : map_list[i*col+j] = '8'; break;
				case 9 : map_list[i*col+j] = '9'; break;
			}
		}

	}
	map_list[snake_vec[0].get_y()*col+snake_vec[0].get_x()] = 'h'; //snake head의 위치 설정
	for(unsigned int i=1; i<snake_vec.size(); ++i)
		map_list[snake_vec[i].get_y()*col+snake_vec[i].get_x()] = 'b'; //snake body의 위치 설정
	return map_list;
}


char Snake::getDirection() { //방향 설정
	if(direction.get_x()==1) return 'r'; //오른쪽
	else if(direction.get_x()==-1) return 'l'; //왼쪽
	else if(direction.get_y()==-1) return 'u'; //윗쪽
	else return 'd'; //아랫쪽
}


void Snake::setEnd(bool e) {end = e;}
bool Snake::isEnd() {return end;}
int Snake::getSpeed() {return speed;}
int Snake::getRow() {return row;}
int Snake::getCol() {return col;}

int Snake::get_level() {return level;}
int Snake::get_snake_length(){return snakeLen;}


void Snake::initGate(int map[40][50]) { //gate설정
	int randWall = rand() % wall.size(); //랜덤함수 추출
	int randWall2 = rand() % wall.size();
	if(randWall == randWall2) initGate(map); //추출한 두 랜덤 값이 같으면 다시 호출
	gate[0] = wall[randWall]; //벡터에 대입
	// gate[0] = Point(32, 8);
	gate[1] = wall[randWall2];
	// gate[1] = Point(48, 3);
	map[gate[0].get_y()][gate[0].get_x()] = 8; //map표시를 위해 바꾸어줌
	map[gate[1].get_y()][gate[1].get_x()] = 9;
}


void Snake::remove_gate(int map[40][50])
{
	map[gate[0].get_y()][gate[0].get_x()] = 1; //전 gate의 map 다시 바꾸어줌
	map[gate[1].get_y()][gate[1].get_x()] = 1;
	gate[0].set_x(0);
	gate[0].set_y(0);
	gate[1].set_x(0);
	gate[1].set_y(0);
}

void Snake::set_gate_pass_cnt(int i) {
	if (i==0){
		gateCnt = 0;
	}
	else{
		gateCnt += 1;
	}
}
int Snake::get_gate_pass_cnt() {return gateCnt;}

//item
int Snake::get_size(){return snake_vec.size();}
void Snake::set_level(int new_level){
	level = new_level;
}

void Snake::decrease_snake(WINDOW *win1){
	removeNerf(level-1,win1);
	snake_vec.pop_back();
	makeBuff(level-1,win1);
}

void Snake::break_item(WINDOW *win1){
	removeBuff(level -1,win1);
	snake_vec.push_back(snake_vec.back());
	makeNerf(level-1,win1);
}

position Snake::plus_head(){ //머리 position 타입으로 바꿔주기
	position head(snake_vec[0].get_x(), snake_vec[0].get_y());
	return head;
}

void Snake::resize(int new_size){
    std::vector<Point> newVector;
    newVector.push_back(snake_vec[0]);
    newVector.push_back(snake_vec[1]);
    newVector.push_back(snake_vec[2]);

    snake_vec = newVector;
}

void Snake::change_snake_length(){snakeLen = snake_vec.size();}
