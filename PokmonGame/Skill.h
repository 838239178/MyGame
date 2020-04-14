#pragma once
#include "Npc.h"
/*属性*/
#define NORMAL 0
#define FIRE 1
#define WATER 2
#define GLASS 3
/*技能类型*/
#define BUFF 0			//对自己增益
#define ATN 1			//物理伤害
#define INT 2			//特殊伤害
#define DEBUFF 3		//削弱对手

class Skill {
public:
	int No;				//编号
	int property;		//属性
	int power;			//威力（基础伤害）
	string name;		//技能名
	int PP;				//使用次数
	int PPmax;			//最大使用次数
	int type;			//技能类型（物攻，特攻，buff）

	IMAGE pic[4];		//0:遮罩 1：图片 2：遮罩 3：图片 …………
	void (*play)(Npc&);		//播放技能动画
	bool operator < (const int& a);
	bool operator > (const int& a);

	Skill() { property = power = PP = PPmax = type = 0; }
};
Skill skls[MAXSIZE];	
#define f_1 skls[0]	//火苗
#define w_1 skls[1]	//水枪
#define n_1 skls[2]	//撞击
			
/*单独技能函数*/
void f1(Npc&);
void w1(Npc&);
void n1(Npc&);
//
bool Skill::operator< (const int& a)
{
	if (a == property) return 0;
	switch (a)
	{
	case FIRE:
		switch (property)
		{
		case WATER:return 0;
		case GLASS:return 1;
		default: return 0;
		}
		break;
	case WATER:
		switch (property)
		{
		case FIRE:return 1;
		case GLASS:return 0;
		default:return 0;
		}
		break;
	case GLASS:
		switch (property)
		{
		case WATER:return 1;
		case FIRE:return 0;
		default:return 0;
		}
		break;
	}
}
bool Skill::operator> (const int& a) 
{
	if (a == property || a == NORMAL || property == NORMAL) return 0;
	return !Skill::operator<(a);
}
/*
敌方坐标大概位置 （350，100）
我方坐标大概位置 （30，217.27）
*/
void f1(Npc& n)
{
	int who = 1;
	if (n.fight == -1)
		who = 0;
	pair<int, int> role[2];
	role[0].first = 30;
	role[0].second = 217;
	role[1].first = 350;
	role[1].second = 100;
	//part1
	if (who == 1) {			//我方攻击敌方
		for (int x = 30; x < 320; x += 2) {
			int y = -0.33 * x + 217.27;
			battleshow(n);
			putimage(x, y, &f_1.pic[0], NOTSRCERASE);
			putimage(x, y, &f_1.pic[1], SRCINVERT);
			FlushBatchDraw();
		}
	}
	else {				//敌方攻击我方
		for (int x = 320; x > 30; x -= 2) {
			int y = -0.33 * x + 217.27;
			battleshow(n);
			putimage(x, y, &f_1.pic[0], NOTSRCERASE);
			putimage(x, y, &f_1.pic[1], SRCINVERT);
			FlushBatchDraw();
		}
	}
	playmic("f1mic");
	//part2
	for (int i = 0; i < 4; i ++) {
		//cleardevice();
		putimage(role[who].first, role[who].second, 47, 74, &f_1.pic[2], i*47, 0,NOTSRCERASE);
		putimage(role[who].first, role[who].second, 47, 74, &f_1.pic[3], i*47, 0, SRCINVERT);
		FlushBatchDraw();
		Sleep(100);
		battleshow(n);
	}
}
void w1(Npc& n)
{
	int who = 1;
	if (n.fight == -1)
		who = 0;
	pair<int, int> role[2];
	role[0].first = 30;
	role[0].second = 217;
	role[1].first = 350;
	role[1].second = 100;
	//part1
	for (int y = role[who].second; y < role[who].second+50; y++) {
		int x = role[who].first;
		battleshow(n);
		putimage(x, y, &w_1.pic[0], NOTSRCERASE);
		putimage(x, y, &w_1.pic[1], SRCINVERT);
		FlushBatchDraw();
		Sleep(1);
	}
	playmic("w1mic");
	//part2
	for (int i = 0; i < 2; i++) {
		putimage(role[who].first, role[who].second, 53, 72, &w_1.pic[2], i*53, 0, NOTSRCERASE);
		putimage(role[who].first, role[who].second, 53, 72, &w_1.pic[3], i*53, 0, SRCINVERT);
		FlushBatchDraw();
		Sleep(200);
		battleshow(n);
	}
}
void n1(Npc& n) 
{
	return;
}