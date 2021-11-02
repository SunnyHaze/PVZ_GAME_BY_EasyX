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

namespace loading{
	const int WIDTH = 1024;
	const int HEIGHT = 768;
	const int Bar = 40;
	float rate = 0.0;
	IMAGE background;
	void startup(){
		loadimage(&background, _T("\images\\loading_back.png"));
		;
	}

	void drawBar(){
		setfillcolor(GREEN);
		fillrectangle(WIDTH * 0.1, (HEIGHT * 0.9 - Bar), WIDTH* 0.1 + 0.8 * rate * WIDTH, HEIGHT * 0.9);
		int i = rand() % 100;
		float incresement = 0.01;
		incresement *= (1.0 * i / 100);
		rate += incresement;
		char process[10];
		sprintf(process, "%.2f%%", rate * 100);
		setbkmode(TRANSPARENT);
		settextstyle(Bar / 2, 0, _T("ºÚÌå"));
		settextcolor(WHITE);
		outtextxy(WIDTH / 2, (HEIGHT * 0.9 - Bar) + 14, _T(process));
	}

	void draw(int *page){
		drawBar();
		putimagePng(0, 0, &background);
		if (rate >= 1){
			(*page)++;
		}
	}
}