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
class obj{
public:
	int animationSize;
	IMAGE img[8];
	std::string dir;
	float cnt;
	float step = 0.7;
	float x=1000, y=100;
	float width, height;
	obj(int size, char path[] ){
		cnt = 0;
		dir = std::string(path);
		animationSize = size;
	}
	void startup(){
		
	}
	void draw(){
		putimagePng(x, y, &img[(int)cnt]);
		cnt += step;
		if (cnt >= animationSize){
			cnt = 0;
		}
		x--;
	}
};

//游戏主界面命名空间
namespace game{
	IMAGE background;
	obj chaoxing(8,"\images\\superstar\\superstar");
	void startup(){
		loadimage(&background, _T("\images\\background.png"));
		chaoxing.startup();

		for (int i = 0; i < chaoxing.animationSize; i++){
			std::string path = chaoxing.dir;
			path.append("0");
			path[path.size() - 1] += i;
			path.append(".png");
			loadimage(&chaoxing.img[i], _T(path.c_str()));
		}
	}

	void draw(int *page){
		putimagePng(0, 0, &background);
		chaoxing.draw();
	}
}