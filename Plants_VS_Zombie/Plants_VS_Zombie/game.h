#include "stdafx.h"
#include <graphics.h>
#include "EasyXPng.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <stdlib.h>
#include <stdio.h>
#include <ctime>
#include <vector>
#include "pageItem.h"

class monster{
public:
	float cnt = 0;
	float step = 0.4;
	float x=1000, y=100;
	float width, height;
	monster(float inx, float iny){
		x = inx;
		y = iny;
	}
	void startup(){
		;
	}

};

class List_monster{
public:
	const static float rowPixY[5];
	int fps = 60;
	long long timer = 0;
	int tmpTimer = 0;
	IMAGE img[8];
	int animationSize = 8;
	std::vector<monster> list;
	std::string img_dir;
	int maxBlood = 10;
	List_monster(char imgdir[],int time = 0){
		img_dir = std::string(imgdir);
		timer = time;
	}
	void startup(){
		for (int i = 0; i < animationSize; i++){
			std::string path = img_dir;
			path.append("0");
			path[path.size() - 1] += i;
			path.append(".png");
			loadimage(&img[i], _T(path.c_str()));
		}
	}
	void addMonster(int idxRow){
		list.push_back(monster(1100, rowPixY[idxRow]));
	}
	void randomMonsterGenerator(){
		int separ = 5 + rand() % 4;
		if (timer % separ == 0 && tmpTimer == 0){
			addMonster(rand() % 5);
		}
		draw();
	}

	void draw(){
		tmpTimer++;
		if (tmpTimer == fps){
			timer++;
			tmpTimer = 0;
		}
		for (auto &i : list){
			putimagePng(i.x, i.y, &img[(int)i.cnt]);
			i.cnt += i.step;
			if (i.cnt >= animationSize){
				i.cnt = 0;
			}
			i.x--;
		}
	}
}; 
const float List_monster::rowPixY[5] = { 90, 220, 350, 480, 630 };

class card{
public:
	float width = 63, height = 84;
	std::string imgdir;
	std::string tardir;
	float x, y;
	card(char imgDir[], char targetDir[],float inx,float iny){
		x = inx;
		y = iny;
		imgdir = std::string(imgDir);
		tardir = std::string(targetDir);
	}
};
class cardSlot{
public:
	const static float cardPosiX[9];
	IMAGE cards[9];
	IMAGE target[9];
	int cnt = 0;
	std::vector<card> lst;
	void addCard(char cardPath[], char targetPath[]){
		lst.push_back(card(cardPath, targetPath,cardPosiX[cnt++],13));
	}
	void startup(){
		for (int i = 0; i < lst.size(); i++){
			loadimage(&cards[i], lst[i].imgdir.c_str());
			loadimage(&target[i], lst[i].tardir.c_str());
		}
	}
	void draw(){
		for (int i = 0; i < lst.size(); i++){
			putimagePng(lst[i].x, lst[i].y, &cards[i]);
		}
	}
};
const float cardSlot::cardPosiX[9] = { 120, 190, 260, 330, 400, 470, 540, 610, 680 };//合适的y轴方向是13

class block{
public:
	IMAGE img;
	int status = 0;
	float left, right, up, down;
	block(){
		;
	}
	block(float l, float r, float u, float d){
		left = l;
		right = r;
		up = u;
		down = d;
	}
	bool isInArea(float x, float y){
		return x > left &&  x <= right && y > up && y <= down;
	}
	void draw(){
		if (status != 0){
			putimagePng((left + right) / 2 - img.getwidth() / 2, (up + down) / 2 - img.getheight() / 2, &img);
		}
	}
	void setImage(char path[]){
		loadimage(&img, _T(path));
	}
};

class chessBoard{
public:
	const static int rowPixY[6];
	const static int colPixX[10];
	block data[5][9];
	chessBoard(){
		;
	}
	void startup(){
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 9; j++){
				data[i][j] = block(colPixX[j], colPixX[j + 1], rowPixY[i], rowPixY[i + 1]);
				data[i][j].setImage("\images\\plant_trans.png");
			}
		}
	}
	void draw(){
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 9; j++){
				data[i][j].draw();
			}
		}
	}
	bool inBoard(float x, float y){
		return x < 970 && x > 40 && y < 726 && y > 113;
	}
	void clearAll(){
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 10; j++){
				data[i][j].status = 0;
			}
		}
	}
};
const int  chessBoard::rowPixY[6] = { 113, 222, 339, 473, 600, 726 };
const int chessBoard::colPixX[10] = { 40,145,242, 354, 460, 566, 673, 781, 886, 970 };

class statusCounter{
public:
	int code = 0;
	cardSlot * slot;
	Mouse m;
	IMAGE *pointImg;
	chessBoard *board;
	statusCounter(cardSlot *cardslot, chessBoard *chessb){
		slot = cardslot;
		board = chessb;
	}
	void trackStatus(){
		switch (code)
		{
		case 0:
			for (int j = 0; j < slot->cnt; j++){
				card * i = &slot->lst[j];
				if (m.isInArea(i->x, i->y, i->y + i->height, i->x + i->width) && m.LEFTDOWN){
					code = 1;
					pointImg = &slot->target[j];
				}
			}
			board->clearAll();
			break;
		case 1:
			if (board->inBoard(m.x, m.y)){
				for (int i = 0; i < 5; i++){
					for (int j = 0; j < 10; j++){
						if (board->data[i][j].isInArea(m.x, m.y)){
							board->data[i][j].status = 1;
						}
						else{
							board->data[i][j].status = 0;
						}
					}
				}
			}
			else{
				putimagePng(m.x - slot->target->getwidth() / 2, m.y - slot->target->getheight() / 2, pointImg);
				board->clearAll();
			}
			if (m.RIGHTDOWN){
				code = 0;
			}
		default:
			break;
		}
	}
};

//游戏主界面命名空间
namespace game{
	IMAGE background;
	List_monster chaoxing("\images\\superstar\\superstar",rand() % 10);
	List_monster dingding("\images\\dingding\\dingding",rand() % 10);
	cardSlot slotCard;
	chessBoard board;
	statusCounter status(&slotCard, &board);
	void startup(){
		//加载背景图片
		loadimage(&background, _T("\images\\background.png"));
		
		//加载怪物素材
		chaoxing.startup();
		dingding.startup();

		//加载卡槽内容
		for (int i = 0; i < 9; i++){
			slotCard.addCard("\images\\card.png", "\images\\plant1.png");
		}
		slotCard.startup();

		//初始化棋盘网格
		board.startup();
	}

	void draw(int *page){
		status.m.update();
		putimagePng(0, 0, &background);
		chaoxing.randomMonsterGenerator();
		dingding.randomMonsterGenerator();
		slotCard.draw();
		status.trackStatus();
		board.draw();
	}
}