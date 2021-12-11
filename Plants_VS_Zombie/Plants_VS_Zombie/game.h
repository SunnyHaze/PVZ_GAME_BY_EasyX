#include "stdafx.h"
#pragma once
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
#include "stageReader.h"
namespace GameGlobal{
	const int GlobalFPS = 60;
	int GlobalGameSecond = 0;
	int GlobalCnt = 0;
	int GlobalQuitSecond = 0;
	int GlobalLastMonsterSecond = 0;
	int GlobalStatus = 0; // 0 �������·��� 1�����ʤ -1����G��
	void GlobalSetAllToZero(){
		GlobalGameSecond = 0;
		GlobalCnt = 0;
		GlobalQuitSecond = 0;
		GlobalLastMonsterSecond = 0;
		GlobalStatus = 0;
	}
}
using namespace GameGlobal;
//�����࣬��һ�ֹ���Ļ���λ����Ϣ���ٶ���Ϣ��¼
class monster{
public:
	float cnt = 0;  
	int blood = 6; //����Ѫ��
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
	bool operator==(monster obj){ //���ڴ���list,remove�����ж�ɾ��
		return x <= -100 || blood <= 0;
	}
};

//��Ƭ�����ͼƬ�ز���
class picturesOFcard{ //��ΪEasyX��IMAGE��Ķ����ڿ�����ʱ��ᷢ���жϣ�������Ҫ��������
public:
	IMAGE cardimg;		//������ͼƬ��ַ
	IMAGE dimCardImg;	//�谵���۵�ͼƬ��ַ
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
		std::string tmp(imgdir);
		loadimage(&cardimg, tmp.append(".png").c_str()); // ���뿨Ƭ�ز�
		tmp = std::string(imgdir);
		loadimage(&dimCardImg, tmp.append("_dim.png").c_str()); //����谵�Ŀ�Ƭ�ز�
		for (int i = 0; i < img_cnt_MAX; i++){
			std::string tmpPath(tardir);
			tmpPath.append("0");
			tmpPath[tmpPath.length() - 1] += i + 1;
			tmpPath.append(".png");
			loadimage(&img[i], _T(tmpPath.c_str()));
		}
		tmp = std::string(tardir);
		loadimage(&trans_img, tmp.append("_trans.png").c_str()); //�����͸��ͼƬ
	}

	//ֱ�ӻ��ư�͸��ֲ����
	void drawTransparent(int x, int y){
		putimagePng(x - trans_img.getwidth() / 2, y - trans_img.getheight() / 2, &trans_img);
	}
};

class statusCounter; //��ǰ����״̬�࣬card�Ļص��������õ�
//��Ƭλ��
class card{
public:
	int status = 0; //�����ж���Ƭ�Ƿ񡱿��á�������ǰ�������Ƿ���ڿ�Ƭcost 0Ϊ�谵��Ƭ 1Ϊ���� -1����ǰ��Ƭ����
	float width = 63, height = 84; 
	float x, y;			//��Ƭλ��
	std::string name;   //��Ƭ��
	int cardID;         //��ƬID
	picturesOFcard *pics;//��Ƭ������ͼƬ�زĿ� ָ��
	int blood;			//Ѫ��
	int cost;			//����������
	int eventPeriod;	//�����¼���Ĭ��������
	int cardCD = 7;		//��Ƭ����ȴʱ��
	int expireUsedSecond = 0;
	void(*action)(statusCounter *obj,int,int);  //******�ǳ���Ҫ******���ڿ���ĳ��ֲ�������Ϊ�ĺ���ָ�룬��Ҫ��ָ�����������
	card(picturesOFcard *picsPtr, float inx, float iny, char cardname[], int id, int inputBlood, int inputCost, float Period){
		pics = picsPtr;
		x = inx;
		y = iny;
		name = std::string(cardname);
		cardID = id;
		blood = inputBlood;
		cost = inputCost;
		eventPeriod = Period * GlobalFPS;
	}
	bool checkCD(){
		
	}
	void startup(){
		pics->startup();
	}
};

