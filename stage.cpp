#include "snake.h"
#include <iostream>
#include <string>
#include <cstring>
#include <vector>
#include <cstdlib>
#include <ncurses.h>
#include <curses.h>
#include <string>
#include <unistd.h>
using namespace std;

int mapCnt = 0;
int buffCnt = 0;
int nerfCnt = 0;
int mResetCnt = 0;
int mResetDuration = 0;


extern int map[4][40][50];
extern int map[4][40][50];
extern void createBuff(int stage, WINDOW *win1);
extern void createNerf(int stage, WINDOW *win1);
extern void createMreset(int stage, WINDOW *win1);
extern void removeNerf(int stage, WINDOW *win1);
extern void removeBuff(int stage, WINDOW *win1);
extern void removeMreset(int stage, WINDOW *win1);


extern vector<position> vpoison_item;
extern vector<position> vapple_item;
extern vector<position> vmReset_item;

char missionBody = 'X';
char missionBuff = 'X';
char missionNerf = 'X';
char missionGate = 'X';
bool mReset_spawnded = false;

#define MISSION_BODY_LENGTH 6
#define MISSION_GROWTH_CNT 3
#define MISSION_POISON_CNT 2
#define MISSION_GATE_CNT 1

// 함수 원형 선언
void start_game(float, float);
void game();
int levelUpScreen(float, float, int);
int userInput();
int finishGame(float, float);
void drawMap(WINDOW*, Snake&, char*, int, int);
void updateMap(Snake& snake, int map[40][50]);
void showScore(WINDOW*, int, int, int, int, int);
void showMission(WINDOW*, int);
int levelUpScreen(float, float, int);
void setMission(Snake&, WINDOW*);
void nextLevel(Snake&,WINDOW*);
int success(float, float);

bool isWin = false; // 성공 여부 알려주는 전역변수


int success(float y, float x){
	clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);
    printw("\n \nCongratulation!!! You Won the Game!!!!");
    return userInput();
}

int userInput(){
    int userInput = getch();
    refresh();
    endwin();
    clear();
    return userInput;
}

void start_game(float y, float x) { 
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);
    printw("Press Any button to start");
    userInput();
	levelUpScreen(0, 0, 1);
	game();
}


int finishGame(float y, float x){
    clear();
    initscr();
    noecho();
    getmaxyx(stdscr, y, x);
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
                    ch = '*';
                    break;
                case '2': //게이트가 생길 수 없는 벽
                    ch = '#';
                    break;
                case '4': //맵 테두리
                    ch = '-';
                    break;
				case 'h': //snake head
					ch = 'S';
					break;
				case 'b': //snake body
					ch = 'S';
					break;
				case '8': //gate1
					ch = 'G';
					break;
				case '9': //gate2
					ch = 'G';
					break;
				case '3': 
                    ch = 'O';
                    break;
                case '6': //nerf
                    ch = 'X';
                    break;
				case '5': //mReset
				    ch = 'R';
					break;
			}
			mvwaddch(win, y, x, ch);
		}
	}
	wrefresh(win);
}

void updateMap(Snake& snake, int map[40][50]) {
	snake.initGate(map); 
}

void showScore(WINDOW* w, int snakeLen, int level, int apple, int poison, int Gate){
	werase(w);
	wbkgd(w, COLOR_PAIR(level));
	wborder(w, '|','|','-','-','|','|','|','|');
	mvwprintw(w, 1, 1, "Score");
	mvwprintw(w, 2, 1, "Body length: %d/%d", snakeLen, snakeMaxLen);
	mvwprintw(w, 3, 1, "+: %d ", apple);
	mvwprintw(w, 4, 1, "-: %d ", poison);
	mvwprintw(w, 5, 1, "Gate: %d ", Gate);
	mvwprintw(w, 6, 1, "Level: %d ", level);
	wrefresh(w);
}

