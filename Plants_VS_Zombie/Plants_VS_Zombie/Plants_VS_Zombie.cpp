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
int activate_page = 2;
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

//用于初始化所有相关资源的函数
void startup(){
	welcome::startup();
	loading::startup();
	game::startup();
	author::startup();
}

int main()
{
	//初始化时间种子
	srand(time(NULL));
	//主要的渲染循环：
	initgraph(1024, 768, EW_SHOWCONSOLE);
	//初始化所有相关资源
	startup();
	//主要的渲染循环
	while (1)
	{
		clock_t start = clock();
		cleardevice();
		BeginBatchDraw();
		entry();
		FlushBatchDraw();
		printf("%d\n", start);
		clock_t current;
		while ((current = clock() - start ) <= 1000 / FPS){
			;
		}
	}
	closegraph();
	return 0;
}