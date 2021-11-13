#pragma once
#include "stdafx.h"
#include "EasyXPng.h"
#include <iostream>
#include <graphics.h>
//此头文件涉及到对于页面内原件的封装

//用于页面监听鼠标事件的鼠标类
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

//用于绘制按钮并且触发按钮事件的按钮类
class BUTTON {
public:
	IMAGE img[3];
	float x, y, width, height;
	int status = 0;
	std::string dir;
	void(*ButtonEvent)();

	//构造函数，主要是输入按钮坐标 以及对应的图片素材路径
	BUTTON(float x, float y, char path[]){
		this->x = x;
		this->y = y;
		dir = std::string(path);
	}

	//用于初始化按钮资源的函数
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

	//用于绘制按钮单元的函数，涉及到悬停、单击和普通状态三种状态的转换，需要响应鼠标类
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
	//用于判定按钮是否处于鼠标下方的函数
	bool ifMouseOver(Mouse m){
		return m.isInArea(x, y, y + height, x + width);
	}
};