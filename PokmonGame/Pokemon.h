#pragma once
typedef Item Potion;
using namespace std;
class Pokemon
{
public:
	string name;
	vector<Skill>skill; //拥有的技能
	int level;			//等级
	int property;		//属性
	int max_life;		//最大生命值
	int life;			//当前生命值
	int ATK;			//攻击力
	int DEF;			//防御力
	int need_EXP;		//升级所需经验值
	int EXP;			//当前经验值	

	IMAGE pic;			//战斗图
	IMAGE picB;
	IMAGE piclist;		//列表小图标
	IMAGE piclistB;
	string text;		//存档地址
	/*类函数*/
	void levelup();
	void use(Potion&);
	void hurt(int rate, int atk);
};

Pokemon Ciken;        //水鸡
Pokemon Charmander;   //小火龙

void pokemon_start()
{
	Ciken.max_life = 100;
	Ciken.level = 1;
	Ciken.ATK = 10;
	Ciken.need_EXP = 50;
	Ciken.EXP = 0;
	Ciken.life = Ciken.max_life;
	Ciken.property = WATER;
	Ciken.name = "水稚鸡";

	Charmander.max_life = 100;
	Charmander.level = 1;
	Charmander.ATK = 12;
	Charmander.need_EXP = 50;
	Charmander.EXP = 49;
	Charmander.life = Charmander.max_life;
	Charmander.property = FIRE;
	Charmander.name = "小火龙";
}

void Pokemon::levelup()				//升级
{
	need_EXP *= 1.5;
	level += 1;
	EXP = 0;
	ATK *= 1.1;
	max_life *= 1.3;
	life = max_life;
}
void Pokemon::use(Potion& item)		//使用道具
{
	switch (item.type)
	{
	case HPPOTION:
		if (life == max_life)
			return;
		life += item.size;
		if (life > max_life)
			life = max_life;
		break;
	}
	item.count--;
}
void Pokemon::hurt(int rate, int atk)
{
	life -= atk * rate;
	life = (life < 0) ? 0 : life;
}
