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
//插入数据结构
#include <vector>
#include <list>
#include <map>
#include <functional>
//用于插入自定义的鼠标类，封装了一些鼠标操作
#include "pageItem.h"
#include "stageReader.h"
namespace GameGlobal{
	const int GlobalFPS = 60;
	int GlobalGameSecond = 0;
	int GlobalCnt = 0;
	int GlobalQuitSecond = 0;
	int GlobalLastMonsterSecond = 0;
	int GlobalStatus = 0; // 0 代表无事发生 1代表获胜 -1代表G了
	void GlobalSetAllToZero(){
		GlobalGameSecond = 0;
		GlobalCnt = 0;
		GlobalQuitSecond = 0;
		GlobalLastMonsterSecond = 0;
		GlobalStatus = 0;
	}
}
using namespace GameGlobal;
//怪物类，是一种怪物的基本位置信息和速度信息记录
class monster{
public:
	float cnt = 0;  
	int blood = 6; //怪物血量
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
	bool operator==(monster obj){ //用于传给list,remove函数判定删除
		return x <= -100 || blood <= 0;
	}
};

//卡片所需的图片素材类
class picturesOFcard{ //因为EasyX的IMAGE类的对象在拷贝的时候会发生中断，所以需要单独处理
public:
	IMAGE cardimg;		//卡槽中图片地址
	IMAGE dimCardImg;	//昏暗卡槽的图片地址
	IMAGE img[8];		//实际渲染图片地址
	IMAGE trans_img;	//半透明图片地址
	std::string imgdir; //卡槽中的卡片的路径
	std::string tardir; //植物实体的路径
	int img_cnt_MAX;
	picturesOFcard(std::string inputImgdir, std::string inputTargetdir, int img_cnt_M){
		imgdir = inputImgdir;
		tardir = inputTargetdir;
		img_cnt_MAX = img_cnt_M;
	}

	void startup(){
		std::string tmp(imgdir);
		loadimage(&cardimg, tmp.append(".png").c_str()); // 读入卡片素材
		tmp = std::string(imgdir);
		loadimage(&dimCardImg, tmp.append("_dim.png").c_str()); //读入昏暗的卡片素材
		for (int i = 0; i < img_cnt_MAX; i++){
			std::string tmpPath(tardir);
			tmpPath.append("0");
			tmpPath[tmpPath.length() - 1] += i + 1;
			tmpPath.append(".png");
			loadimage(&img[i], _T(tmpPath.c_str()));
		}
		tmp = std::string(tardir);
		loadimage(&trans_img, tmp.append("_trans.png").c_str()); //读入半透明图片
	}

	//直接绘制半透明植物用
	void drawTransparent(int x, int y){
		putimagePng(x - trans_img.getwidth() / 2, y - trans_img.getheight() / 2, &trans_img);
	}
};

