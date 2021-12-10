#include "stdafx.h"
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>
#include "welcome.h"
#include "loading.h"
#include "game.h"
#include "author.h"
#include "stageReader.h"
#include "selectStage.h"

int activate_page = 0;
const int FPS = 60;
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
		selectStage::draw(&activate_page);
		break;
	case 2:
		loading::draw(&activate_page);
		break;
	case 3:
		game::draw(&activate_page);
		break;
	case 4:
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
	selectStage::startup();
	game::startup();
	author::startup();
/* ����StageReader.h�Ƿ���д��ȷ���ܷ������ȷ������
	readStage::stageInfo info;
	info = readStage::ParseFromFile(2);
	readStage::showStageInfo(info);*/
}
//���ĵĶ�����Ⱦѭ��
void MainGenerationLoop(){
	while (1)
	{
		clock_t start = clock();
		cleardevice();
		BeginBatchDraw();
		entry();
		FlushBatchDraw();
		clock_t current;
		while ((current = clock() - start) <= 1000 / FPS){
			;
		}
	}
}


int main()
{
	//��ʼ��ʱ������
	srand(time(NULL));
	//��Ҫ����Ⱦѭ����
	initgraph(1024, 768, EW_SHOWCONSOLE);
	//��ʼ�����������Դ
	startup();
	//��Ҫ����Ⱦѭ��
	MainGenerationLoop();
	
	closegraph();
	system("PAUSE");
	return 0;
}