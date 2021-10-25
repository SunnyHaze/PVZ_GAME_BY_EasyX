// Test_20210831.cpp : 定义控制台应用程序的入口点。

#include "stdafx.h"
#include <graphics.h>
#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <math.h>

int score = 0;
int r = 10;
int FPS = 120;
int height = 600, width = 600;

int b_x = 50;
int b_y = 500;
float b_ay = 0.5;
float b_vx = 0, b_vy = -10;

float score_high;
void showScore(){
	settextstyle(30, 0, _T("Consolas"));
	char s_score[30];
	//_itoa_s(score, s_score, 10);
	sprintf(s_score, "%f", score_high);
	outtextxy(10, 10, s_score);
}

int rec_width = 15;
int rec_height = 100;
int rec_x = width - rec_width;
int rec_y = height / 2 - rec_height / 2;
int rec_vy = 10;

void resetBall(){
	b_x = 50;
	b_y = 500;
	b_vx = rand() % 30 + 20;
	b_vy = -(rand() % 30);
}

int frame_counter = 0;
void shootBall(){
	cleardevice();
	showScore();
	BeginBatchDraw();
	fillcircle(b_x, b_y, r);
	frame_counter++;
	b_x += b_vx;
	b_y += b_vy;
	b_vy += b_ay;
	if (b_y + r >= height && b_vy > 0){
		float delta = b_ay / 1000 * FPS * frame_counter / 2;
		float overrun = b_y + r - height;
		printf("%f\n", delta);
		b_vy = -0.8 *(sqrt(b_vy * b_vy - overrun) - delta);
		score_high = b_vy;
		frame_counter = 0;
	}

	if (b_x >= 700){
		resetBall();
	}
	rec_y += rec_vy;
	fillrectangle(rec_x, rec_y, rec_x + rec_width, rec_y + rec_height);
	if (rec_y < 0 || rec_y + rec_height > height){
		rec_vy = -rec_vy;
	}
	if (b_x + r >= rec_x && b_x - r <= rec_x + rec_width + b_vx && (b_y - r > rec_y && b_y + r < rec_y + rec_height)){
		resetBall();
		score++;
	}
	FlushBatchDraw();
	Sleep(1000 / FPS);
}

int main()
{
	initgraph(600, 600, EW_SHOWCONSOLE);
	while (1){
		cleardevice();
		BeginBatchDraw();
		shootBall();

		FlushBatchDraw();
		Sleep(1000 / FPS);
	}
	shootBall();
	_getch();
	closegraph();
	return 0;
}

