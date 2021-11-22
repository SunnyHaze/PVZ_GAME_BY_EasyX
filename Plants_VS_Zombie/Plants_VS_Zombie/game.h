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
//�������ݽṹ
#include <vector>
#include <list>
#include <map>
#include <functional>
//���ڲ����Զ��������࣬��װ��һЩ������
#include "pageItem.h"

//�����࣬��һ�ֹ���Ļ���λ����Ϣ���ٶ���Ϣ��¼
class monster{
public:
	float cnt = 0; 
	int blood = 10;
	float step = 0.4;
	float x=1000, y=100;
	float width, height = 100;
	monster(float inx, float iny){
		x = inx;
		y = iny;
	}
	void startup(){
		;
	}
	bool operator==(monster obj){
		return x <= -100 || blood <= 0;
	}
};
//�����б����ڹ���һ�ֹ����ڽ����ϵ���Ⱦ����ʧ
class List_monster{
public:
	const static float rowPixY[5];
	int fps = 60;
	long long timer = 0;
	int tmpTimer = 0;
	IMAGE img[8];
	int animationSize = 8;
	std::list<monster> list;
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
		for (auto &i : list){
			if (i.x < -100){
				list.remove(i);
				break;
			}
			if (i.blood <= 0){
				list.remove(i);
				break;
			}
		}
	}
}; 
const float List_monster::rowPixY[5] = { 90, 220, 350, 480, 630 }; //ȷ������Ӧ����Ⱦ��y������

