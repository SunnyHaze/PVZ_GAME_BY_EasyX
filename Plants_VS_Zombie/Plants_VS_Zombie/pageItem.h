#include "stdafx.h"
#include "EasyXPng.h"
#include <iostream>
#include <graphics.h>
class Mouse{
public:
	ExMessage m;
	float x, y;
	bool LEFTDOWN = 0;
	void update(){
		while (peekmessage(&m, EM_MOUSE)){
			x = m.x;
			y = m.y;
			if (m.message == WM_LBUTTONDOWN){
				LEFTDOWN = 1;
			}
			else{
				LEFTDOWN = 0;
			}
		}
	}
	bool isInArea(float left, float up, float down, float right){
		if (right > x && left < x && down > y && up < y){
			return 1;
		}
		else{
			return 0;
		}
	}
};
class BUTTON {
public:
	IMAGE img[3];
	float x, y, width, height;
	int status = 0;
	std::string dir;
	void(*ButtonEvent)();

	BUTTON(float x, float y, char path[]){
		this->x = x;
		this->y = y;
		dir = std::string(path);
	}
	void startup(){
		for (int i = 0; i < 3; i++){
			std::string path = dir;
			path.append("1");
			path[path.size() - 1] += i;
			path.append(".png");
			loadimage(&img[i], _T(path.c_str()));
		}
		width = img[0].getwidth();
		height = img[0].getheight();
	}

	void draw(Mouse m){
		switch (status)
		{
		case 0:
			putimagePng(x, y, &img[status]);
			if (ifMouseOver(m)){
				status = 1;
			}
			break;
		case 1:
			putimagePng(x, y, &img[status]);
			if (!ifMouseOver(m)){
				status = 0;
			}
			if (m.LEFTDOWN){
				status = 2;
			}
			break;
		case 2:
			putimagePng(x, y, &img[status]);
			if (!ifMouseOver(m)){
				status = 0;
			}
			else{
				if (!m.LEFTDOWN){
					(*ButtonEvent)();
					status = 1;
				}
			}
			break;
		default:
			break;
		}
	}
	bool ifMouseOver(Mouse m){
		return m.isInArea(x, y, y + height, x + width);
	}
};