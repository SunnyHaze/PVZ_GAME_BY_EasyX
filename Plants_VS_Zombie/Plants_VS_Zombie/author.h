#include "stdafx.h"
#include <graphics.h>
#include "EasyXPng.h"
#include <iostream>
#include <cstring>
#include <string>
#include <cmath>
#include <vector>
#include "pageItem.h"

//作者详情页命名空间
namespace author{
	int * pageControl;
	Mouse m;
	// 初始化按钮元件
	BUTTON butt_return(412, 300, "\images\\buttons\\button_return_");
	IMAGE background;
	//用于维护按钮的数组
	std::vector<BUTTON*> vButton;
	//退出游戏按钮事件
	void returnButtonOnClick(){
		*pageControl = 0;
	}
	int *pagetmp; //用于在页面与主函数之间传递信息的指针
	//初始化内容组件
	void startup(){
		//按钮原件填入按钮列表
		vButton.push_back(&butt_return);
		//按钮初始化图片
		for (auto &i : vButton){
			i->startup();
		}
		//绑定按钮事件函数
		vButton[0]->ButtonEvent = &returnButtonOnClick;
		//读入背景图片
		loadimage(&background, _T("\images\\Author.png"));
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