void showMission(WINDOW* w, int level){
  werase(w);
  wbkgd(w, COLOR_PAIR(level));
  wborder(w, '|','|','-','-','|','|','|','|');
  mvwprintw(w, 1, 1, "Mission");
  mvwprintw(w, 2, 1, "Body length: %d ( %c )", MISSION_BODY_LENGTH, missionBody);
  mvwprintw(w, 3, 1, "+: %d  ( %c )", MISSION_GROWTH_CNT, missionBuff);
  mvwprintw(w, 4, 1, "-: %d  ( %c )", MISSION_POISON_CNT, missionNerf);
  mvwprintw(w, 5, 1, "Gate: %d  ( %c )", MISSION_GATE_CNT, missionGate);
  wrefresh(w);
}

int levelUpScreen(float y, float x, int level){
  clear();
  initscr();
  noecho();
  getmaxyx(stdscr, y, x);
  clear();
  initscr();
  noecho();
getmaxyx(stdscr, y, x);
  if (level==1){
    printw("Level 1.\nPress Any key to Start Game...");
  }
  else{
    string lev = to_string(level);
    printw("Good Job!! You passed Now Level. Let's go to ");
    printw((const char* )lev.data());
	printw(" level.");
    printw("\nPress Enter to continue Game.");
  }
  return userInput();
}

void setMission(Snake& snake, WINDOW *win1){
  if(vapple_item.empty() ==0){
	  position head = snake.plus_head();
	  if(head == vapple_item.back()){
      snake.break_item(win1);
	  snake.change_snake_length();
	  snake.apple++;
	  }
  }

  if(vpoison_item.empty() == 0){
    position head = snake.plus_head();
    if(head == vpoison_item.back()){
      snake.decrease_snake(win1);
      snake.change_snake_length();
      snake.poison++;
    }
  }

  if(snake.get_size() == MISSION_BODY_LENGTH) {missionBody ='O';}
  if(snake.apple == MISSION_GROWTH_CNT) {missionBuff = 'O';}
  if(snake.poison == MISSION_POISON_CNT) {missionNerf = 'O';}
  if(snake.get_gate_pass_cnt() == MISSION_GATE_CNT) {missionGate = 'O';}
}

void nextLevel(Snake& snake,WINDOW *win1){
	if((missionBody == 'O')&&(missionGate=='O')&&(missionBuff=='O')&&(missionNerf=='O')){
		snake.claerWall();
		mapCnt = 0;
		buffCnt = 0;
		nerfCnt = 0;
		mResetDuration =0;
		mReset_spawnded = false;
		// snake.resize(3);
		snake.apple =0;
		snake.poison =0;
		snake.set_gate_pass_cnt(0);
		missionBody = 'X'; 
		missionBuff = 'X';
		missionNerf = 'X';
		missionGate = 'X';
		removeBuff(snake.get_level()-1,win1);
		removeNerf(snake.get_level()-1,win1);
		snake.set_level(snake.get_level()+1);
		if(snake.get_level() == 5){
			isWin = true;
			snake.setEnd(true);
		}
		else{
			snake.randomSpawn(map[snake.get_level() - 1]);
			if (levelUpScreen(0,0, snake.get_level()) == 13) {}; //엔터 누르면 다음 레벨로 게임 계속 진행
		}
	}
}

