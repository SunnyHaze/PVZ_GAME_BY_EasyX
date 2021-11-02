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

namespace game{
	IMAGE background;
	void startup(){
		loadimage(&background, _T("\images\\background.png"));
	}

	void draw(int *page){
		putimagePng(0, 0, &background);
	}
}