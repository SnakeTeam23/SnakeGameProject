#include "snake.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ncurses.h>
#include <string>
#include <unistd.h>
using namespace std;

extern int map[4][40][50];
extern int map[4][40][50];
extern void makeBuff(int stage, WINDOW *win1);
extern void makeNerf(int stage, WINDOW *win1);
extern void removeNerf(int stage, WINDOW *win1);
extern void removeBuff(int stage, WINDOW *win1);
extern void breakItem(WINDOW *win1);
extern vector<Point> decreaseSnake(WINDOW *win1);
extern position plusHead();
extern vector<position> vnerf_item;
extern vector<position> vbuff_item;

char missionBody = 'X';
char missionBuff = 'X';
char missionNerf = 'X';
char missionGate = 'X';

int num_missionBody = 6;
int num_missionGrowth = 2;
int num_missionPoison = 2;
int num_missionGate = 1;


void newWindow(float y, float x){
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);
}

int userInput(){
    int userInput = getch();
    refresh();
    endwin();
    clear();
    return userInput;
}

int startGame(float y, float x) { 
    newWindow(y,x);
    printw("Press s buttion to start");
    return userInput();
}


int finishWindow(float y, float x){
    newWindow(y,x);
    printw("\n \nGame Over! Press any button to finish");
    return userInput();
}
 
void drawMap(WINDOW* win, Snake& snake, char* table, int row, int col) 
{
	werase(win);

	for(int i=0; i<(row * col); i++){
		if(table[i]!=' '){
			int y = i / col;
			int x = i - (y*col);
			int ch;
			switch(table[i]){
				case '0': //스네이크가 움직일 수 있는 공간
                    ch = ' ';
                    break;
                case '1': //게이트가 생길 수 있는 벽
                    ch = '@';
                    break;
                case '2': //게이트가 생길 수 없는 벽
                    ch = '*';
                    break;
                case '4': //맵 테두리
                    ch = '=';
                    break;
				case 'h': //snake head
					ch = '&';
					break;
				case 'b': //snake body
					ch = '&';
					break;
				case '8': //gate1 임시표시
					ch = '?';
					break;
				case '9': //gate2 임시표시
					ch = '?';
					break;
				case '3': //buff
                    ch = '0';
                    break;
                case '6': //nerf
                    ch = 'X';
                    break;
			}
			mvwaddch(win, y, x, ch);
		}
	}
	wrefresh(win);
}

void updateMap(Snake& snake, int map[40][50]) {
	snake.setGate(map); 
}

void showScore(WINDOW* w, int snakeLen, int level, int buffItem, int nerfItem, int Gate){
	werase(w);
	wbkgd(w, COLOR_PAIR(level));
	wborder(w, '|','|','-','-','|','|','|','|');
	mvwprintw(w, 1, 1, "Score");
	mvwprintw(w, 2, 1, "Body length: %d/%d", snakeLen, snakeMaxLen);
	mvwprintw(w, 3, 1, "+: %d ", buffItem);
	mvwprintw(w, 4, 1, "-: %d ", nerfItem);
	mvwprintw(w, 5, 1, "Gate: %d ", Gate);
	mvwprintw(w, 6, 1, "Level: %d ", level);
	wrefresh(w);
}

void showMission(WINDOW* w, int level){
  werase(w);
  wbkgd(w, COLOR_PAIR(level));
  wborder(w, '|','|','-','-','|','|','|','|');
  mvwprintw(w, 1, 1, "Mission");
  mvwprintw(w, 2, 1, "Body length: 6 ( %c )", missionBody);
  mvwprintw(w, 3, 1, "+: 2  ( %c )", missionBuff);
  mvwprintw(w, 4, 1, "-: 2  ( %c )", missionNerf);
  mvwprintw(w, 5, 1, "Gate: 1  ( %c )" ,missionGate);
  wrefresh(w);
}

int levelUpScreen(float y, float x, int level){
  clear();
  initscr();
  noecho();
  getmaxyx(stdscr, y, x);
  newWindow(y,x);
  if (level==1){
    printw("Welcome to SnakeGame! \n Level 1");
  }
  else{
    string lev = to_string(level);
    printw("Let's go to next level! It's ");
    printw(lev.data());
	printw(" level.");
    printw("\nPress Enter button.");
  }
  return userInput();
}

void setMission(Snake& snake, WINDOW *win1){
  if(vbuff_item.empty() ==0){
	  position head = snake.plusHead();
	  if(head == vbuff_item.back()){
      snake.breakItem(win1);
	  snake.changeSnakeLen();
	  snake.buffItem++;
	  }
  }

  if(vnerf_item.empty() ==0){
    position head = snake.plusHead();
    if(head == vnerf_item.back()){
      snake.decreaseSnake(win1);
      snake.changeSnakeLen();
      snake.nerfItem++;
    }
  }

  if(snake.getSize() == num_missionBody) {missionBody ='O';}
  if(snake.buffItem == num_missionGrowth) {missionBuff = 'O';}
  if(snake.nerfItem == num_missionPoison) {missionNerf = 'O';}
  if(snake.getGateCnt() == num_missionGate) {missionGate = 'O';}
}

