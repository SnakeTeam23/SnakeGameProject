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

vector<position> vbuff_item;
vector<position> vnerf_item;
position buff_position(0,0);
position nerf_position(0,0);

void updateBuff(int stage){ 
    buff_position = vbuff_item.back();
    map[stage][buff_position.y][buff_position.x] = 3;
}

void updateNerf(int stage){
    nerf_position = vnerf_item.back();
    map[stage][nerf_position.y][nerf_position.x] = 6;
}

void makeNerf(int stage, WINDOW *win1){
    nodelay(win1, true);    
    while(1){
        if(map[stage][buff_position.randomPosition().y][buff_position.randomPosition().x] != 0){
            buff_position.randomPosition();
        }
        else break;
    }
    vbuff_item.push_back(buff_position);
    updateBuff(stage);
}

void makeBuff(int stage, WINDOW *win1){ 
    nodelay(win1, true);
    while(1){
        if(map[stage][nerf_position.randomPosition().y][nerf_position.randomPosition().x] != 0){
            nerf_position.randomPosition();
        }
        else break;
    }
    vnerf_item.push_back(nerf_position);
    updateNerf(stage);
}

void removeBuff(int stage, WINDOW *win1){ //추가 아이템 숨기기
    nodelay(win1, true);
    if(vbuff_item.empty() == 0 ){
        buff_position = vbuff_item.back();
        map[stage][buff_position.y][buff_position.x] = 0;
        vbuff_item.pop_back();
    }
}

void removeNerf(int stage, WINDOW *win1){//독 없애기
    nodelay(win1, true);
    if(vbuff_item.empty() == 0 ){
        nerf_position = vnerf_item.back();
        map[stage][nerf_position.y][nerf_position.x] = 0;
        vnerf_item.pop_back();
    }
}

