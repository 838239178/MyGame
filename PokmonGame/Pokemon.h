#pragma once
#define MAXSIZE 10
#define NORMAL 0
#define FIRE 1
#define WATER 2
#define GLASS 3
typedef Item Potion;
using namespace std;
class Pokemon
{
public:
	int No;				//编号
	vector<Skill>skill; //拥有的技能
	string name;
	int property;		//属性
	int level;			//等级
	int max_life;		//最大生命值
	int life;			//当前生命值
	int ATK;			//攻击力
	int DEF;			//防御力
	int INK;			//特攻
	int need_EXP;		//升级所需经验值
	int EXP;			//当前经验值	

	IMAGE pic[2];			//战斗图	0:self 1:emy
	IMAGE picB[2];
	IMAGE piclist;		//列表小图标
	IMAGE piclistB;
	/*类函数*/
	void levelup();
	int use(Potion&);
	void hurt(double, double);
	void buff(double, double);
};

Pokemon Poks[MAXSIZE];
#define Ciken Poks[0]	//水鸡
#define Charmander Poks[1] //小火龙 

vector<Pokemon>Wild;

void Pokemon::levelup()				//升级
{
	need_EXP *= 1.5;
	level += 1;
	EXP = 0;
	ATK *= 1.1;
	max_life *= 1.3;
	life = max_life;
}
int Pokemon::use(Potion& item)		//使用道具
{
	switch (item.type)
	{
	case HPPOTION:
		if (life == max_life)
			return 0;
		life += item.size;
		if (life > max_life)
			life = max_life;
		break;
	default:
		return -1;
		break;
	}
	item.count--;
	return 1;
}
void Pokemon::hurt(double rate, double dmg)
{
	life -= dmg * rate;
	life = (life < 0) ? 0 : life;
}