void nextLevel(Snake& snake,WINDOW *win1){
	if((missionBody == 'O')&&(missionGate=='O')&&(missionBuff=='O')&&(missionNerf=='O')){
		snake.resize(3);
		snake.buffItem =0;
		snake.nerfItem =0;
		snake.setGateCnt(0);
		missionBody = 'X'; 
		missionBuff = 'X';
		missionNerf = 'X';
		missionGate = 'X';
		removeBuff(snake.getLevel()-1,win1);
		removeNerf(snake.getLevel()-1,win1);
		snake.setLevel(snake.getLevel()+1);
		if (levelUpScreen(0,0, snake.getLevel()) == 13) {}; //엔터 누르면 다음 레벨로 게임 계속 진행
	}
}

void game() { 
	float x, y;
	initscr();
	noecho();
	cbreak();

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_BLUE);
	init_pair(2, COLOR_WHITE, COLOR_GREEN);
	init_pair(3, COLOR_WHITE, COLOR_YELLOW);
	init_pair(4, COLOR_WHITE, COLOR_RED);

	getmaxyx(stdscr, y, x);
	WINDOW *win1 = newwin(40, 50, 0, 0);
    WINDOW *mission = newwin(10, 40, 12, 50);
    wrefresh(mission);
    
	 WINDOW *scoreBoard = newwin(10, 40, 0, 50);
	wrefresh(scoreBoard);

	Snake snake(40, 50);
	int mapCnt = 0;
	int buffCnt = 0;
	int nerfCnt = 0;

	while(!snake.getEnd()){ //exit가 true가 될때까지 반복문
	WINDOW *win1 = newwin(40, 50, 0, 0);
	showMission(mission, snake.getLevel());
	showScore(scoreBoard, snake.getSnakeLen(), snake.getLevel(), snake.buffItem, snake.nerfItem, snake.getGateCnt());
	

	srand(time(NULL)); 
	char *map_table = snake.setMaptoList(map[snake.getLevel()-1]);
	wbkgd(win1, COLOR_PAIR(snake.getLevel()));
	wattron(win1, COLOR_PAIR(snake.getLevel()));
	nodelay(win1, TRUE);
	keypad(win1, TRUE);
	refresh();

	wrefresh(win1);
		drawMap(win1, snake, map_table, snake.getRow(), snake.getCol());
    setMission(snake,win1);
		if (mapCnt == 0) {
			(snake, map[snake.getLevel()-1]);
		}
		mapCnt+= 1;

		if (mapCnt == 100) {
			snake.rmGate(map[snake.getLevel()-1]);
			updateMap(snake, map[snake.getLevel()-1]);
			mapCnt = 1;
		}

		if (buffCnt == 0) {
			makeNerf(snake.getLevel()-1,win1);
		}
		buffCnt+= 1;

		if (buffCnt == 70) {
			removeBuff(snake.getLevel()-1,win1);
			makeNerf(snake.getLevel()-1,win1);
			buffCnt = 1;
		}

		if (nerfCnt == 0) {
			makeBuff(snake.getLevel()-1,win1);
		}
		nerfCnt+= 1;

		if (nerfCnt == 35) {
			removeNerf(snake.getLevel()-1,win1);
			makeBuff(snake.getLevel()-1,win1);
			nerfCnt = 1;
		}

		int input = wgetch(win1);
		char d = snake.getDirection();

		switch(input){
			case KEY_UP:
				if(d !='u' && d !='d') snake.setDirection(0);
				else if (d =='d') snake.setEnd(true);
				break;

			case KEY_DOWN: 
				if(d !='d' && d !='u') snake.setDirection(2);
				else if (d =='u') snake.setEnd(true);
				break;

			case KEY_LEFT:
				if(d != 'l' && d != 'r') snake.setDirection(3);
				else if (d == 'r') snake.setEnd(true);
				break;

			case KEY_RIGHT:
				if(d != 'r' && d != 'l') snake.setDirection(1);
				else if (d == 'l') snake.setEnd(true);
				break;    
		}

		if(snake.getSize() < 3) snake.setEnd(TRUE);
		snake.mvSnakeBody(); 
		snake.mvSnakeHead(map[snake.getLevel()-1]);
        nextLevel(snake, win1);
		usleep(snake.getSpeed());
	}
}


int main(){
    if (startGame(0, 0) == 's') game();
    finishWindow(0, 0);
	endwin();
	return 0;
}