//��Ƭλ��
class card{
public:
	float width = 63, height = 84;
	std::string imgdir;
	std::string tardir;
	std::string name;
	float x, y;
	card(char imgDir[], char targetDir[],float inx,float iny, char cardname[]){
		x = inx;
		y = iny;
		imgdir = std::string(imgDir);
		tardir = std::string(targetDir);
		name = std::string(cardname);
	}
};
class cardSlot{
public:
	const static float cardPosiX[9];
	IMAGE cards[9];
	IMAGE target[9];
	int cnt = 0;
	std::vector<card> lst;
	void addCard(char cardPath[], char targetPath[], char plantname[]){
		lst.push_back(card(cardPath, targetPath,cardPosiX[cnt++],13, plantname));
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
const float cardSlot::cardPosiX[9] = { 120, 190, 260, 330, 400, 470, 540, 610, 680 };//���ʵ�y�᷽����13

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

class sun{
public:
	IMAGE *img;
	float x, y, destx, desty,speed = 2;
	float width = 80, height = 86;
	float finalx = 50, finaly = 50;
	Mouse *m;
	int status = 0;
	sun(IMAGE *image, Mouse *mouse){
		img = image;
		destx = 100 + rand() % 800;
		desty = 100 + rand() % 450;
		x = destx;
		y = -100;
		m = mouse;
	}
	void draw(){
		switch (status){
		case 0:
			if (m->isInArea(x, y, y + height, x + width) && m->LEFTDOWN){
				status = 1;
			}
			if (y <= desty){
				y += speed;
			}
			putimagePng(x, y, img);
			break;
		case 1:
			if (x <= finalx && y <= finaly){
				status = 2;
			}
			else{
				int k = 10;
				x -= speed * (x) / (x + y) * k;
				y -= speed * (y) / (x + y) * k;
			}
			putimagePng(x, y, img);
			break;
		default:
			break;
		}
	}
	bool operator==(sun obj){
		return status == 2;
	}
};

class List_sun{
public:
	Mouse *m;
	IMAGE img;
	int count = 0;
	char str_count[8];
	int time_cnt = 0;
	int step = 1200;
	std::list<sun> lst;
	void startup(){
		loadimage(&img, "\images\\sun.png");
	}
	void sunGenerator(){
		time_cnt++;
		if (time_cnt >= step){
			lst.push_back(sun(&img,m));
			time_cnt = 0;
			step = 1000 + rand() % 500;
		}
		time_cnt++;
	}
	void draw(){
		sprintf(str_count, "%d", count);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(55, 83, str_count);
		for (auto &i : lst){
			i.draw();
		}
		for (auto &i : lst){
			if (i.status == 2)
			{
				count += 25;
			 lst.remove(i);
				break;
			}
		}
	}
};
//�ӵ��࣬����ȷ�������ӵ������ʣ�
class bullet{
public:
	float x, y;
	float step = 4;
	float width = 58, height = 57;
	int status = 0;
	IMAGE * img;
	bullet(float inx, float iny, IMAGE* image){
		x = inx;
		y = iny;
		img = image;
	}
	void draw(){
		switch (status){
		case 0:
			putimagePng(x, y, img);
			x += step;
			if (x > 1024)
				status = 1;
			break;
		case 1:
			;
			break;
		}
	}
	bool operator==(bullet obj){
		return status == 1;
	}
};

class List_bullet{
const static int rowPixY[6];
public:
	int cnt = 0;
	int step = 100;
	std::list<bullet> mylst;
	IMAGE img;
	void addbullet(float x, float y){
		mylst.push_back(bullet(x, y, &img));
	}
	void testBullet(){
		cnt++;
		if (cnt == step){
			for (int i = 0; i < 5; i++){
				addbullet(0, List_bullet::rowPixY[i]);
			}
			cnt = 0;
		}
	}
	void startup(){
		loadimage(&img, "\images\\bullet.png");
	}
	void draw(){
		for (auto &i : mylst){
			i.draw();
		}
		for (auto &i : mylst){
			if (i.status == 1){
				mylst.remove(i);
				break;
			}
		}
	}
};
const int List_bullet::rowPixY[6] = { 115, 240, 370, 490, 640, 750 };
//ֲ���࣬���ڶ���ֲ��ʵ��
class plant{
	static std::string name;
	IMAGE *img;
	float x, y;
	int cnt = 0;
	plant(int inx, int iny){
		x = inx;
		y = iny;
	}
	void draw(){
		putimagePng(x, y, img);
		cnt++;
		//Todo
	}
};

//ֲ������ڹ������е�ֲ�Ｐ����Ϊ
class List_plant{
	std::map<std::string, std::function<plant(int,int)>> mpPlant;
	std::map<std::string, IMAGE> mpImg;
	void startup(){
		//̫��������

		//�㶹���ֲ���
		//mpPlant.insert(std::pair<std::string, std::function<plant(int, int)>>(std::string("pee"), plant()));
		//mpImg.insert(std::pair<)
	}
};

class statusCounter{
public:
	int code = 0;
	cardSlot * slot;
	Mouse m;
	IMAGE *pointImg;
	chessBoard *board;
	List_sun *listSun;
	statusCounter(cardSlot *cardslot, chessBoard *chessb, List_sun *listofsun){
		slot = cardslot;
		board = chessb;
		listSun = listofsun;
		//��ʼ����������
	}
	void startup(){
		listSun->m = &m;
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
const int  chessBoard::rowPixY[6] = { 113, 222, 339, 473, 600, 726 };
const int chessBoard::colPixX[10] = { 40, 145, 242, 354, 460, 566, 673, 781, 886, 970 };
//��Ϸ�����������ռ�

class hitEvent{
public:
	std::vector<List_monster*> mons;
	List_bullet *bull;
	void addMonsterList(List_monster *obj){
		mons.push_back(obj);
	}
	void setBulletList(List_bullet *obj){
		bull = obj;
	}

	void draw(){
		for (auto &i : mons){
			for (auto &m : i->list){ //j��monsters����
				for (auto &b : bull->mylst){
					//printf("%d%d%d\n", m.y < b.y, m.y + m.height > b.y, b.x + b.width > m.x);
					if (m.y < b.y && m.y + m.height > b.y && b.x + b.width > m.x){ //m.y < b.y && m.y + m.height > b.y + b.height &&
						b.status = 1;
						m.blood--;
					}
				}
			}
		}
	}
};

namespace game{
	IMAGE background;
	List_monster chaoxing("\images\\superstar\\superstar",rand() % 10);
	List_monster dingding("\images\\dingding\\dingding",rand() % 10);
	List_sun listSun;
	cardSlot slotCard;
	chessBoard board;
	statusCounter status(&slotCard, &board, &listSun);
	List_bullet listBullet;
	hitEvent hitEvt;
	void startup(){
		//���ر���ͼƬ
		loadimage(&background, _T("\images\\background.png"));
		//���ع����ز�
		chaoxing.startup();
		dingding.startup();
		listSun.startup();
		status.startup();
		//���ؿ�������
		for (int i = 0; i < 9; i++){
			slotCard.addCard("\images\\card.png", "\images\\plant1.png","pea");
		}
		slotCard.startup();

		//��ʼ����������
		board.startup();
		listBullet.startup();

		//��ʼ��hitEvent
		hitEvt.addMonsterList(&chaoxing);
		hitEvt.addMonsterList(&dingding);
		hitEvt.bull = &listBullet;
	}

	void draw(int *page){
		status.m.update();
		putimagePng(0, 0, &background);
		listSun.sunGenerator();
		slotCard.draw();
		status.trackStatus();
		listSun.draw();
		chaoxing.randomMonsterGenerator();
		dingding.randomMonsterGenerator();
		board.draw();

		listBullet.testBullet();
		hitEvt.draw();
		listBullet.draw();
	}
}