//�����࣬���ڹ���ǰ�����д��ڵĿ�Ƭ
class cardSlot{
public:
	const static float cardPosiX[9];
	int cnt = 0;
	std::vector<card> lst;
	void addCard(picturesOFcard *picsPtr, char plantname[], int ID, int blood, int cost, int eventPeriod){
		lst.push_back(card(picsPtr, cardPosiX[cnt++], 13, plantname, ID, blood, cost, eventPeriod));
	}
	void startup(){
		for (int i = 0; i < lst.size(); i++){
			lst[i].startup();
		}
	}
	void statusCheck(int sunCnt){
		//��������м���
		for (auto &i : lst){
			if (i.cost <= sunCnt && GlobalGameSecond >= i.expireUsedSecond){
				i.status = 1;
			}
			else{
				i.status = 0;
			}
			
		}
	}

	void draw(){
		for (int i = 0; i < lst.size(); i++){
			if (lst[i].status == 0)
				putimagePng(lst[i].x, lst[i].y, &lst[i].pics->dimCardImg);
			else
				putimagePng(lst[i].x, lst[i].y, &lst[i].pics->cardimg);
		}
	}

	void erase(std::string name){
		std::vector<card>::iterator it = lst.begin();
		while (it != lst.end()){
			if (it->name == name){
				it = lst.erase(it);
				cnt--;
			}
			else{
				it++;
			}
		}
	}
	void eraseException(std::vector<std::string> SelectPlants){
		std::vector<std::string> deletetarget;
		for (auto c : lst){
			bool flag = false;
			for (auto sname : SelectPlants){
				if (c.name == sname){
					flag = true;
				}
			}
			if (flag == false){
				deletetarget.push_back(c.name);
			}
		}
		for (auto i : deletetarget){
			this->erase(i);
		}
	}
};

class List_bullet;
const float cardSlot::cardPosiX[9] = { 120, 190, 260, 330, 400, 470, 540, 610, 680 };//���ʵ�y�᷽����13

//��chessbord�����Ҫ��Ա�����ÿһ��ֲ�����Ϣ
class block{
public:
	int status = 0;				//��ʶ�����ڵ�״̬��0����ֲ�2����ֲ�1Ŀǰû�������ˣ�֮ǰ����ֲ�������״̬��ά����statuscounter��)��
	float left, right, up, down; //�궨block��λ��
	int eventCnt = 0;			//���봥���¼��Ĳ���
	int eventStep = 200;		//�����¼�������
	int bloodNow = 10;			//��ǰѪ��
	int img_cnt = 0;				//��ǰ��Ⱦ����ͼƬ
	int FPS = 10;				// ���ٴ���Ⱦ�Ž�����һ֡
	int FPS_CNT = 0;			// ��ǰ֡������
	card *NowPlant;				//ָ��ǰ�����ڹ���Ŀ�Ƭ��ָ��
	block(){
		;
	}
	float getCenterX(){
		return (left + right) / 2;
	}
	float getCenterY(){
		return (up + down) / 2; 
	}
	//���õ�ǰֲ��Ϊ
	void setPlant(card *cardPtr){ 
		if (cardPtr != NULL){
			NowPlant = cardPtr;
			bloodNow = cardPtr->blood;
			eventStep = cardPtr->eventPeriod;
			eventCnt = 0;
		}
		else{
			status = 0;
		}
	}

	block(float l, float r, float u, float d){
		status = 0;
		left = l;
		right = r;
		up = u;
		down = d;
	}
	//��Ҫ�����ж������Ƿ���block����ķ�Χ��
	bool isInArea(float x, float y){
		return x > left &&  x <= right && y > up && y <= down;
	}
	//��ͼ����
	void draw(){
		//���ж�Ϊ��ֲ��ʱ���������԰���֡�����޻�ͼ
		if (status == 2){
			putimagePng((left + right) / 2 - NowPlant->pics->img[img_cnt].getwidth() / 2, (up + down) / 2 - NowPlant->pics->img[img_cnt].getheight() / 2, &NowPlant->pics->img[img_cnt]);
			if (FPS_CNT == FPS){ // ÿFPS�λ��ƣ��ͼ�һ֡
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
// �����ڴ������ֲ����Ϣ����
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
	//�ж��Ƿ���ֲ��������
	bool inBoard(float x, float y){
		return x < 970 && x > 40 && y < 726 && y > 113;
	}
	//������е�ֲ��
	void clearAll(){
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 10; j++){
				if (data[i][j].status >= 1){
					data[i][j].status = 0;
				}
			}
		}
	}
};
//�����б����ڹ���һ�ֹ�������ж���
class List_monster{
public:
	const static float rowPixY[5];
	int fps = 60;		//FPS
	long long timer = 0; //ʱ�������
	int tmpTimer = 0;
	IMAGE img[8];
	int animationSize = 8; //���ﶯ����֡��
	std::list<monster> list; //�����б�
	std::string img_dir;	//����ͼ���path
	int maxBlood = 10;		//����Ѫ��
	chessBoard *ptrChessboard; //ָ�����̵�ָ�룬���ڿ���ֲ��ġ����ԡ��߼�
	int biteStep = 10; //���ƶ�ó�һ��
	int biteCount = 0;

