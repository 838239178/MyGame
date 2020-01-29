#pragma once 
class Npc
{
public:
	IMAGE pic;					//原图
	IMAGE picB;				    //遮罩图
	IMAGE battlepic;			//战斗画面的图片（原图）
	IMAGE battlepicB;			//遮罩	
	string text;				//角色剧情文本的文件名
	vector<Pokemon> theMON;		//拥有的pokemon
	vector<Item> item;			//拥有的道具
	int x, y;					//位置
	int state;					//当前动作；
	int sizex, sizey;			//图片大小
	int Ri, Li, Di, Ui;			//控制人物上下左右的动画播放
	bool fight;					//是否具有进攻性  true为有
	int money;					//角色的钱
	int useNo;					//使用第几只Pokemon
};

Npc player;				//主角
vector<Npc> npc(10);	//路人

void npc_strat()
{
	//角色初始化
	player.x = 126;
	player.y = 250;
	player.sizex = 35;
	player.sizey = 30;
	player.Ri = 3;				//人物向右走坐标移动   3-->5
	player.Di = 0;				//1-->2
	player.Ui = 6;				//6-->8
	player.Li = 9;				//9-->11
	player.state = 0;
	player.fight = false;		
	player.useNo = 0;
	player.money = 100;
	player.theMON.push_back(Charmander);
	player.theMON.push_back(Ciken);
	hp_potion.count = 5;
	player.item.push_back(hp_potion);
	ball.count = 10;
	player.item.push_back(ball);
	

	npc[1].x = 100;
	npc[1].y = 150;
	npc[1].sizex = 35;
	npc[1].sizey = 30;
	npc[1].Ri = 3;
	npc[1].Di = 0;
	npc[1].Ui = 6;
	npc[1].Li = 9;
	npc[1].state = 0;
	npc[1].text = "npc1text.txt";
	npc[1].fight = true;
	npc[1].useNo = 0;
	npc[1].money = 100;
	npc[1].theMON.push_back(Ciken);
	hp_potion.count = 2;
	npc[1].item.push_back(hp_potion);

}
