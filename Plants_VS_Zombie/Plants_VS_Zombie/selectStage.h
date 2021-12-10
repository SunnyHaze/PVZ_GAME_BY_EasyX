#include "stdafx.h"
#include <graphics.h>
#include "EasyXPng.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include "pageItem.h"
#include "game.h"
//��������ҳ�����ռ�
namespace selectStage{
	int * pageControl; //������ҳ����������֮�䴫����Ϣ��ָ��
	Mouse m;
	// ��ʼ����ťԪ��
	BUTTON butt_return(412, 600, "\images\\buttons\\button_return_");
	BUTTON butt_stage1(412, 300, "\images\\buttons\\button_1_");
	BUTTON butt_stage2(412, 400, "\images\\buttons\\button_2_");
	BUTTON butt_stage3(412, 500, "\images\\buttons\\button_3_");
	IMAGE background;
	//����ά����ť������
	std::vector<BUTTON*> vButton;
	void returnButtonOnClick(){
		*pageControl = 0;
	}
	//�ؿ���Ծ�Ļص�����
	void gotoStage1(){
		game::loadStage(1);
		*pageControl = 2;
	}
	void gottostage2(){
		game::loadStage(2);
		*pageControl = 2;
	}
	void gottostage3(){
		game::loadStage(3);
		*pageControl = 2;
	}
	//��ʼ���������
	void startup(){
		//��ťԭ�����밴ť�б�
		vButton.push_back(&butt_return);
		vButton.push_back(&butt_stage1);
		vButton.push_back(&butt_stage2);
		vButton.push_back(&butt_stage3);
		//��ť��ʼ��ͼƬ
		for (auto &i : vButton){
			i->startup();
		}
		//�󶨰�ť�¼�����
		vButton[0]->ButtonEvent = &returnButtonOnClick;
		vButton[1]->ButtonEvent = gotoStage1;
		vButton[2]->ButtonEvent = gottostage2;
		vButton[3]->ButtonEvent = gottostage3;
		//���뱳��ͼƬ
		loadimage(&background, _T("\images\\welcome_back.png"));
	}
	//�������
	void draw(int *page){
		m.update();
		putimagePng(0, 0, &background);
		pageControl = page;
		for (auto &i : vButton){
			i->draw(m);
		}
	}
}