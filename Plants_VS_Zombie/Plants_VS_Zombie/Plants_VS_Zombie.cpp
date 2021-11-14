#include "stdafx.h"
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include "welcome.h"
#include "loading.h"
#include "game.h"
#include "author.h"
int activate_page = 0;
const int FPS = 120;
void entry(){
	switch (activate_page)
	{
	case -1:
		exit(0);
		break;
	case 0:
		welcome::draw(&activate_page);
		break;
	case 1:
		loading::draw(&activate_page);
		break;
	case 2:
		game::draw(&activate_page);
		break;
	case 3:
		author::draw(&activate_page);
		break;
	default:
		break;
	}
}

//���ڳ�ʼ�����������Դ�ĺ���
void startup(){
	welcome::startup();
	loading::startup();
	game::startup();
	author::startup();
}

int main()
{
	srand(time(NULL));
	//��Ҫ����Ⱦѭ����
	initgraph(1024, 768, EW_SHOWCONSOLE);
	//��ʼ�����������Դ
	startup();
	//��Ҫ����Ⱦѭ��
	while (1)
	{
		cleardevice();
		BeginBatchDraw();
		entry();
		FlushBatchDraw();
		Sleep(1000 / FPS);
	}
	closegraph();
	return 0;
}