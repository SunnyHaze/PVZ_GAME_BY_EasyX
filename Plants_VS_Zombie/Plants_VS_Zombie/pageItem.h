#pragma once
#include "stdafx.h"
#include "EasyXPng.h"
#include <iostream>
#include <graphics.h>
//��ͷ�ļ��漰������ҳ����ԭ���ķ�װ

//����ҳ���������¼��������
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

//���ڻ��ư�ť���Ҵ�����ť�¼��İ�ť��
class BUTTON {
public:
	IMAGE img[3];
	float x, y, width, height;
	int status = 0;
	std::string dir;
	void(*ButtonEvent)();

	//���캯������Ҫ�����밴ť���� �Լ���Ӧ��ͼƬ�ز�·��
	BUTTON(float x, float y, char path[]){
		this->x = x;
		this->y = y;
		dir = std::string(path);
	}

	//���ڳ�ʼ����ť��Դ�ĺ���
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

	//���ڻ��ư�ť��Ԫ�ĺ������漰����ͣ����������ͨ״̬����״̬��ת������Ҫ��Ӧ�����
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
	//�����ж���ť�Ƿ�������·��ĺ���
	bool ifMouseOver(Mouse m){
		return m.isInArea(x, y, y + height, x + width);
	}
};