void game() { 
	float x, y;
	initscr();
	noecho();
	cbreak();

	start_color();
	init_pair(1, COLOR_WHITE, COLOR_MAGENTA);
	init_pair(2, COLOR_WHITE, COLOR_CYAN);
	init_pair(3, COLOR_WHITE, COLOR_YELLOW);
	init_pair(4, COLOR_WHITE, COLOR_RED);

	getmaxyx(stdscr, y, x);
	WINDOW *win1 = newwin(40, 50, 0, 0);
	box(win1, 0, 0);
	refresh();
    WINDOW *mission = newwin(10, 40, 12, 50);
    wrefresh(mission);
    
	WINDOW *scoreBoard = newwin(10, 40, 0, 50);
	wrefresh(scoreBoard);

	Snake snake(40, 50);

	while(!snake.isEnd()){ //exit가 true가 될때까지 반복문
		WINDOW *win1 = newwin(40, 50, 0, 0);
		showMission(mission, snake.get_level());
		showScore(scoreBoard, snake.get_snake_length(), snake.get_level(), snake.apple, snake.poison, snake.get_gate_pass_cnt());
	

		srand(time(NULL)); 
		char *map_table = snake.change_List(map[snake.get_level()-1]);
		wbkgd(win1, COLOR_PAIR(snake.get_level()));
		wattron(win1, COLOR_PAIR(snake.get_level()));
		nodelay(win1, TRUE);
		keypad(win1, TRUE);
		refresh();

		wrefresh(win1);
		drawMap(win1, snake, map_table, snake.getRow(), snake.getCol());
    	setMission(snake,win1);
		if (mapCnt == 0) {
			updateMap(snake, map[snake.get_level()-1]);
		}
		mapCnt+= 1;

		if (mapCnt >= 100 && !snake.isGate()) {
			snake.remove_gate(map[snake.get_level()-1]);
			updateMap(snake, map[snake.get_level()-1]);
			mapCnt = 1;
		}

		if (buffCnt == 0) {
			createBuff(snake.get_level()-1,win1);
		}
		buffCnt+= 1;

		if (buffCnt == 100) { // 15초에 한번씩 바뀝니다.
			removeBuff(snake.get_level()-1,win1);
			createBuff(snake.get_level()-1,win1);
			buffCnt = 1;
		}

		if (nerfCnt == 0) {
			createNerf(snake.get_level()-1,win1);
		}
		nerfCnt+= 1;

		if (nerfCnt == 70) { // 10.5초에 한번씩 바뀝니다.
			removeNerf(snake.get_level()-1,win1);
			createNerf(snake.get_level()-1,win1);
			nerfCnt = 1;
		}
		if((missionBody == 'O')||(missionGate=='O')||(missionBuff=='O')||(missionNerf=='O')){
			mResetDuration = 1;
		}


		if (mResetDuration !=0 && !mReset_spawnded) {
            createMreset(snake.get_level() - 1, win1);
            mResetCnt = 1;
			mResetDuration = 2;
			mReset_spawnded = true;
        }

		if(mReset_spawnded){
			mResetCnt+= 1;
		}

        if (mResetCnt == 100) {  // 약 15초 경과
            removeMreset(snake.get_level() - 1, win1);
            mResetCnt = 0;
			mResetDuration =3;			
        }

        position head = snake.plus_head();
        if (!vmReset_item.empty() && head == vmReset_item.back()) {
            // 미션 아이템 먹음
            missionBody = 'X'; 
		    missionBuff = 'X';
		    missionNerf = 'X';
		    missionGate = 'X'; 
			snake.resize(3);
		    snake.apple =0;
		    snake.poison =0;
		    snake.set_gate_pass_cnt(0);
            removeMreset(snake.get_level() - 1, win1);
			mResetCnt = 0;
			mResetDuration =4;
		}

		int input = wgetch(win1);
		char d = snake.getDirection();

		switch(input){
			case 'w':
				if(d !='u' && d !='d') snake.setDirection(0);
				else if (d =='d') snake.setEnd(true);
				break;

			case 's': 
				if(d !='d' && d !='u') snake.setDirection(2);
				else if (d =='u') snake.setEnd(true);
				break;

			case 'a':
				if(d != 'l' && d != 'r') snake.setDirection(3);
				else if (d == 'r') snake.setEnd(true);
				break;

			case 'd':
				if(d != 'r' && d != 'l') snake.setDirection(1);
				else if (d == 'l') snake.setEnd(true);
				break;    
		}

		if(snake.get_size() < 3) snake.setEnd(TRUE);
		snake.move_snake_body(); 
		snake.move_snake_head(map[snake.get_level()-1]);
        nextLevel(snake, win1);
		usleep(snake.getSpeed());
	}
}


int main(){
	int a = 0;
    start_game(0, 0);
	if(isWin) success(0, 0);
	else finishGame(0, 0);
	endwin();
	return 0;
}