#include "stdafx.h"
#pragma once
#include <fstream>
#include <sstream>
#include "json\jsonxx.h" // https://github.com/hjiang/jsonxx/tree/legacy 引入了此开源库
#include <vector>
namespace readStage{
	
	struct stageInfo{
		int TIME;
		int monsterNumber;
		int plantNumber;
		int startSunlight;
		std::vector<std::string> plantsCanUse;
		std::vector<int> monsterTime;
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
	std::string GenerateURL(int stagenumber){
		std::string url(baseUrlHead);
		url.append("0");
		url[url.length() - 1] += stagenumber;
		return url + baseUrlTail;
	}

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