class statusCounter; //提前声明状态类，card的回调函数会用到
//卡片位置
class card{
public:
	int status = 0; //用于判定卡片是否”可用“，即当前阳光数是否高于卡片cost 0为昏暗卡片 1为可用 -1代表当前卡片禁用
	float width = 63, height = 84; 
	float x, y;			//卡片位置
	std::string name;   //卡片名
	int cardID;         //卡片ID
	picturesOFcard *pics;//卡片的三种图片素材库 指针
	int blood;			//血量
	int cost;			//消耗阳光数
	int eventPeriod;	//触发事件的默认周期数
	int cardCD = 7;		//卡片的冷却时间
	int expireUsedSecond = 0;
	void(*action)(statusCounter *obj,int,int);  //******非常重要******用于控制某种植物具体行为的函数指针，需要传指针才能运作。
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

//卡槽类，用于管理当前卡槽中存在的卡片
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
		//对阳光进行检索
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
const float cardSlot::cardPosiX[9] = { 120, 190, 260, 330, 400, 470, 540, 610, 680 };//合适的y轴方向是13

//是chessbord类的主要成员，存放每一个植物的信息
class block{
public:
	int status = 0;				//标识块现在的状态（0是无植物，2是有植物，1目前没有意义了（之前是虚植物，现在虚状态的维护在statuscounter中)）
	float left, right, up, down; //标定block的位置
	int eventCnt = 0;			//距离触发事件的步数
	int eventStep = 200;		//触发事件的周期
	int bloodNow = 10;			//当前血量
	int img_cnt = 0;				//当前渲染到的图片
	int FPS = 10;				// 多少次渲染才进行下一帧
	int FPS_CNT = 0;			// 当前帧的索引
	card *NowPlant;				//指向当前格子内管理的卡片的指针
	block(){
		;
	}
	float getCenterX(){
		return (left + right) / 2;
	}
	float getCenterY(){
		return (up + down) / 2; 
	}
	//设置当前植物为
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
	//主要用于判定坐标是否处于block管理的范围内
	bool isInArea(float x, float y){
		return x > left &&  x <= right && y > up && y <= down;
	}
	//绘图函数
	void draw(){
		//当判定为有植物时，则周期性按照帧数上限绘图
		if (status == 2){
			putimagePng((left + right) / 2 - NowPlant->pics->img[img_cnt].getwidth() / 2, (up + down) / 2 - NowPlant->pics->img[img_cnt].getheight() / 2, &NowPlant->pics->img[img_cnt]);
			if (FPS_CNT == FPS){ // 每FPS次绘制，就加一帧
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
// 是用于存放所有植物信息的类
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
	//判定是否在植物区域内
	bool inBoard(float x, float y){
		return x < 970 && x > 40 && y < 726 && y > 113;
	}
	//清空所有的植物
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
//怪物列表，用于管理一种怪物的所有对象
class List_monster{
public:
	const static float rowPixY[5];
	int fps = 60;		//FPS
	long long timer = 0; //时间计数器
	int tmpTimer = 0;
	IMAGE img[8];
	int animationSize = 8; //怪物动画总帧数
	std::list<monster> list; //怪物列表
	std::string img_dir;	//怪物图像的path
	int maxBlood = 10;		//怪物血量
	chessBoard *ptrChessboard; //指向棋盘的指针，用于控制植物的“被吃”逻辑
	int biteStep = 10; //控制多久吃一口
	int biteCount = 0;

	bool BrainEating = false; //重要，代表僵尸吃掉了你的脑子！！！
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
	// 用于判断是否和某物体“相碰”，主要是用于子弹碰撞逻辑和啃食逻辑
	bool inDistance(float x1, float y1, float x2, float y2){
		int R = 50;
		return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2) < R * R;
	}
	void draw(){
		if (BrainEating){
			if (GlobalStatus == -1){
				settextcolor(GREEN);
				settextstyle(40, 0, "隶书");
				outtextxy(400, 300, "僵尸吃掉了你的脑子！！！");
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
			// 啃食逻辑的实现
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
	// 清空怪物列表
	void clear(){
		BrainEating = false;
		list.clear();
	}
};
const float List_monster::rowPixY[5] = { 90, 220, 350, 480, 630 }; //确定怪物应该渲染的y轴坐标

// 阳光类 每一个阳光的信息
class sun{
public:
	IMAGE *img;
	float x, y, destx, desty,speed = 2,accspeed = -2;
	float width = 80, height = 86;
	float finalx = 50, finaly = 50;
	Mouse *m;
	int status = 0;
	
	float acc; //一个加速度模拟重力
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

		case 0: //太阳正常下落
			if (m->isInArea(x, y, y + height, x + width) && m->LEFTDOWN){
				status = 1;
			}
			if (y <= desty){
				y += speed;
			}
			putimagePng(x, y, img);
			break;
		case 1: //太阳正在走向左上角
			if (x <= finalx && y <= finaly){
				status = 2; // 太阳回收完成（在Sunlist中remove）
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

//阳光列表，用于管理所有的阳光
class List_sun{
public:
	Mouse *m;
	IMAGE img;
	int count = 10000; //初始化时的总阳光数
	char str_count[8];
	int LastGenerate = 0; //计算上一个阳光生成的时间
	int step = 6;		//多少秒生成一个阳光
	std::list<sun> lst;
	void startup(){
		loadimage(&img, "\images\\sun.png");
	}
	//天空中掉落阳光的生成器
	void sunGenerator(){
		if (GlobalGameSecond - LastGenerate >= step){
			lst.push_back(sun(&img,m));
			LastGenerate = GlobalGameSecond;
			step = 7 - rand() % 2;
		}
	}
	//添加一个太阳花产生的阳光的生成器
	void addSunflowerSun(float x, float y){
		lst.push_back(sun(&img, m, 0.1, x, y));
	}
	//清空阳光列表
	void clear(){
		lst.clear();
		LastGenerate = 0;
	}
	void draw(){
		sprintf(str_count, "%d", count);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		settextstyle(20, 0, "黑体");
		outtextxy(55, 83, str_count);  //输出你当前拥有的阳光
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
//子弹类，用于确定单颗子弹的性质：
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
//子弹列表，用于管理所有的子弹
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
	//测试子弹列表性能的类，目前并未使用
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

//鼠标事件控制器，拥有所有管理类对象的指针
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
		//初始化鼠标监听器
	}
	void startup(){
		listSun->m = &m;
		loadimage(&shovelPoint, "\images\\shovel.png");
	}
	//进行一次鼠标事件的触发监听
	void trackStatus(){
		//对卡槽进行阳光判定
		slot->statusCheck(listSun->count);
		//植物被吃掉的逻辑判定
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
		//判定当前全局的鼠标事件状态
		switch (code)
		{
		//用于铲除植物的状态
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
			
		//选择卡片之前“无事发生”的状态
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
		//已经选中一张卡片，目前在植物区是可以显示植物虚影的时候
		case 1:
			if (board->inBoard(m.x, m.y)){
				for (int i = 0; i < 5; i++){
					for (int j = 0; j < 10; j++){
						if (board->data[i][j].isInArea(m.x, m.y)){
							if (board->data[i][j].status == 0){
								selectCardPtr->pics->drawTransparent(board->data[i][j].getCenterX(), board->data[i][j].getCenterY());
								if (m.LEFTDOWN ){ //单击后代表放置下去此植物
									board->data[i][j].setPlant(selectCardPtr); //必须先设置植物才能调节status
									selectCardPtr->expireUsedSecond = GlobalGameSecond + selectCardPtr->cardCD;
									selectCardPtr->status = 0;
									board->data[i][j].status = 2;
									code = 0;
									listSun->count -= selectCardPtr->cost;
								}
							}
						}
						else if (board->data[i][j].status < 2){ //清除虚影
							board->data[i][j].status = 0;
						}
					}
				}
			}
			else{
				selectCardPtr->pics->drawTransparent(m.x, m.y); //鼠标跟随有植物虚影
			}
			// 如果右击，则取消所哟的特殊状态，回到“无事发生”
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
//游戏主界面命名空间

//用于管理所有怪物的类
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
		//胜利判定
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
		//胜利判定
		if (GlobalGameSecond > GlobalLastMonsterSecond && GlobalGameSecond < GlobalQuitSecond && GlobalStatus == 1){
			setcolor(RED);
			settextstyle(40, 0, "隶书");
			outtextxy(400, 300, "本关胜利！！！");
		}
	}
	void clear(){
		for (auto &i : mons){
			i->clear();
		}
	}
};
//用于管理植物射出的子弹与怪物碰撞后掉血的类
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
			for (auto &m : i->list){ //j是monsters对象
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
//这个命名空间是所有的植物的行为的类，目前只有两个植物，所以只有他们俩有这里的函数
namespace plantFunctions{
	void shootEvent(statusCounter *obj, int x, int y){ //豌豆射手的功能
		obj->listBullet->addbullet(x, y);
	}
	void popSunEvent(statusCounter *obj, int x, int y){ //太阳花的功能
		obj->listSun->addSunflowerSun(x, y);
	}
}

//实例化的game命名空间，内部为各种类的实例化以实现整体的逻辑交互
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
	//实现加载植物所需的图片路径
	picturesOFcard peePics("\images\\cards\\card_pea", "\images\\plant\\plant", 1);
	picturesOFcard sunflowerPics("\images\\cards\\card_sunflower", "\images\\sunflower\\sunflower", 8);
	//加载卡片信息
	void loadCards(){
		slotCard.cnt = 0;
		slotCard.addCard(&peePics, "pea", 1, 10, 100, 2);
		slotCard.addCard(&sunflowerPics, "sunflower", 2, 10, 50, 7);
		slotCard.lst[0].action = plantFunctions::shootEvent;
		slotCard.lst[1].action = plantFunctions::popSunEvent;
	}
	//加载本关卡所需所有信息
	void loadStage(int stageNum){
		//清空页面全局数据
		GlobalSetAllToZero();
		readStage::stageInfo stageData(readStage::ParseFromFile(stageNum));
		listSun.count = stageData.startSunlight;
		MonsManage.generateTime = stageData.monsterTime;
		MonsManage.idx = 0; // 从0开始重新计怪
		GlobalLastMonsterSecond = stageData.TIME;
		board.clearAll();
		listSun.clear();
		MonsManage.clear();
		listBullet.mylst.clear();
		slotCard.lst.clear();
		loadCards(); //重新加载所有卡片
		slotCard.eraseException(stageData.plantsCanUse);
		slotCard.startup();
	}

	void startup(){
		//加载背景图片
		loadimage(&background, _T("\images\\background.png"));
		//加载怪物素材
		chaoxing.startup();
		dingding.startup();
		MonsManage.addMonsterList(&chaoxing);
		MonsManage.addMonsterList(&dingding);
		//加载阳光素材
		listSun.startup();
		//加载全局鼠标事件监听器
		status.startup();
		
		//加载卡槽内容
		//在此处加载上文的图片路径用于准备图片素材
		slotCard.startup();

		//初始化棋盘网格
		board.startup();
		//初始化子弹列表
		listBullet.startup();
		//初始化hitEvent
		hitEvt.MonsterManagerPtr = &MonsManage;
		hitEvt.setBulletList(&listBullet);
		hitEvt.startup();

		chaoxing.ptrChessboard = &board;
		dingding.ptrChessboard = &board;
	}

	void draw(int *page){
		//用于计算当前秒数方便游戏平衡性调整
		GlobalCnt++;
		if (GlobalCnt == GlobalFPS){
			GlobalCnt = 0;
			GlobalGameSecond++;
			printf("%d", GlobalGameSecond);
		}
		status.m.update(); //更新鼠标状态
		putimagePng(0, 0, &background);	//输出北京
		listSun.sunGenerator();	//生成阳光时间
		slotCard.draw();	//绘制卡槽

		board.draw();		//绘制植物区
		listSun.draw(); //阳光在植物后渲染
		//listBullet.testBullet();
		MonsManage.randomMonsterGenerator();	//生成怪物区
		hitEvt.draw();	//判定撞击事件的地方
		listBullet.draw();
		status.trackStatus();
		if (GlobalQuitSecond != 0 && GlobalQuitSecond == GlobalGameSecond){
			*page = 1; //游戏结束则跳回到第一页 
		}
	}
}