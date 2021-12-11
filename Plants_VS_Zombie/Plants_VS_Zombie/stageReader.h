#include "stdafx.h"
#pragma once
#include <fstream>
#include <sstream>
#include "json\jsonxx.h" // https://github.com/hjiang/jsonxx/tree/legacy 引入了此开源库
#include <vector>
namespace readStage{
	//组织了关卡数值全部信息的结构体
	struct stageInfo{
		int TIME;			//最后一个怪物渲染的时间
		int monsterNumber;	//一共渲染的怪物个数
		int plantNumber;	//本关可使用的植物个数
		int startSunlight;	//初始阳光数量
		std::vector<std::string> plantsCanUse;	//可以使用的植物名称
		std::vector<int> monsterTime;			// 怪物刷新的具体时间节点
		/*stageInfo(){
			TIME = 0;
			monsterNumber = 0;
			startSunlight = 0;
			plantNumber = 0;
			
		}
		stageInfo(stageInfo &obj){
			TIME = obj.TIME;
			monsterNumber = obj.monsterNumber;
			plantNumber = obj.plantNumber;
			startSunlight = obj.startSunlight;
			plantsCanUse = obj.plantsCanUse;
			monsterTime = obj.monsterTime;
		}*/
		//单纯的重载赋值运算符
		stageInfo operator=(stageInfo obj){
			TIME = obj.TIME;
			monsterNumber = obj.monsterNumber;
			plantNumber = obj.plantNumber;
			startSunlight = obj.startSunlight;
			plantsCanUse = obj.plantsCanUse;
			monsterTime = obj.monsterTime;
			return *this;
		}
	};
	std::string baseUrlHead(".\\stages\\");
	std::string baseUrlTail(".json");
	std::string GenerateURL(int stagenumber){	//用于生成储存有关卡信息的json文件的路径
		std::string url(baseUrlHead);
		url.append("0");
		url[url.length() - 1] += stagenumber;
		return url + baseUrlTail;
	}
	// 从文件中读取变量到内存中，输入为关卡号
	stageInfo ParseFromFile(int stagenumber){
		stageInfo tmp;
		std::string url = GenerateURL(stagenumber);
		std::cout << url << std::endl;
		std::ifstream ifs(url.c_str());
		std::stringstream buffer;
		buffer << ifs.rdbuf();
		jsonxx::Object jsonData;
		jsonData.parse(buffer, jsonData);
		tmp.TIME = jsonData.get<jsonxx::number>("TIME");
		tmp.monsterNumber = jsonData.get<jsonxx::number>("monsterNumber");
		tmp.plantNumber = jsonData.get<jsonxx::number>("plantNumber");
		tmp.startSunlight = jsonData.get<jsonxx::number>("startSunlight");
		for (int i = 0; i < tmp.plantNumber; i++){
			std::string plantName(jsonData.get<jsonxx::Array>("plantCanUse").get<std::string>(i) );
			tmp.plantsCanUse.push_back(plantName);
		}
		for (int i = 0; i < tmp.monsterNumber; i++){
			int a = jsonData.get<jsonxx::Array>("monsterTime").get<jsonxx::number>(i);
			tmp.monsterTime.push_back(a);
		}
		return tmp;
	}
	//测试用的函数，可以显示stageinfo结构体的具体参数
	void showStageInfo(stageInfo obj){
		printf("TIME:%d\n", obj.TIME);
		printf("monsterNumber:%d\n", obj.monsterNumber);
		printf("plantNumber:%d\n", obj.plantNumber);
		printf("startSunlight:%d\n", obj.startSunlight);
		printf("PlantCanUseNames:\n \t");
		for (int i = 0; i < obj.plantNumber; i++){
			printf("%s ", obj.plantsCanUse[i].c_str());
		}
		printf("\n");
		printf("Monster appear times:\n\t");
		for (int i = 0; i < obj.monsterNumber; i++){
			printf("%d ", obj.monsterTime[i]);
		}
		printf("\n");
	}
}