	bool BrainEating = false; //��Ҫ������ʬ�Ե���������ӣ�����
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
	// �����ж��Ƿ��ĳ���塰����������Ҫ�������ӵ���ײ�߼��Ϳ�ʳ�߼�
	bool inDistance(float x1, float y1, float x2, float y2){
		int R = 50;
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < R * R;
	}
	void draw(){
		if (BrainEating){
			if (GlobalStatus == -1){
				settextcolor(GREEN);
				settextstyle(40, 0, "����");
				outtextxy(400, 300, "��ʬ�Ե���������ӣ�����");
			}
			if (GlobalQuitSecond == 0){
				GlobalQuitSecond = GlobalGameSecond + 5;
			}
		}
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
			// ��ʳ�߼���ʵ��
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
				BrainEating = true;
				GlobalStatus = -1;
				list.remove(mons);
				break;
			}
			if (mons.blood <= 0){
				list.remove(mons);
				break;
			}
		}
	}
	// ��չ����б�
	void clear(){
		BrainEating = false;
		list.clear();
	}
};
const float List_monster::rowPixY[5] = { 90, 220, 350, 480, 630 }; //ȷ������Ӧ����Ⱦ��y������

// ������ ÿһ���������Ϣ
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

//�����б����ڹ������е�����
class List_sun{
public:
	Mouse *m;
	IMAGE img;
	int count = 10000; //��ʼ��ʱ����������
	char str_count[8];
	int LastGenerate = 0; //������һ���������ɵ�ʱ��
	int step = 6;		//����������һ������
	std::list<sun> lst;
	void startup(){
		loadimage(&img, "\images\\sun.png");
	}
	//����е��������������
	void sunGenerator(){
		if (GlobalGameSecond - LastGenerate >= step){
			lst.push_back(sun(&img,m));
			LastGenerate = GlobalGameSecond;
			step = 7 - rand() % 2;
		}
	}
	//���һ��̫���������������������
	void addSunflowerSun(float x, float y){
		lst.push_back(sun(&img, m, 0.1, x, y));
	}
	//��������б�
	void clear(){
		lst.clear();
		LastGenerate = 0;
	}
	void draw(){
		sprintf(str_count, "%d", count);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(20, 0, "����");
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
//�ӵ��б����ڹ������е��ӵ�
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
	//�����ӵ��б����ܵ��࣬Ŀǰ��δʹ��
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

//����¼���������ӵ�����й���������ָ��
class statusCounter{
public:
	IMAGE shovelPoint;
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
		loadimage(&shovelPoint, "\images\\shovel.png");
	}
	//����һ������¼��Ĵ�������
	void trackStatus(){
		//�Կ��۽��������ж�
		slot->statusCheck(listSun->count);
		//ֲ�ﱻ�Ե����߼��ж�
		for (int i = 0; i < 5; i++){
			for (int j = 0; j < 9; j++){
				if (board->data[i][j].status == 2 && board->data[i][j].bloodNow <= 0){
					board->data[i][j].status = 0;
					board->data[i][j].setPlant(NULL);
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
		//�ж���ǰȫ�ֵ�����¼�״̬
		switch (code)
		{
		//���ڲ���ֲ���״̬
		case -1:
			if (m.RIGHTDOWN){
				code = 0;
			}
			putimagePng(m.x - shovelPoint.getwidth() / 2, m.y - shovelPoint.getheight()/ 2, &shovelPoint);
			for (int i = 0; i < 5; i++){
				for (int j = 0; j < 10; j++){
					if (board->data[i][j].isInArea(m.x, m.y) && board->data[i][j].status == 2 && m.LEFTDOWN){
						board->data[i][j].setPlant(NULL);
						code = 0;
					}
				}
			}
			break;
			
		//ѡ��Ƭ֮ǰ�����·�������״̬
		case 0:
			for (int j = 0; j < slot->cnt; j++){
				card * i = &slot->lst[j];
				if (m.isInArea(i->x, i->y, i->y + i->height, i->x + i->width) && m.LEFTDOWN && listSun->count >= slot->lst[j].cost){
					code = 1;
					selectCardPtr = i;
				}
				if (m.isInArea(790, 14, 82, 852) && m.LEFTDOWN){
					code = -1;
				}
			}

			break;
		//�Ѿ�ѡ��һ�ſ�Ƭ��Ŀǰ��ֲ�����ǿ�����ʾֲ����Ӱ��ʱ��
		case 1:
			if (board->inBoard(m.x, m.y)){
				for (int i = 0; i < 5; i++){
					for (int j = 0; j < 10; j++){
						if (board->data[i][j].isInArea(m.x, m.y)){
							if (board->data[i][j].status == 0){
								selectCardPtr->pics->drawTransparent(board->data[i][j].getCenterX(), board->data[i][j].getCenterY());
								if (m.LEFTDOWN ){ //��������������ȥ��ֲ��
									board->data[i][j].setPlant(selectCardPtr); //����������ֲ����ܵ���status
									selectCardPtr->expireUsedSecond = GlobalGameSecond + selectCardPtr->cardCD;
									selectCardPtr->status = 0;
									board->data[i][j].status = 2;
									code = 0;
									listSun->count -= selectCardPtr->cost;
								}
							}
						}
						else if (board->data[i][j].status < 2){ //�����Ӱ
							board->data[i][j].status = 0;
						}
					}
				}
			}
			else{
				selectCardPtr->pics->drawTransparent(m.x, m.y); //��������ֲ����Ӱ
			}
			// ����һ�����ȡ����Ӵ������״̬���ص������·�����
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

//���ڹ������й������
class monsterManger{
public:
	std::vector<List_monster*> mons;
	std::vector<int> generateTime;
	int idx = 0;
	bool flag = false;
	void addMonsterList(List_monster *obj){
		mons.push_back(obj);
	}
	void randomMonsterGenerator(){
		if (generateTime.size() != idx){
			if (GlobalCnt == 0 && GlobalGameSecond == generateTime[idx]){
				int whichKind = rand() % mons.size();
				mons[whichKind]->addMonster(rand() % 5);
				idx++;
			}
		}
		flag = false;
		for (auto i : mons){
			if (i->list.size() != 0){
				flag = true;
			}
		}
		//ʤ���ж�
		if (GlobalQuitSecond == 0 && !flag && GlobalGameSecond >= GlobalLastMonsterSecond && GlobalStatus==0){
			GlobalStatus = 1;
			GlobalQuitSecond = GlobalGameSecond + 5;
		}
		draw();
	}
	void draw(){
		for (auto i : mons){
			i->draw();
		}
		//ʤ���ж�
		if (GlobalGameSecond > GlobalLastMonsterSecond && GlobalGameSecond < GlobalQuitSecond && GlobalStatus == 1){
			setcolor(RED);
			settextstyle(40, 0, "����");
			outtextxy(400, 300, "����ʤ��������");
		}
	}
	void clear(){
		for (auto &i : mons){
			i->clear();
		}
	}
};
//���ڹ���ֲ��������ӵ��������ײ���Ѫ����
class hitEvent{
public:
	monsterManger *MonsterManagerPtr;
	std::vector<List_monster*> *mons;
	List_bullet *bull;
	void startup(){
		mons = &MonsterManagerPtr->mons;

	}
	void setBulletList(List_bullet *obj){
		bull = obj;
	}

	void draw(){
		for (auto &i : *mons){
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
//��������ռ������е�ֲ�����Ϊ���࣬Ŀǰֻ������ֲ�����ֻ��������������ĺ���
namespace plantFunctions{
	void shootEvent(statusCounter *obj, int x, int y){ //�㶹���ֵĹ���
		obj->listBullet->addbullet(x, y);
	}
	void popSunEvent(statusCounter *obj, int x, int y){ //̫�����Ĺ���
		obj->listSun->addSunflowerSun(x, y);
	}
}

//ʵ������game�����ռ䣬�ڲ�Ϊ�������ʵ������ʵ��������߼�����
namespace game{
	IMAGE background;
	List_monster chaoxing("\images\\superstar\\superstar",rand() % 10);
	List_monster dingding("\images\\dingding\\dingding",rand() % 10);
	monsterManger MonsManage;
	List_sun listSun;
	cardSlot slotCard;
	chessBoard board;
	List_bullet listBullet;
	statusCounter status(&slotCard, &board, &listSun, &listBullet);
	hitEvent hitEvt;
	//ʵ�ּ���ֲ�������ͼƬ·��
	picturesOFcard peePics("\images\\cards\\card_pea", "\images\\plant\\plant", 1);
	picturesOFcard sunflowerPics("\images\\cards\\card_sunflower", "\images\\sunflower\\sunflower", 8);
	//���ؿ�Ƭ��Ϣ
	void loadCards(){
		slotCard.cnt = 0;
		slotCard.addCard(&peePics, "pea", 1, 10, 100, 2);
		slotCard.addCard(&sunflowerPics, "sunflower", 2, 10, 50, 7);
		slotCard.lst[0].action = plantFunctions::shootEvent;
		slotCard.lst[1].action = plantFunctions::popSunEvent;
	}
	//���ر��ؿ�����������Ϣ
	void loadStage(int stageNum){
		//���ҳ��ȫ������
		GlobalSetAllToZero();
		readStage::stageInfo stageData(readStage::ParseFromFile(stageNum));
		listSun.count = stageData.startSunlight;
		MonsManage.generateTime = stageData.monsterTime;
		MonsManage.idx = 0; // ��0��ʼ���¼ƹ�
		GlobalLastMonsterSecond = stageData.TIME;
		board.clearAll();
		listSun.clear();
		MonsManage.clear();
		listBullet.mylst.clear();
		slotCard.lst.clear();
		loadCards(); //���¼������п�Ƭ
		slotCard.eraseException(stageData.plantsCanUse);
		slotCard.startup();
	}

	void startup(){
		//���ر���ͼƬ
		loadimage(&background, _T("\images\\background.png"));
		//���ع����ز�
		chaoxing.startup();
		dingding.startup();
		MonsManage.addMonsterList(&chaoxing);
		MonsManage.addMonsterList(&dingding);
		//���������ز�
		listSun.startup();
		//����ȫ������¼�������
		status.startup();
		
		//���ؿ�������
		//�ڴ˴��������ĵ�ͼƬ·������׼��ͼƬ�ز�
		slotCard.startup();

		//��ʼ����������
		board.startup();
		//��ʼ���ӵ��б�
		listBullet.startup();
		//��ʼ��hitEvent
		hitEvt.MonsterManagerPtr = &MonsManage;
		hitEvt.setBulletList(&listBullet);
		hitEvt.startup();

		chaoxing.ptrChessboard = &board;
		dingding.ptrChessboard = &board;
	}

	void draw(int *page){
		//���ڼ��㵱ǰ����������Ϸƽ���Ե���
		GlobalCnt++;
		if (GlobalCnt == GlobalFPS){
			GlobalCnt = 0;
			GlobalGameSecond++;
			printf("%d", GlobalGameSecond);
		}
		status.m.update(); //�������״̬
		putimagePng(0, 0, &background);	//�������
		listSun.sunGenerator();	//��������ʱ��
		slotCard.draw();	//���ƿ���

		board.draw();		//����ֲ����
		listSun.draw(); //������ֲ�����Ⱦ
		//listBullet.testBullet();
		MonsManage.randomMonsterGenerator();	//���ɹ�����
		hitEvt.draw();	//�ж�ײ���¼��ĵط�
		listBullet.draw();
		status.trackStatus();
		if (GlobalQuitSecond != 0 && GlobalQuitSecond == GlobalGameSecond){
			*page = 1; //��Ϸ���������ص���һҳ 
		}
	}
}