#include "stdafx.h"
#include <graphics.h>
#include "EasyXPng.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include "pageItem.h"

//��������ҳ�����ռ�
namespace author{
	int * pageControl;
	Mouse m;
	// ��ʼ����ťԪ��
	BUTTON butt_return(412, 300, "\images\\buttons\\button_return_");
	IMAGE background;
	//����ά����ť������
	std::vector<BUTTON*> vButton;
	//�˳���Ϸ��ť�¼�
	void returnButtonOnClick(){
		*pageControl = 0;
	}
	int *pagetmp; //������ҳ����������֮�䴫����Ϣ��ָ��
	//��ʼ���������
	void startup(){
		//��ťԭ�����밴ť�б�
		vButton.push_back(&butt_return);
		//��ť��ʼ��ͼƬ
		for (auto &i : vButton){
			i->startup();
		}
		//�󶨰�ť�¼�����
		vButton[0]->ButtonEvent = &returnButtonOnClick;
		//���뱳��ͼƬ
		loadimage(&background, _T("\images\\Author.png"));
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