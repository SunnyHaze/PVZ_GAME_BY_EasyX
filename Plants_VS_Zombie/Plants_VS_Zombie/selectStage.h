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
//作者详情页命名空间
namespace selectStage{
	int * pageControl; //用于在页面与主函数之间传递信息的指针
	Mouse m;
	// 初始化按钮元件
	BUTTON butt_return(412, 600, "\images\\buttons\\button_return_");
	BUTTON butt_stage1(412, 300, "\images\\buttons\\button_1_");
	BUTTON butt_stage2(412, 400, "\images\\buttons\\button_2_");
	BUTTON butt_stage3(412, 500, "\images\\buttons\\button_3_");
	IMAGE background;
	//用于维护按钮的数组
	std::vector<BUTTON*> vButton;
	void returnButtonOnClick(){
		*pageControl = 0;
	}
	//关卡跳跃的回调函数
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
	//初始化内容组件
	void startup(){
		//按钮原件填入按钮列表
		vButton.push_back(&butt_return);
		vButton.push_back(&butt_stage1);
		vButton.push_back(&butt_stage2);
		vButton.push_back(&butt_stage3);
		//按钮初始化图片
		for (auto &i : vButton){
			i->startup();
		}
		//绑定按钮事件函数
		vButton[0]->ButtonEvent = &returnButtonOnClick;
		vButton[1]->ButtonEvent = gotoStage1;
		vButton[2]->ButtonEvent = gottostage2;
		vButton[3]->ButtonEvent = gottostage3;
		//读入背景图片
		loadimage(&background, _T("\images\\welcome_back.png"));
	}
	//绘制组件
	void draw(int *page){
		m.update();
		putimagePng(0, 0, &background);
		pageControl = page;
		for (auto &i : vButton){
			i->draw(m);
		}
	}
}