import json
a = input("请输入要更改的关卡号：")
Url = "./stages/{}.json".format(a)
with open(Url,"w",encoding="GB2312") as f:
    data = {}
    times = input("请输入关卡总时间：")
    nums = input("请输入怪物总个数：")
    plantNum = input("请输入可以使用的植物个数：")
    startSunlight = input("请输入初始阳光个数：")
    data["TIME"] = int(times)
    data["monsterNumber"] = int(nums)
    data["plantNumber"] = int(plantNum)
    data["startSunlight"] = int(startSunlight)
    # 先读取可以使用的植物名称
    plants = []
    for i in range(int(plantNum)):
        plants.append(input("请输入可以使用的第{}个植物，一共{}个".format(i + 1,plantNum)))
    data["plantCanUse"] = plants
    MonsterTime = []
    for i in range(int(nums)):
        while True:
            mons = input("请输入第{}个怪物的出现时间，一共有{}个怪物。".format(i + 1,nums))
            if mons < times:
                MonsterTime.append(int(mons))
                break
            else:
                print("您输入的{}，超过了总时间{}，请重新输入：".format(mons,times))
    data["monsterTime"] = MonsterTime.sort()
    json.dump(data,f)
       
            
        