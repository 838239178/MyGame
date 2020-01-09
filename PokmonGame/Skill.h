#pragma once
#define FIRE 0
#define WATER 1
#define GLASS 2

class Skill {
public:
	int property;		//属性
	int power;			//威力（基础伤害）
	string name;		//技能名
	int PP;				//使用次数
	int PPmax;			//最大使用次数

	string music;
	string pic;
	void play();		//播放技能动画
};
void Skill::play()
{

}

