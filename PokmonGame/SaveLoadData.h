#pragma once
struct npcSave
{
	int x, y;					//坐标
	int state;					//当前动作；
	bool fight;					//是否具有进攻性  true为有 判断npc或者野生宝可梦
	int money;					//角色的钱
	int time;					//游玩时长
	int mapx, mapy;				//地图坐标
	void read(const Npc& tmp);
	void write(Npc& tmp);
};
void npcSave::read(const Npc& tmp)
{
	x = tmp.x;
	y = tmp.y;
	state = tmp.state;
	fight = tmp.fight;
	money = tmp.money;
}
void npcSave::write(Npc& tmp)
{
	tmp.x = x;
	tmp.y = y;
	tmp.state = state;
	tmp.fight= fight;
	tmp.money= money;
}
struct pokmonSave
{
	string name;		//名字
	int level;			//等级
	int property;		//属性
	int max_life;		//最大生命值
	int life;			//当前生命值
	int ATK;			//攻击力
	int DEF;			//防御力
	int INK;			//特攻
	int need_EXP;		//升级所需经验值
	int EXP;			//当前经验值	

	void read(const Pokemon& tmp);
};
struct itemSave
{
	string name;
	string describe;
	int size;			//回复量
	int value_buy;		//购买价格
	int value_sold;		//出售价格
	TYPE type;			//类型：hp，exp等
	int count;			//数量

	void read(const Item& tmp);
};
typedef struct npcSave npcSave;
typedef struct pokemonSave pokemonSave;
typedef struct itemSave itemSave;