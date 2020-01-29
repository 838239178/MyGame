#pragma once
#define HPPOTION 0
#define POK 1
using namespace std;
typedef int TYPE;
class Item				//道具
{
public:
	IMAGE pic;
	IMAGE picB;
	string name;
	string describe;
	int size;			//回复量
	int value_buy;		//购买价格
	int value_sold;		//出售价格
	TYPE type;			//类型：hp，exp等
	int count;			//数量
	/*类函数*/
	string typestr();
	Item();
};
//class Bag
//{
//public:
//	list<Item>::iterator into;
//	int type;
//};
Item hp_potion;      //回血药
Item ball;
string Item::typestr() {			//返回物品的作用参数
	switch (type)
	{
	case HPPOTION:
		return "HP";
	}
}
Item::Item()
{
	pic = picB = NULL;
	name = "";
	describe = "";
	size = value_buy = value_sold = count = type = 0;
}
void itemstart()
{
	loadimage(&hp_potion.pic, "test\\hppotion.png");
	hp_potion.type = HPPOTION;
	hp_potion.name = "伤药";
	hp_potion.size = 30;
	hp_potion.count = 0;
	hp_potion.describe = "回复30点HP";

	loadimage(&ball.pic, "test\\ball.png");
	loadimage(&ball.picB, "test\\ballB.png");
	ball.type = POK;
	ball.name = "精灵球";
	ball.size = 0.2;
	ball.count = 0;
	ball.describe = "有小概率可以捕捉宝可梦";
}