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
	float width= 100, height = 100;
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

class picturesOFcard{ //��ΪEasyX��IMAGE��Ķ����ڿ�����ʱ��ᷢ���жϣ�������Ҫ��������
public:
	IMAGE cardimg;		//������ͼƬ��ַ
	IMAGE img[8];		//ʵ����ȾͼƬ��ַ
	IMAGE trans_img;	//��͸��ͼƬ��ַ
	std::string imgdir; //�����еĿ�Ƭ��·��
	std::string tardir; //ֲ��ʵ���·��
	int img_cnt_MAX;
	picturesOFcard(std::string inputImgdir, std::string inputTargetdir, int img_cnt_M){
		imgdir = inputImgdir;
		tardir = inputTargetdir;
		img_cnt_MAX = img_cnt_M;
	}

	void startup(){
		loadimage(&cardimg, imgdir.c_str()); // ���뿨Ƭ�ز�
		for (int i = 0; i < img_cnt_MAX; i++){
			std::string tmpPath(tardir);
			tmpPath.append("0");
			tmpPath[tmpPath.length() - 1] += i + 1;
			tmpPath.append(".png");
			loadimage(&img[i], _T(tmpPath.c_str()));
		}
		std::string tmp(tardir);
		loadimage(&trans_img, tmp.append("_trans.png").c_str()); //�����͸��ͼƬ
	}

	void drawTransparent(int x, int y){
		putimagePng(x - trans_img.getwidth() / 2, y - trans_img.getheight() / 2, &trans_img);
	}
};

class statusCounter;
//��Ƭλ��
class card{
public:
	float width = 63, height = 84;
	float x, y;
	std::string name;   //��Ƭ��
	int cardID;         //��ƬID
	picturesOFcard *pics;
	int blood;			//Ѫ��
	int cost;			//����������
	void(*action)(statusCounter *obj,int,int);  //******�ǳ���Ҫ******���ڿ���ĳ��ֲ����Ϊ�ĺ���ָ�룬��Ҫ��ָ�����������
	card(picturesOFcard *picsPtr, float inx, float iny, char cardname[], int id, int inputBlood, int inputCost){
		pics = picsPtr;
		x = inx;
		y = iny;
		name = std::string(cardname);
		cardID = id;
		blood = inputBlood;
		cost = inputCost;
	}

	void startup(){
		pics->startup();
	}
};

class cardSlot{
public:
	const static float cardPosiX[9];
	int cnt = 0;
	std::vector<card> lst;
	void addCard(picturesOFcard *picsPtr, char plantname[], int ID, int blood, int cost){
		lst.push_back(card(picsPtr,cardPosiX[cnt++],13, plantname,ID,blood,cost));
	}
	void startup(){
		for (int i = 0; i < lst.size(); i++){
			lst[i].startup();
		}
	}
	void draw(){
		for (int i = 0; i < lst.size(); i++){
			putimagePng(lst[i].x, lst[i].y, &lst[i].pics->cardimg);
		}
	}
};

class List_bullet;
const float cardSlot::cardPosiX[9] = { 120, 190, 260, 330, 400, 470, 540, 610, 680 };//���ʵ�y�᷽����13

class block{
public:
	int status = 0;				//��ʶ�����ڵ�״̬��0����ֲ�1����ֲ�2����ֲ�
	float left, right, up, down; //�궨block��λ��
	int eventCnt = 0;			//���봥���¼��Ĳ���
	int eventStep = 200;		//�����¼�������
	int bloodNow = 10;			//��ǰѪ��
	int img_cnt = 0;				//��ǰ��Ⱦ����ͼƬ
	int FPS = 10;
	int FPS_CNT = 0;
	card *NowPlant;
	block(){
		;
	}
	float getCenterX(){
		return (left + right) / 2;
	}
	float getCenterY(){
		return (up + down) / 2; 
	}
	void setPlant(card *cardPtr){
		NowPlant = cardPtr;
	}

	block(float l, float r, float u, float d){
		status = 0;
		left = l;
		right = r;
		up = u;
		down = d;
	}
	bool isInArea(float x, float y){
		return x > left &&  x <= right && y > up && y <= down;
	}
	void draw(){
		if (status == 2){
			putimagePng((left + right) / 2 - NowPlant->pics->img[img_cnt].getwidth() / 2, (up + down) / 2 - NowPlant->pics->img[img_cnt].getheight() / 2, &NowPlant->pics->img[img_cnt]);
			if (FPS_CNT == FPS){
				img_cnt++;
				img_cnt %= NowPlant->pics->img_cnt_MAX;
				FPS_CNT = 0;
			}
			else{
				FPS_CNT++;
			}
		}
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
			//	data[i][j].setImage("\images\\plant_trans.png");
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
	void clearAllTrans(){
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 10; j++){
				if (data[i][j].status == 1){
					data[i][j].status = 0;
				}
			}
		}
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
	std::list<monster> list;
	std::string img_dir;
	int maxBlood = 10;
	chessBoard *ptrChessboard;

