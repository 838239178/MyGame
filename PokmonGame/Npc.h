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
	int fight;					//是否具有进攻性  true为有 判断npc或者野生宝可梦
	int money;					//角色的钱
	int useNo;					//使用第几只Pokemon
	Npc() { x = y = state = sizex = sizey = Ri = Li = Di = Ui = fight = money = useNo = 0; }
};

vector<Npc> npc(10);	//路人
void npc_strat();												//main地图角色初始化
Npc player;				//主角

