#include "stdafx.h"
#include <graphics.h>
#include "EasyXPng.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include "pageItem.h"


namespace welcome{
	int * pageControl;
	Mouse m;
	// ��ʼ����ťԪ��
	BUTTON butt_start(412, 300, "\images\\buttons\\button_start_");
	BUTTON butt_exit(412, 400, "\images\\buttons\\button_exit_");
	IMAGE background;
	//����ά����ť������
	std::vector<BUTTON*> vButton;
	//��ʼ��Ϸ��ť�¼�
	void startButtonOnClick(){
		*pageControl = 1;
	}
	//�˳���Ϸ��ť�¼�
	void exitButtonOnClick(){
		*pageControl = -1;
	}
	int *pagetmp; //������ҳ����������֮�䴫����Ϣ��ָ��
	//��ʼ���������
	void startup(){
		//��ťԭ�����밴ť�б�
		vButton.push_back(&butt_start);
		vButton.push_back(&butt_exit);
		//��ť��ʼ��ͼƬ
		for (auto &i : vButton){
			i->startup();
		}
		//�󶨰�ť�¼�����
		vButton[0]->ButtonEvent = &startButtonOnClick;
		vButton[1]->ButtonEvent = &exitButtonOnClick;
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