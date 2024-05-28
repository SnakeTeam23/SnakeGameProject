#include <ncurses.h>
#include <stdlib.h>
#include <ctime>
#include <unistd.h>
#include <string>
#include <vector>
#include "position.h"
using namespace std;

extern int map[4][40][50];
extern WINDOW *win1;

vector<position> vapple_item;
vector<position> vpoison_item;
vector<position> vmReset_item;
position buff_position(0,0);
position nerf_position(0,0);
position mReset_position(0,0);

void createRandom(int mode, int stage);

void updateBuff(int stage){ 
    buff_position = vapple_item.back();
    map[stage][buff_position.y][buff_position.x] = 3;
}

void updateNerf(int stage){
    nerf_position = vpoison_item.back();
    map[stage][nerf_position.y][nerf_position.x] = 6;
}

void updateMreset(int stage) {
    mReset_position = vmReset_item.back();
    map[stage][mReset_position.y][mReset_position.x] = 5;
}

void createNerf(int stage, WINDOW *win1){
    nodelay(win1, true);
    createRandom(2, stage);
    vpoison_item.push_back(nerf_position);
    updateNerf(stage);
}

void createBuff(int stage, WINDOW *win1){ 
    nodelay(win1, true);    
    createRandom(1, stage);
    vapple_item.push_back(buff_position);
    updateBuff(stage);
}

void createMreset(int stage, WINDOW *win1) {
    nodelay(win1, true);
    createRandom(3, stage);  
    vmReset_item.push_back(mReset_position);
    updateMreset(stage);
    wrefresh(win1);
}


void removeBuff(int stage, WINDOW *win1){ //추가 아이템 숨기기
    nodelay(win1, true);
    if(vapple_item.empty() == 0 ){
        buff_position = vapple_item.back();
        map[stage][buff_position.y][buff_position.x] = 0;
        vapple_item.pop_back();
    }
}

void removeNerf(int stage, WINDOW *win1){//독 없애기
    nodelay(win1, true);
    if(vapple_item.empty() == 0 ){
        nerf_position = vpoison_item.back();
        map[stage][nerf_position.y][nerf_position.x] = 0;
        vpoison_item.pop_back();
    }
}

void removeMreset(int stage, WINDOW *win1) {//미션리셋아이템 없애기
    nodelay(win1, true);
    if (!vmReset_item.empty()) {
        mReset_position = vmReset_item.back();
        map[stage][mReset_position.y][mReset_position.x] = 0;
        vmReset_item.pop_back();
        wrefresh(win1);
    }
}



void createRandom(int mode, int stage){
    position* item_position = nullptr;
    switch (mode) {
        case 1: item_position = &buff_position; break;
        case 2: item_position = &nerf_position; break;
        case 3: item_position = &mReset_position; break;
    }
    
    while (true) {
        position temp_position = item_position->randomPosition();
        if (map[stage][temp_position.y][temp_position.x] != 0) {
            item_position->randomPosition();
        } else {
            *item_position = temp_position;
            break;
        }
    }
}