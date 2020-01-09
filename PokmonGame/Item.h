#pragma once
#define HPPOTION 0
using namespace std;
typedef int TYPE;
class Item				//道具
{
public:
	IMAGE pic;
	string name;
	int size;			//回复量
	int value_buy;		//购买价格
	int value_sold;		//出售价格
	TYPE type;			//类型：hp，exp等
	int count;			//数量
	/*类函数*/
	string typestr();
};
string Item::typestr() {			//返回物品类型名称
	switch (type)
	{
	case HPPOTION:
		return "HP";
	}
}