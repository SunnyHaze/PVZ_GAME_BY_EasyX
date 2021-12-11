#include "stdafx.h"
#pragma once
#include <fstream>
#include <sstream>
#include "json\jsonxx.h" // https://github.com/hjiang/jsonxx/tree/legacy �����˴˿�Դ��
#include <vector>
namespace readStage{
	//��֯�˹ؿ���ֵȫ����Ϣ�Ľṹ��
	struct stageInfo{
		int TIME;			//���һ��������Ⱦ��ʱ��
		int monsterNumber;	//һ����Ⱦ�Ĺ������
		int plantNumber;	//���ؿ�ʹ�õ�ֲ�����
		int startSunlight;	//��ʼ��������
		std::vector<std::string> plantsCanUse;	//����ʹ�õ�ֲ������
		std::vector<int> monsterTime;			// ����ˢ�µľ���ʱ��ڵ�
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
		//���������ظ�ֵ�����
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
	std::string GenerateURL(int stagenumber){	//�������ɴ����йؿ���Ϣ��json�ļ���·��
		std::string url(baseUrlHead);
		url.append("0");
		url[url.length() - 1] += stagenumber;
		return url + baseUrlTail;
	}
	// ���ļ��ж�ȡ�������ڴ��У�����Ϊ�ؿ���
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
	//�����õĺ�����������ʾstageinfo�ṹ��ľ������
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