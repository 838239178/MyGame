#pragma once
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
#define HPPOTION 0
#define POK 1
Item items[10];
#define hp_potion items[0]			//回血药
#define ball items[1]

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