	int biteStep = 10;
	int biteCount = 0;
	List_monster(char imgdir[], int time = 0){
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
	bool inDistance(float x1, float y1, float x2, float y2){
		int R = 50;
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < R * R;
	}
	void draw(){
		tmpTimer++;
		if (tmpTimer == fps){
			timer++;
			tmpTimer = 0;
		}
		for (auto &mons : list){
			putimagePng(mons.x, mons.y, &img[(int)mons.cnt]);
			mons.cnt += mons.step;
			if (mons.cnt >= animationSize){
				mons.cnt = 0;
			}
			bool status = 0;
			for (auto & j : ptrChessboard->data){
				for (auto &plant : j){
					if (plant.status > 1 && inDistance(mons.x + mons.width ,mons.y + mons.height * 0.5, plant.right,(plant.up + plant.down) / 2)){
						status = 1;
						biteCount++;
						if (biteCount == biteStep){
							plant.bloodNow--;
							biteCount = 0;
						}
					}
				}
			}
			if (!status){
				mons.x--;
			}
		}
		for (auto &mons : list){
			if (mons.x < -100){
				list.remove(mons);
				break;
			}
			if (mons.blood <= 0){
				list.remove(mons);
				break;
			}
		}
	}
};
const float List_monster::rowPixY[5] = { 90, 220, 350, 480, 630 }; //ȷ������Ӧ����Ⱦ��y������



class sun{
public:
	IMAGE *img;
	float x, y, destx, desty,speed = 2,accspeed = -2;
	float width = 80, height = 86;
	float finalx = 50, finaly = 50;
	Mouse *m;
	int status = 0;
	
	float acc; //һ�����ٶ�ģ������
	sun(IMAGE *image, Mouse *mouse, float acclerate, float inx, float iny){
		img = image;
		m = mouse;
		x = inx - img->getwidth() / 2;
		y = iny - img->getheight() / 2;
		destx = x - rand() % 100 + 50;
		desty = y + rand() % 20;
		acc = acclerate;
		status = -1;
		accspeed = -2;

	}
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
		case -1:
			if (m->isInArea(x, y, y + height, x + width) && m->LEFTDOWN){
				status = 1;
			}
			if (y <= desty){
				accspeed += acc;
				y += accspeed;
				x -= (x - destx) * 0.5;
			}
			putimagePng(x, y , img);
			break;

