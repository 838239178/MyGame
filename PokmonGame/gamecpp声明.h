#pragma once

#define cUP 1
#define cLF 2
#define cDN 3
#define cRT 4

#define CATCH -3
#define RUN -2
#define LOSE -1
#define VICTOR 0

#define SPEED 4

void startup();													//初始化参数
void main_mapstart();											//main地图初始化
void updata();													//刷新画面
void npcmove(Npc&);												//npc随机移动
void scanINPUT();												//检查输入
void cantmove(const Npc&, bool[]);								//障碍物判断
void interface_switch();										//界面切换动画
void welcom_flush();											//开场动画
template <class T> void drawmsg(const T&);						//显示一个对话框
void drawamsg(string m);
//-------------------存档------------------
void save();
void load();
//-------------------菜单------------------
void MenuSys();
void MenuShow();
void MenuInput();
//------------------背包-------------------
void BagSys();
void BagShow();
void BagInput();
void BagUse();
//-----------------名片-------------------
void CardSys();
void CardShow();
//----------------宝可梦列表---------------
void PKlistsys();
void PKlistshow();
void PKlistinput();
//-------------------战斗--------------------
void battlesys(Npc&);				//战斗系统
void battleshow(const Npc&);		//绘制战斗场景
void battlestart(Npc&);				//开场动画及初始化
void battlefinish(Npc&);			//战斗结束动画
void battleinput(Npc&);				//战斗时输入检测
void battleSuccess(Npc&);			//战斗胜利动画
void battleLose();					//战斗失败效果
void battlemusic_open();			//战斗相关音乐打开
void battlemusic_close();			//………………关闭
void battlemenu();					//战斗时的菜单
void playerAttack(Npc&, int);		//玩家攻击动画及效果
void playerRun(Npc&);				//逃跑
void emyAttack(Npc&);				//敌人进攻
int emySkill(Pokemon&);				//敌人技能使用逻辑
void attackflush(Npc);				//攻击动画
int is_over(const Npc&);
void switchPokmon(Npc&);			//切换精灵
void WildMet();						//野生遭遇战
void PokemonCatch(const Npc&);

IMAGE ground;			//背景
IMAGE msgbk[10];		//对话框 (0:战斗 1：对话 2:菜单）
IMAGE bottom[10];		//按钮 （0：攻击 1:指示器）
IMAGE bag[3];			//三个背包
IMAGE PKlist[5];		//0：背景图，1：选择框，2：选择框掩码，3：大框，4：大框掩码图；
IMAGE statebk[10];		//战斗时的技能窗口  （0：火 1：水）
IMAGE battle;			//战斗画面
IMAGE card;				//名片背景
IMAGE roleOnCard[2];	//名片人像


int mapSIZEx, mapSIZEy;				//地图大小
int mapX, mapY;						//地图坐标
int cover_glass[1000][1000];		//地图g
/*menu*/
int pointX, pointY;					//菜单指示器
int menu_close;						//控制关闭菜单
/*pklist*/
int pklist_close;					//控制关闭列表
int pki;							//精灵列表的序号
int pkjump;							//控制列表中的精灵上下跳动
/*bag*/
int bag_close;						//控制关闭背包
bool onBattle;						//是否在战斗中打开背包
int bagi;							//当前背包页面序号
int codei;							//当前选中的物品序号
int codemin, codemax;				//背包当前页面中最小物品序号和最大物品序号
int point2X, point2Y;				//背包指示器
/*battle*/
int battle_close;					//控制战斗结束
int turn;							//1:敌人，-1:玩家1，-2：玩家2， -3：捕捉
int point3X, point3Y;				//战斗菜单的指示器
int playerflush;					//控制玩家攻击动画
int emyflush;						//控制敌人攻击动画
int onPlay;							//控制濒死音效播放
/*game_time*/
time_t gameStart;
time_t gameEnd;
int timeSave;
