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
struct itemSave
{
	int No;				//编号
	int count;			//数量

	void read(const Item& tmp);
	void write(Item&);
};	
struct sklSave
{
	int No;
	int PPmax;
	int PP;
};
struct pokeSave
{
	int No;				//编号
	int level;			//等级
	int max_life;		//最大生命值
	int life;			//当前生命值
	int ATK;			//攻击力
	int DEF;			//防御力
	int INK;			//特攻
	int need_EXP;		//升级所需经验值
	int EXP;			//当前经验值


	sklSave skl[4];
	int sklnum;
	void read(const Pokemon& tmp);
	void write(Pokemon& tmp);
};
typedef struct npcSave npcSave;
typedef struct pokemonSave pokemonSave;
typedef struct itemSave itemSave;
typedef struct sklSave sklSave;

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
	tmp.fight = fight;
	tmp.money = money;
}
void pokeSave::read(const Pokemon& tmp)
{
	No = tmp.No;
	level = tmp.level;
	max_life = tmp.max_life;
	life = tmp.life;
	ATK = tmp.ATK;
	DEF = tmp.DEF;
	INK = tmp.INK;
	need_EXP = tmp.need_EXP;
	EXP = tmp.EXP;
	sklnum = tmp.skill.size();
	sklSave s;
	for (int i = 0; i < tmp.skill.size(); i++) {
		s.No = tmp.skill[i].No;
		s.PP = tmp.skill[i].PP;
		s.PPmax = tmp.skill[i].PPmax;
		skl[i] = s;
	}
}
void pokeSave::write(Pokemon& tmp)
{
	tmp.No = No ;
	tmp.level = level ;
	tmp.max_life = max_life  ;
	tmp.life = life;
	tmp.ATK = ATK;
	tmp.DEF = DEF;
	tmp.INK = INK;
	tmp.need_EXP = need_EXP;
	tmp.EXP = EXP;
	Skill s;
	for (int i = 0; i < sklnum; i++) {
		s = skls[skl[i].No];
		s.PP = skl[i].PP;
		s.PPmax = skl[i].PPmax;
		if (i >= tmp.skill.size())
			tmp.skill.push_back(s);
		else
			tmp.skill[i] = s;
	}
}