		case 0: //̫����������
			if (m->isInArea(x, y, y + height, x + width) && m->LEFTDOWN){
				status = 1;
			}
			if (y <= desty){
				y += speed;
			}
			putimagePng(x, y, img);
			break;
		case 1: //̫�������������Ͻ�
			if (x <= finalx && y <= finaly){
				status = 2; // ̫��������ɣ���Sunlist��remove��
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
	int step = 120;
	int pace = 1200;
	std::list<sun> lst;
	void startup(){
		loadimage(&img, "\images\\sun.png");
	}
	void sunGenerator(){
		time_cnt++;
		if (time_cnt >= step){
			lst.push_back(sun(&img,m));
			time_cnt = 0;
			step = pace * 0.8 + rand() % (int)(pace*0.4);
		}
		time_cnt++;
	}
	void addSunflowerSun(float x, float y){
		lst.push_back(sun(&img, m, 0.1, x, y));
	}
	void draw(){
		sprintf(str_count, "%d", count);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		outtextxy(55, 83, str_count);  //����㵱ǰӵ�е�����
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
	float width = 58, height = 58;
	std::list<bullet> mylst;
	IMAGE img;
	void addbullet(float x, float y){
		mylst.push_back(bullet(x - img.getwidth() / 2, y - img.getheight() / 2, &img));
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

class statusCounter{
public:
	int code = 0;
	cardSlot * slot;
	Mouse m;
	IMAGE *pointImg;
	chessBoard *board;
	List_sun *listSun;
	List_bullet* listBullet;
	int selectCardNum = 0;
	card *selectCardPtr;
	statusCounter(cardSlot *cardslot, chessBoard *chessb, List_sun *listofsun, List_bullet *listBull){
		slot = cardslot;
		board = chessb;
		listSun = listofsun;
		listBullet = listBull;
		//��ʼ����������
	}
	void startup(){
		listSun->m = &m;
	}
	
	void trackStatus(){
		//shootEvent();
		//ֲ�ﱻ�Ե����߼��ж�
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 9; j++){
				if (board->data[i][j].status == 2 && board->data[i][j].bloodNow <= 0){
					board->data[i][j].status = 0;
					board->data[i][j].bloodNow = 10;
				}
				if (board->data[i][j].status == 2 && board->data[i][j].bloodNow > 0){
					board->data[i][j].eventCnt++;
					if (board->data[i][j].eventCnt == board->data[i][j].eventStep){
						board->data[i][j].NowPlant->action(this, board->data[i][j].getCenterX(), board->data[i][j].getCenterY());
						board->data[i][j].eventCnt = 0;
					}
				}
			}
		}
		switch (code)
		{
			
		//ѡ��Ƭ֮ǰ
		case 0:
			for (int j = 0; j < slot->cnt; j++){
				card * i = &slot->lst[j];
				if (m.isInArea(i->x, i->y, i->y + i->height, i->x + i->width) && m.LEFTDOWN && listSun->count >= slot->lst[j].cost){
					code = 1;
					selectCardPtr = i;
				}
			}

			break;
		//��ѡ��Ƭʱ��
		case 1:
			if (board->inBoard(m.x, m.y)){
				for (int i = 0; i < 5; i++){
					for (int j = 0; j < 10; j++){
						if (board->data[i][j].isInArea(m.x, m.y)){
							if (board->data[i][j].status == 0){
								selectCardPtr->pics->drawTransparent(board->data[i][j].getCenterX(), board->data[i][j].getCenterY());
								if (m.LEFTDOWN ){
									board->data[i][j].NowPlant = selectCardPtr;
									board->data[i][j].status = 2;
									code = 0;
									listSun->count -= selectCardPtr->cost;
								}
							}
						}
						else if (board->data[i][j].status < 2){
							board->data[i][j].status = 0;
						}
					}
				}
			}
			else{
				selectCardPtr->pics->drawTransparent(m.x, m.y);
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
					if (m.y < b.y && m.y + m.height > b.y && b.x + b.width > m.x + 20){ //m.y < b.y && m.y + m.height > b.y + b.height &&
						b.status = 1;
						m.blood--;
					}
				}
			}
		}
	}
};
namespace plantFunctions{
	void shootEvent(statusCounter *obj, int x, int y){ //�㶹���ֵĹ���
		obj->listBullet->addbullet(x, y);
	}
	void popSunEvent(statusCounter *obj, int x, int y){ //̫�����Ĺ���
		obj->listSun->addSunflowerSun(x, y);
	}

}

namespace game{
	IMAGE background;
	List_monster chaoxing("\images\\superstar\\superstar",rand() % 10);
	List_monster dingding("\images\\dingding\\dingding",rand() % 10);
	List_sun listSun;
	cardSlot slotCard;
	chessBoard board;
	List_bullet listBullet;
	statusCounter status(&slotCard, &board, &listSun, &listBullet);
	hitEvent hitEvt;
	//ʵ�ּ���ֲ�������ͼƬ·��
	picturesOFcard peePics("\images\\card.png", "\images\\plant\\plant", 1);
	picturesOFcard sunflowerPics("\images\\card.png", "\images\\sunflower\\sunflower", 8);
	void startup(){
		//���ر���ͼƬ
		loadimage(&background, _T("\images\\background.png"));
		//���ع����ز�
		chaoxing.startup();
		dingding.startup();
		listSun.startup();
		status.startup();
		//���ؿ�������
		
		//�ڴ˴��������ĵ�ͼƬ·������׼��ͼƬ�ز�
		slotCard.addCard(&peePics,"pea",1,10,100);
		slotCard.addCard(&sunflowerPics, "sunflower",2,10,50);
		slotCard.lst[0].action = plantFunctions::shootEvent;
		slotCard.lst[1].action = plantFunctions::popSunEvent;

		slotCard.startup();

		//��ʼ����������
		board.startup();
		listBullet.startup();

		//��ʼ��hitEvent
		hitEvt.addMonsterList(&chaoxing);
		hitEvt.addMonsterList(&dingding);
		hitEvt.bull = &listBullet;
		chaoxing.ptrChessboard = &board;
		dingding.ptrChessboard = &board;
	}

	void draw(int *page){
		status.m.update();
		putimagePng(0, 0, &background);
		listSun.sunGenerator();
		slotCard.draw();
		status.trackStatus();


		board.draw();
		listSun.draw(); //������ֲ�����Ⱦ
		//listBullet.testBullet();
		chaoxing.randomMonsterGenerator();
		dingding.randomMonsterGenerator(); //������ֲ�����Ⱦ
		hitEvt.draw();
		listBullet.draw();
	}
}