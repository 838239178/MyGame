/*
	口袋妖怪
模块：
	1.人物移动（包括 场景变换，行走动画）                    完成
		---》其他npc的移动；                                 初步完成
	2.地图碰撞区（墙壁 树木 河湖 山脉）                      局部完成
	3.游戏菜单  （包括 欢迎界面，背包，精灵列表）
		---》背包{数组)										 初步完成
		---》菜单											 初步完成
		---》精灵列表(数组)									 初步完成
		---> 开始界面										 未完成
		---》训练师名片										 完成
	4.事件系统  （包括 触发剧情，引起打斗，对话）
		---> 触发对战										 完成
		---》对话（文本考虑用文件方式只读）					 完成
		---》随机遇敌										 完成
	5.战斗系统  （场景绘制，技能选择，精灵状态变化，背包道具数量变化，技能动画）
		---》场景绘制										部分完成
		---》技能											初步完成
		---》逃跑											初步完成
		——》捕捉											初步完成
		---》切换											完成
	6.音乐（背景音乐，战斗音乐，技能音效)					大部分完成
	7.存档系统												未完成
	8.道具系统												初步完成
*/
// ！！!!!!!!：背包改成多重链表
// ！！：单独的技能函数
// !:技能特效框架构建好了，等待完善 （效率太低了，累了)
//2020.2.28 做好了火苗的特效 但是改了一堆声明 代码更乱了！ 感觉还不错吧，毕竟终于做出个特效来了
#include <iostream>
#include <string>
#include <easyx.h>
#include <vector>
#include <map>
#include <list>
#include <algorithm>
#include <fstream> 
#include <sstream>	
#include <graphics.h>
#include <conio.h>
#include <cstdlib>
#include <ctime>
#include <mmsystem.h>
#include "Item.h"
#include "default.h"
#include "Pokemon.h"
#include "Object.h"
#include "Npc.h"
#include "Skill.h"
#include "SaveLoadData.h"
#include "gamecpp声明.h"
#include "starup.h"
#pragma comment(lib, "winmm.lib")
using namespace std;

void load()
{
	FILE* saveData;
	saveData = fopen("save_player_default.txt","rb");
	npcSave temp_npc;
	/*玩家信息*/
	fread(&temp_npc, sizeof(npcSave), 1, saveData);
	temp_npc.write(player);
	timeSave = temp_npc.time;
	mapX = temp_npc.mapx;
	mapY = temp_npc.mapy;
	fclose(saveData);
	/*npc信息*/
	saveData = fopen("save_npc_default.txt", "rb");
	for (int i = 0; i < npc.size(); i++) {
		fread(&temp_npc, sizeof(npcSave), 1, saveData);
		if (feof(saveData)) break;
		temp_npc.write(npc[i]);
	}
	fclose(saveData);
}
void save()
{
	drawamsg("正在保存。。。");
	FILE* saveData;
	/*玩家信息*/
	saveData = fopen("save_player_default.txt", "wb");
	npcSave temp_npc;
	temp_npc.read(player);
	//获取游戏时长
	time(&gameEnd);
	int timetotal = difftime(gameEnd, gameStart) + timeSave;
	//
	temp_npc.mapx = mapX;
	temp_npc.mapy = mapY;
	temp_npc.time = timetotal;
	fwrite(&temp_npc, sizeof(npcSave), 1, saveData);
	fclose(saveData);
	/*npc信息*/
	saveData = fopen("save_npc_default.txt", "wb");
	for (int i = 0; i< npc.size();i++) {
		temp_npc.read(npc[i]);
		fwrite(&temp_npc, sizeof(npcSave), 1, saveData);
		if (feof(saveData)) break;
	}
	fclose(saveData);
	playmic("savemic");
	drawamsg("保存成功。。。");
	system("pause");
	updata();
}
void main_mapstart()
{
	mapSIZEx = 990;
	mapSIZEy = 926;
	mapX = 0;
	mapY = -mapSIZEy / 2;
	for (int i = 0; i < mapSIZEy; i++) {          //1:草坪 -1:障碍物  -2：物件
		for (int j = 0; j < mapSIZEx; j++) {
			if (j >= 151 && j <= 281 && i >= 568 && i <= 658)	//t1
				cover_glass[i][j] = -1;
			if (j >= 280 && j <= 416 && i >= 628 && i <= 640)	//k1 
				cover_glass[i][j] = -1;
			if (j >= 240 && j <= 260 && i >= 668 && i <= 678)	//b1
				cover_glass[i][j] = -2;
			if (j >= 34 && j <= 144 && i >= 538 && i <= 691)	//g1
				cover_glass[i][j] = 1;
		}
	}
}
void startup()
{
	initgraph(495, 463);

	//初始化
	main_mapstart();
	skill_startup();
	pokemon_start();
	itemstart();
	npc_strat();
	load();
	

	//音乐初始化
	mciSendString(_T("open test\\backmic.mp3 alias backmusic"), NULL, 0, NULL);
	mciSendString(_T("play backmusic repeat"), NULL, 0, NULL);
	mciSendString("open test\\chose.mp3 alias chosemic", 0, 0, 0);
	mciSendString("open test\\升级.mp3 alias levelupmic", 0, 0, 0);
	mciSendString("open test\\撞墙.mp3 alias movemic", 0, 0, 0);
	mciSendString("open test\\保存成功.mp3 alias savemic", 0, 0, 0);

	BeginBatchDraw();

	loadimage(&ground, "test\\glass.png");
	loadimage(&player.pic, "test\\ROLE.png");
	loadimage(&player.picB, "test\\ROLEblack.png");
	loadimage(&player.battlepic, "test\\ROLEfight.png");
	loadimage(&player.battlepicB, "test\\ROLEfightblack.png");
	loadimage(&npc[0].picB, "test\\npc2black.png");
	loadimage(&npc[0].pic, "test\\npc2.png");
	loadimage(&npc[0].battlepic, "test\\npc1fight.png");
	loadimage(&npc[0].battlepicB, "test\\npc1fightblack.png");
	loadimage(&battle, "test\\battle1.png");
	loadimage(&card, "test\\card.png");
	loadimage(&roleOnCard[0], "test\\roleOnCardB.png");
	loadimage(&roleOnCard[1], "test\\roleOnCard.png");
	loadimage(&msgbk[0], "test\\msgbk.png");		// 0 ：战斗的对话框
	loadimage(&msgbk[1], "test\\msgtalk.png");		// 1：与npc对话
	loadimage(&msgbk[2], "test\\menu.png");			//2：菜单
	loadimage(&statebk[1], "test\\fire.png");		//状态窗口
	loadimage(&statebk[2], "test\\water.png");		//状态窗口
	loadimage(&bottom[0], "test\\bt_attack.png");	//攻击按钮
	loadimage(&bottom[1], "test\\point.png");		//指示器
	loadimage(&bottom[2], "test\\bt_bag.png");
	loadimage(&bottom[3], "test\\bt_pokemon.png");
	loadimage(&bottom[4], "test\\bt_run.png");
	loadimage(&bottom[5], "test\\bt_point.png");
	loadimage(&bag[0], "test\\bag0.png");
	loadimage(&bag[1], "test\\bag1.png");
	loadimage(&bag[2], "test\\bag2.png");
	loadimage(&PKlist[0], "test\\pklist.png");
	loadimage(&PKlist[1], "test\\小框.png");
	loadimage(&PKlist[2], "test\\小框B.png");
	loadimage(&PKlist[3], "test\\大框.png");
	loadimage(&PKlist[4], "test\\大框B.png");

	board[1].x = 250;//246;
	board[1].y = 210;//668;
	board[1].text = "board1text.txt";
}
void cantmove(const Npc& role, bool can[])
{
	if (cover_glass[player.y - mapY + 10][player.x - mapX] != 0
		&& cover_glass[player.y - mapY+10][player.x - mapX] != 1)
		can[cDN] = 0;
	if (cover_glass[player.y - mapY - 10][player.x - mapX] != 0
		&& cover_glass[player.y - mapY - 10][player.x - mapX] != 1)
		can[cUP] = 0;
	if (cover_glass[player.y - mapY][player.x - mapX + 10] != 0
		&& cover_glass[player.y - mapY][player.x - mapX+10] != 1)
		can[cRT] = 0;
	if (cover_glass[player.y - mapY][player.x - mapX - 10] != 0
		&& cover_glass[player.y - mapY][player.x - mapX - 10] != 1)
		can[cLF] = 0;
}
template <class T>
void drawmsg(const T& role)  //绘制剧情对话框
{
	Settxt(BLACK);
	char s[100] = "";
	//加入文件读取文本
	ifstream msg(role.text);   //只读打开
	while (!msg.eof()) {
		playmic("chosemic");
		putimage(0, 403, &msgbk[1]);
		msg.getline(s, 100);     //读取一行
		outtextxy(60, 420, s);
		Sleep(150);  //防止闪频
		FlushBatchDraw();
		system("pause"); //按任意键继续
	}
	if (role.fight == true && !player.theMON[player.useNo].life) {
		putimage(0, 403, &msgbk[1]);
		strcpy(s, "你的首发精灵没有体力了");
		outtextxy(60, 420, s);
		FlushBatchDraw();
		system("pause");
	}
	msg.close(); //关闭
}
void drawamsg(string m)	//绘制一条信息
{
	Settxt(BLACK);
	putimage(0, 403, &msgbk[1]);
	outtextxy(60, 420, m.c_str());
	FlushBatchDraw();
}
void interface_switch()								//界面切换动画
{
	int i, j;
	i = 247, j = 0;                                 //从正上方开始画
	setlinecolor(BLACK);							//画线颜色设置为黑色
	while (!(i == 246 && j == 0))
	{
		line(247, 231, i, j);
		FlushBatchDraw();
		if (i == 640)
			j++;
		if (i == 0)
			j--;
		if (j == 600)
			i--;
		if (j == 0)
			i++;
	}
}
void updata()
{
	cleardevice();
	//播放动画
	putimage(mapX, mapY, &ground);
	//role1
	putimage(player.x, player.y, player.sizex, player.sizey, &player.picB, player.sizex * player.state, 0, NOTSRCERASE);  //原图白底，掩码图黑底
	putimage(player.x, player.y, player.sizex, player.sizey, &player.pic, player.sizex * player.state, 0, SRCINVERT);
	//npc[0]
	putimage(npc[0].x, npc[0].y, npc[0].sizex, npc[0].sizey, &npc[0].picB, npc[0].sizex * npc[0].state, 0, NOTSRCERASE);  //原图白底，掩码图黑底
	putimage(npc[0].x, npc[0].y, npc[0].sizex, npc[0].sizey, &npc[0].pic, npc[0].sizex * npc[0].state, 0, SRCINVERT);

	FlushBatchDraw();
}
void npcmove(Npc& people)
{
	//随机移动
	srand(time(0));
	int range = 1 - (-1) + 1;   // 移动频率范围【-1，1】；
	int val = rand() % range + -1;
	srand(time(0));
	int f = rand() % 5;  //随机方向；
	if (f == 0) {
		people.x += val;
		//改变运动状态
		if (val > 0)
			people.state = people.Ri;
		else
			people.state = people.Li;
		if (people.Ri++ == 5) people.Ri = 3;
		if (people.Li++ == 11) people.Li = 9;
	}
	if (f == 1) {
		people.y -= val;
		if (val < 0)
			people.state = people.Di;
		else
			people.state = people.Ui;
		if (people.Ui++ == 8) people.Ui = 6;
		if (people.Di++ == 2) people.Di = 1;
	}
}
//*****************CARD**************
void CardSys()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	playmic("openmic");
	cleardevice();
	Resize(0, 480, 320);
	CardShow();
	while (1) {	
		if (GetAsyncKeyState(0x44) & 0x8000) {     //D
			playmic("openmic");
			break;
		}
	}
	Resize(0, 495, 463);
	updata();
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
void CardShow()
{
	Settxt(BLACK);
	char s[10];
	putimage(0, 0, &card);			//背景
	putimage(360, 120, &roleOnCard[0], NOTSRCERASE);
	putimage(360, 120, &roleOnCard[1], SRCINVERT);
	outtextxy(280, 35, "IDNo.000000001");
	//名字
	outtextxy(50, 77, "NAME:");
	outtextxy(190, 77, "史上最憨训练师");
	//金钱
	outtextxy(50, 133, "MONEY:");
	sprintf(s, "%6dG", player.money);
	outtextxy(244, 133, s);
	//宝可梦数量
	outtextxy(50, 165, "POKeDEX:");
	sprintf(s, "%d", player.theMON.size());
	outtextxy(270, 165, s);
	//获取游戏时长
	time(&gameEnd);
	int timetotal = difftime(gameEnd, gameStart) + timeSave;
	int hh = timetotal / 3600;
	int mm = timetotal / 60 - hh * 60;
	int ss = timetotal - hh * 3600 - mm * 60;
	sprintf(s, "%02d:%02d:%02d",hh,mm,ss);
	outtextxy(50, 197, "TIME:");
	outtextxy(235, 197, s);
	FlushBatchDraw();
}
//*****************pokmonlist**************
void PKlistshow()
{
	int y1 = 17;    //17  +60  
	int y2 = 19;	//20  +60
	char lv[10];
	char hp[10];
	Settxt(WHITE);
	putimage(0, 0, &PKlist[0]);   //背景
	/*首发*/
	putimage(2, 39, 173, 144, &PKlist[4], (pki == 0) * 173, 0, NOTSRCERASE);   //大框
	putimage(2, 39, 173, 144, &PKlist[3], (pki == 0) * 173, 0, SRCINVERT);
	putimage(33, 67+(pkjump>0 && pki == 0)*3, &player.theMON[player.useNo].piclistB, NOTSRCERASE);   //首发精灵
	putimage(33, 67+(pkjump > 0 && pki == 0)* 3, &player.theMON[player.useNo].piclist, SRCINVERT);
	sprintf(lv, "%2d", player.theMON[player.useNo].level);
	sprintf(hp, "%3d  %3d", player.theMON[player.useNo].life, player.theMON[player.useNo].max_life);
	outtextxy(105, 97, lv);
	outtextxy(92, 130, hp);
	/*其余五只*/
	for (int i = 0; i < player.theMON.size(); i++, y1 += 55, y2 += 55) {
		if (i == player.useNo) {
			y1 -= 55;
			y2 -= 55;
			continue;
		}
		sprintf(lv, "%2d", player.theMON[i].level);
		sprintf(hp, "%3d  %3d", player.theMON[i].life, player.theMON[i].max_life);
		putimage(176, y2, 300, 48, &PKlist[2], 0, (pki == i) * 48, NOTSRCERASE);
		putimage(176, y2, 300, 48, &PKlist[1], 0, (pki == i) * 48, SRCINVERT);
		putimage(212, y1 + (pkjump > 0 && pki == i) * 3, &player.theMON[i].piclistB, NOTSRCERASE);
		putimage(212, y1 + (pkjump > 0 && pki == i) * 3, &player.theMON[i].piclist, SRCINVERT);
		outtextxy(280, y2 + 26, lv);
		outtextxy(393, y2 + 26, hp);
	}
	FlushBatchDraw();
}
void PKlistinput()
{
	if (_kbhit()) {
		playmic("chosemic");
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			(pki > 0) ? pki-- : 0;
		}
		else if (::GetAsyncKeyState(VK_DOWN) & 0x8000) {
			(pki < player.theMON.size() - 1) ? pki++ : 0;
		}
		else if (::GetAsyncKeyState(VK_LEFT) & 0x8000) {
			pki = player.useNo;
		}
		else if (::GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			pki = 1;
		}
		else if (GetAsyncKeyState(0x41) & 0x8000) {  //A
			if (onBattle && player.theMON[pki].life == 0) {
				Settxt(BLACK);
				outtextxy(20, 280, "没有反应");
				FlushBatchDraw();
				Sleep(1000);
				return;
			}
			else if (onBattle && player.useNo == pki) {
				Settxt(BLACK);
				outtextxy(20, 280, "已经在场上了");
				FlushBatchDraw();
				Sleep(1000);
				return;
			}
			swap(player.theMON[player.useNo],player.theMON[pki]);
			if (onBattle) {
				pklist_close = 0;
				turn *= -1;
			}
		}
		else if (GetAsyncKeyState(0x44) & 0x8000) { //D
			pklist_close = 0;
			playmic("openmic");
		}
	}
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
void PKlistsys()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	pklist_close = 1;
	pkjump = -250;
	cleardevice();
	Resize(0, 480, 322);
	while (pklist_close) {
		PKlistshow();
		PKlistinput();
		if (pkjump < 250)
			pkjump++;
		else
			pkjump = -250;
	}
	Resize(0, 495, 463);
	if(!onBattle) updata();
	
}
//*****************bagsys******************
void BagUse()
{
	Settxt(WHITE);
	int flag = 1;
	string outxt;
	if (codei < player.item.size()) {								//防止数组越界
		Item& potion = player.item[codei];
		if (potion.count > 0) {										//道具数量大于零
			char s[30];
			int t1 = 0, t2 = 0;			
			if ((flag = player.theMON[player.useNo].use(potion)) == -1) {	
				if (onBattle) {
					potion.count--;
					bag_close = 0;
					turn = -3;
					return;
				}
				else {
					outxt = "不能在这里用！";
					goto out;
				}
			}
			t1 = player.theMON[player.useNo].life;
			t2 = player.theMON[player.useNo].max_life;
			if (t1 == t2) {											//满血状态
				if (flag) {											//使用药剂后的
					strcpy(s, "已经完全回复");
					playmic("covermic");
				}
				else												//使用药剂前的
					strcpy(s, "不需要回复");
			}
			else {													//没满血的状态
				sprintf(s, "回复了%d点", potion.size);
				playmic("covermic");
			}
			outxt = player.theMON[player.useNo].name + "的" + potion.typestr() + s;
		}
		else {
			outxt = "已经没有剩余了";
		}
	out:
		outtextxy(100, 360, outxt.c_str());
		FlushBatchDraw();
		Sleep(1000);
		if (onBattle && flag) {
			bag_close = 0;
			turn = 1;
		}
	}
}
void BagInput()
{
	if (_kbhit()) {
		playmic("chosemic");
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			if (point2Y > 45)point2Y -= 45;
			if (codei > codemin) codei--;
		}
		else if (::GetAsyncKeyState(VK_DOWN) & 0x8000) {
			if (point2Y < 265)point2Y += 45;
			if (codei < codemax - 1) codei++;
		}
		else if (::GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (bagi > 0)
				bagi--;
			if (codemin > 0) {
				codemax -= 6;
				codemin -= 6;
				point2Y = 40;
				codei = codemin;
			}
		}
		else if (::GetAsyncKeyState(VK_RIGHT) & 0x8000) {
			if (bagi < 2)
				bagi++;
			if (codemax < 18) {
				codemax += 6;
				codemin += 6;
				point2Y = 40;
				codei = codemin;
			}
		}
		else if (GetAsyncKeyState(0x41) & 0x8000) {						//A键
			playmic("chosemic");
			BagUse();
		}
		else if (GetAsyncKeyState(0x44) & 0x8000) {   //D键退出
			stopmic("chosemic");
			playmic("openmic");
			bag_close = 0;
		}
	}
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
void BagShow()
{
	putimage(0, 0, &bag[bagi]);
	putimage(point2X, point2Y, &bottom[1]);
	Settxt(BLACK);
	int y = 40;
	char s[10];
	for (int i = codemin; i < min(codemax, player.item.size()); i++, y += 45) {
		sprintf(s, "X%d", player.item[i].count);
		outtextxy(220, y, player.item[i].name.c_str());
		outtextxy(420, y, s);
	}
	if (codei < player.item.size())
		putimage(23, 380, &player.item[codei].pic);
	FlushBatchDraw();
}
void BagSys()
{
	if (!onBattle) mciSendString("open test\\回复.mp3 alias covermic", 0, 0, 0);
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	bag_close = 1;
	bagi = 0;
	codei = 0;
	codemin = 0;
	codemax = 6;
	point2X = 200;
	point2Y = 40;
	cleardevice();
	while (bag_close) {
		BagShow();
		BagInput();
	}
	if (!onBattle) mciSendString("close covermic", 0, 0, 0);
}
//*****************menu*****************
void MenuShow()
{
	putimage(355, 50, &msgbk[2]);
	putimage(pointX, pointY, &bottom[1]);
	Settxt(BLACK);
	outtextxy(385, 70,  "背 包");
	outtextxy(385, 90,  "精 灵");
	outtextxy(385, 110, "名 片");
	outtextxy(385, 130, "存 档");
	FlushBatchDraw();
}
void MenuInput()
{
	if (_kbhit()) {
		playmic("chosemic");
		if (GetAsyncKeyState(VK_UP) & 0x8000) {
			if (pointY > 73)
				pointY -= 22;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000) {
			if (pointY < 310)
				pointY += 22;
		}
		else if (GetAsyncKeyState(0x41) & 0x8000) {
			playmic("chosemic");
			switch (pointY)
			{
			case 73://第一个选项
				BagSys();
				updata();
				break;
			case 95://第二个选项
				PKlistsys();
				break;
			case 117://第三个
				CardSys();
				break;
			case 139:
				save();
				break;
			}
		}
		else if (GetAsyncKeyState(0x44) & 0x8000) {
			menu_close = 0;  //再按一次打开菜单就退出菜单
		}
	}
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
void MenuSys()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	menu_close = 1;
	pointX = 365;
	pointY = 73;
	mciSendString("open test\\open.mp3 alias openmic", 0, 0, 0);
	playmic("openmic");

	while (menu_close) {
		MenuShow();
		MenuInput();
	}
	mciSendString("close openmic", 0, 0, 0);
	Sleep(200);
}
//——————————————————————
void battleSuccess(Npc& emy)
{
	char s[100];
	/*经验获得*/
	int rate = emy.theMON[emy.useNo].level - player.theMON[player.useNo].level;
	rate = (rate < 0) ? 0 : rate;
	const int getexp = rate * 8 + 10;
	string m = player.theMON[player.useNo].name;
	sprintf(s, "获得%d点经验", getexp);
	m += s;
	putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
	outtextxy(40, 300, m.c_str());
	FlushBatchDraw();
	Sleep(800);
	//升级判定
	if (getexp + player.theMON[player.useNo].EXP >= player.theMON[player.useNo].need_EXP) {
		player.theMON[player.useNo].levelup();
		playmic("levelupmic");
		int muchexp = getexp + player.theMON[player.useNo].EXP - player.theMON[player.useNo].need_EXP;
		muchexp = (muchexp < 0) ? 0 : muchexp;
		player.theMON[player.useNo].EXP += muchexp;
		/*升级信息*/
		putimage(0, 279, &msgbk[0]);		//覆盖之前的画面
		outtextxy(40, 300, "等级提升了！");
		FlushBatchDraw();
		Sleep(1000);
	}
	/*金钱获得*/
	const int getmoney = emy.money / 2;
	player.money += getmoney;
	sprintf(s, "从对手那边拿到了%dG", getmoney);
	putimage(0, 279, &msgbk[0]);			//覆盖之前的画面
	outtextxy(40, 300, s);
	FlushBatchDraw();
	Sleep(800);
	system("pause");
}
void battleLose()
{
	/*失去金钱*/
	char s[100] = "";
	const int losemoney = player.money * 0.2;
	player.money -= losemoney;
	sprintf(s, "失去了%dG", losemoney);
	putimage(0, 279, &msgbk[0]);			//覆盖之前的画面
	outtextxy(40, 300, s);
	FlushBatchDraw();
	Sleep(1000);
	system("pause");
	/*传送回治疗中心*/
}
void attackflush(Npc emy)
{
	//进攻动画
	if (turn == -2)
		playerflush = 15;
	else if (turn == 1)
		emyflush = 15;
	int tmp = turn;
	turn = 0;
	battleshow(emy);
	Sleep(150);
	battleshow(emy);
	turn = (tmp == -2) ? 1 : -1;
	//
}
void playerAttack(Npc& emy, int no)
{
	if (no >= player.theMON[player.useNo].skill.size())
		return;
	Skill& skl = player.theMON[player.useNo].skill[no];
	skl.PP--;
	string s = "你使用了" + skl.name;
	putimage(0, 279, &msgbk[0]);		//覆盖之前的画面
	outtextxy(40, 300, s.c_str());
	FlushBatchDraw();
	Sleep(600);
	double r = 1;   //伤害倍数；
	srand(time(NULL));
	//命中的判定
	if (3 == rand() % 10) {
		putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
		outtextxy(40, 300, "没有命中");
		FlushBatchDraw();
		Sleep(600);
		turn = 1;
		r = 0;
	}
	else {
		attackflush(emy);
		skl.play(emy);
		playmic("attackmic");
		//属性克制的判定
		putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
		if (skl > emy.theMON[emy.useNo].property) {
			outtextxy(40, 300, "效果拔群");
			r *= 1.5;
		}
		else if (skl < emy.theMON[emy.useNo].property) {
			outtextxy(40, 300, "效果微弱");
			r *= 0.5;
		}
		else
			outtextxy(40, 300, "效果一般");
		FlushBatchDraw();
		Sleep(600);
		//命中要害的判定
		srand(time(NULL));
		if (3 == rand() % 7) {
			putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
			outtextxy(40, 300, "命中要害");
			FlushBatchDraw();
			Sleep(600);
			r *= 1.5;
		}
	}
	double damage = 0;
	switch (skl.type)
	{
	case ATN:damage = emy.theMON[emy.useNo].ATK * 0.3 + skl.power; break;
	case INT:damage = emy.theMON[emy.useNo].INK * 0.3 + skl.power; break;
	}
	emy.theMON[emy.useNo].hurt(r, damage);
	Sleep(600);
}
void playerRun(Npc& emy)
{
	if (emy.fight == 1) {
		putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
		outtextxy(40, 300, "跟训练师对战时不能临阵脱逃！");
		FlushBatchDraw();
		Sleep(1000);
		return;
	}
	srand(time(NULL));
	int pro = 0;										//概率,（1~9）对应（10%~100%)
	Pokemon* emypok = &emy.theMON[emy.useNo];
	Pokemon* playerpok = &player.theMON[player.useNo];
	int levelgap = emypok->level - playerpok->level;	//等级差距
	if (levelgap >= 10)
		pro = 1;
	else if (levelgap >= 5)
		pro = 5;
	else
		pro = 7;
	int r = rand() % 9;
	if (r < pro)
		battle_close = RUN;
	else
	{
		string m = emypok->name + "死死盯着你不放";
		putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
		outtextxy(40, 300, m.c_str());
		FlushBatchDraw();
		Sleep(1000);
	}
}
void battleinput(Npc& emy)
{
	if (_kbhit()) {
		playmic("chosemic");
		if (GetAsyncKeyState(VK_LEFT) & 0x8000) {
			if (point3X > 38) point3X -= 214;
		}
		else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
		{
			if (point3X < 252) point3X += 214;
		}
		else if (GetAsyncKeyState(VK_UP) & 0x8000)
		{
			if (point3Y > 299) point3Y -= 80;
		}
		else if (GetAsyncKeyState(VK_DOWN) & 0x8000)
		{
			if (point3Y < 379) point3Y += 80;
		}
		else if (GetAsyncKeyState(0x41) & 0x8000) {
			if (point3X == 38 && point3Y == 299)			//左上按钮
			{
				if (turn == -1)
					turn = -2;
				else if (turn == -2) {
					playerAttack(emy, 0);
				}
			}
			else if (point3X == 252 && point3Y == 299)		//右上按钮
			{
				if (turn == -1) {								//如果是总菜单就是背包按钮
					BagSys();
					battleshow(emy);
				}
				else if (turn == -2) {
					playerAttack(emy, 1);
				}
			}
			else if (point3X == 38 && point3Y == 379) {		//左下
				if (turn == -1) {								
					switchPokmon(player);								//精灵列表
				}
				else if (turn == -2) {
					playerAttack(emy, 3);
				}
			}
			else if (point3X == 252 && point3Y == 379)		//右下按钮
			{
				if (turn == -1) {								//逃跑
					playerRun(emy);
				}
				else if (turn == -2) {
					playerAttack(emy, 3);
				}
			}
		}
		else if (GetAsyncKeyState(0x44) & 0x8000) {
			if (turn == -2)
				turn = -1;
		}
	}
	FlushMouseMsgBuffer();
}
int emySkill(Pokemon& emy)		//敌人的技能选择逻辑
{
	for (int i = 0; i < emy.skill.size(); i++) {
		if (emy.skill[i] > player.theMON[player.useNo].property)			//优先级1：克制对手属性的技能
			return i;
		else if (emy.property < player.theMON[player.useNo].property) {	//被对手克制
			if (emy.skill[i].type == DEBUFF)							//优先级2：使用削弱对手能力的技能
				return i;
			else if (emy.skill[i].type == BUFF)							//优先级3：使用提高自己能力的技能
				return i;
		}
	}
	return 0;
}
void emyAttack(Npc& emy)				//敌人的攻击方式
{
	/*
		1.血量充足则攻击
		2.血量不足回血
	*/
	if (emy.theMON[emy.useNo].life <= emy.theMON[emy.useNo].max_life * 0.25 && emy.item.size()> 0
		&& emy.item[0].count > 0)
	{
		outtextxy(40, 300, "对方使用了伤药");
		FlushBatchDraw();
		playmic("covermic");
		Sleep(800);
		//emy.theMON[emy.useNo].life += emy.item[0].size;
		emy.theMON[emy.useNo].use(emy.item[0]);
		turn = -1;
	}
	else {
		int i = emySkill(emy.theMON[emy.useNo]);
		Skill& skl = emy.theMON[emy.useNo].skill[i];
		skl.PP--;
		string s = "对方使用了" + skl.name;
		putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
		outtextxy(40, 300, s.c_str());
		FlushBatchDraw();
		Sleep(600);
		double r = 1;   //伤害倍数；
		srand(time(NULL));
		if (3 == rand() % 10) {
			putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
			outtextxy(40, 300, "没有命中");
			FlushBatchDraw();
			Sleep(600);
			r = 0;
			turn = -1;
		}
		else {
			attackflush(emy);
			playmic("attackmic");
			//属性克制判定
			putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
			if (skl > player.theMON[player.useNo].property) {
				outtextxy(40, 300, "效果拔群");
				r *= 1.5;
			}
			else if (skl < player.theMON[player.useNo].property) {
				outtextxy(40, 300, "效果微弱");
				r *= 0.5;
			}
			else
				outtextxy(40, 300, "效果一般");
			FlushBatchDraw();
			Sleep(600);
			//命中要害的判定
			srand(time(NULL));
			if (3 == rand() % 7) {
				Sleep(600);
				putimage(0, 279, &msgbk[0]);//覆盖之前的画面
				outtextxy(40, 300, "命中要害");
				FlushBatchDraw();
				Sleep(600);
				r = 1.5;
			}
		}
		double damage = 0;
		switch (skl.type)
		{
		case ATN:damage = emy.theMON[emy.useNo].ATK * 0.3 + skl.power; break;
		case INT:damage = emy.theMON[emy.useNo].ATK * 0.3 + skl.power; break;
		}
		player.theMON[player.useNo].hurt(r, damage);
		Sleep(600);
	}
}
void battlemusic_open()							//关闭背景音乐 打开战斗音乐
{
	mciSendString("stop backmusic", NULL, 0, NULL);

	mciSendString(_T("open test\\battlemic.mp3 alias battlemic"), NULL, 0, NULL);//战斗背景音乐
	mciSendString("open test\\攻击.mp3 alias attackmic", NULL, 0, NULL);
	mciSendString("open test\\濒死.mp3 alias deadmic", NULL, 0, NULL);
	mciSendString("open test\\回复.mp3 alias covermic", NULL, 0, NULL);
	mciSendString("open test\\投精灵球.mp3 alias throwmic", NULL, 0, NULL);
	mciSendString("open test\\胜利.mp3 alias successmic", 0, 0, 0);
	mciSendString("open test\\逃跑.mp3 alias runmic", 0, 0, 0);

	//技能音效
	mciSendString("open test\\火苗.mp3 alias f1mic", 0, 0, 0);
	mciSendString("open test\\水枪.mp3 alias w1mic", 0, 0, 0);
}
void battlemusic_close()
{
	mciSendString("close battlemic", NULL, 0, NULL);
	mciSendString("close attackmic", NULL, 0, NULL);
	mciSendString("close successmic", 0, 0, 0);
	mciSendString("close deadmic", 0, 0, 0);
	mciSendString("close covermic", 0, 0, 0);
	mciSendString("close throwmic", 0, 0, 0);
	mciSendString("close runmic", 0, 0, 0);
	mciSendString("close f1mic", 0, 0, 0);
	mciSendString("play backmusic repeat", NULL, 0, NULL);
}
void battlestart(Npc& emy)														//战斗开始的场景（只播放一次）
{
	battlemusic_open();
	mciSendString(_T("play battlemic repeat"), NULL, 0, NULL);
	interface_switch();
	cleardevice();
	Settxt(BLACK);
	/*画面 1*/
	putimage(0, 0, &battle);  //场景
	putimage(0, 279, &msgbk[0]);  //对话框
	putimage(0, 149, 136, 130, &player.battlepicB, 0, 0, NOTSRCERASE);	 //原图白底，掩码图黑底
	putimage(0, 149, 136, 130, &player.battlepic, 0, 0, SRCINVERT);
	putimage(330, 64, 120, 120, &emy.battlepicB, 0, 0, NOTSRCERASE);  //原图白底，掩码图黑底
	putimage(330, 64, 120, 120, &emy.battlepic, 0, 0, SRCINVERT);
	FlushBatchDraw();
	Sleep(500);
	/*画面 2*/
	string s;
	if (emy.money != 0) s = "对方发起了挑战";
	else s = "野生的" + emy.theMON[emy.useNo].name + "跳了出来";
	outtextxy(50, 300, s.c_str());
	putimage(0, 0, &battle);  //场景
	putimage(0, 149, 135, 130, &player.battlepicB, 138, 0, NOTSRCERASE);	 //原图白底，掩码图黑底
	putimage(0, 149, 135, 130, &player.battlepic, 138, 0, SRCINVERT);
	putimage(330, 64, 120, 120, &emy.battlepicB, 0, 0, NOTSRCERASE);  //原图白底，掩码图黑底
	putimage(330, 64, 120, 120, &emy.battlepic, 0, 0, SRCINVERT);
	FlushBatchDraw();
	Sleep(1000);
	/*画面 3*/
	putimage(0, 279, &msgbk[0]);  //对话框覆盖
	string msg = "就决定是你了，" + player.theMON[player.useNo].name;
	outtextxy(50, 300, msg.c_str());
	FlushBatchDraw();
	playmic("throwmic");
	for (int i = 269; i <= 579; i += 155) {
		putimage(0, 0, &battle);  //场景
		putimage(0, 149, 155, 130, &player.battlepicB, i, 0, NOTSRCERASE);	 //原图白底，掩码图黑底
		putimage(0, 149, 155, 130, &player.battlepic, i, 0, SRCINVERT);
		putimage(330, 64, 120, 120, &emy.battlepicB, 0, 0, NOTSRCERASE);  //原图白底，掩码图黑底
		putimage(330, 64, 120, 120, &emy.battlepic, 0, 0, SRCINVERT);
		FlushBatchDraw();
		Sleep(100);
	}
	Sleep(300);

	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
void battlemenu()
{
	Pokemon temp = player.theMON[player.useNo];
	if (turn == -1) {												//总菜单
		putimage(point3X, point3Y, &bottom[5]);		//指示器 
		putimage(40, 300, &bottom[0]);				//攻击按钮
		putimage(254, 300, &bottom[2]);				//背包
		putimage(40, 380, &bottom[3]);				//精灵列表
		putimage(254, 380, &bottom[4]);				//逃跑
	}
	else if (turn == -2) {											//技能菜单
		putimage(point3X, point3Y, &bottom[5]);		//指示器 
		int i = 0;
		char s[10];
		for (int y = 300; y <= 380; y += 80) {
			for (int x = 40; x <= 254; x += 214) {
				Settxt(BLACK);
				outtextxy(x, y, temp.skill[i].name.c_str());
				Settxt(BLACK, 20, 20);
				sprintf(s, "PP:%2d/%2d", temp.skill[i].PP, temp.skill[i].PPmax);
				outtextxy(x + 115, y + 30, s);
				if (i++ == temp.skill.size() - 1) {
					i = -1;
					break;
				}
			}
			if (i == -1) break;
		}
	}
}
void battleshow(const Npc& emy)  //绘制战斗场景
{
	cleardevice();
	char hp[50];
	char lv[10];
	char exp[50];
	if (player.theMON[player.useNo].life < (double)player.theMON[player.useNo].max_life * 0.25) {
		if (!onPlay) {
			playmic("deadmic", "repeat");
			onPlay = 1;
		}
	}
	else {
		stopmic("deadmic");
		onPlay = 0;
	}
	putimage(0, 0, &battle);						//场景
	putimage(0, 279, &msgbk[0]);					//对话框
	if (playerflush != 15) battlemenu();	
	Settxt(BLACK);
	//玩家
	putimage(280, 220, &statebk[player.theMON[player.useNo].property]);//状态窗口		
	putimage(30 + playerflush, 159, &player.theMON[player.useNo].picB[0], NOTSRCERASE);//主角宠物
	putimage(30 + playerflush, 159, &player.theMON[player.useNo].pic[0], SRCINVERT);
	sprintf(hp, "HP: %3d/%3d", player.theMON[player.useNo].life, player.theMON[player.useNo].max_life);
	sprintf(lv, "LV: %2d", player.theMON[player.useNo].level);
	sprintf(exp, "EXP:%3d/%3d", player.theMON[player.useNo].EXP, player.theMON[player.useNo].need_EXP);
	outtextxy(290, 225, hp);
	outtextxy(420, 225, lv);
	outtextxy(391, 247, exp);
	//敌人
	putimage(30, 50, &statebk[emy.theMON[emy.useNo].property]);//状态窗口	
	putimage(320 - emyflush, 60, &emy.theMON[emy.useNo].picB[1], NOTSRCERASE);
	putimage(320 - emyflush, 60, &emy.theMON[emy.useNo].pic[1], SRCINVERT);  //emy
	sprintf(hp, "HP: %3d/%3d", emy.theMON[emy.useNo].life, emy.theMON[emy.useNo].max_life);
	sprintf(lv, "LV: %2d", emy.theMON[emy.useNo].level);
	sprintf(exp, "EXP:%3d/%3d", emy.theMON[emy.useNo].EXP, emy.theMON[emy.useNo].need_EXP);
	outtextxy(40, 55, hp);
	outtextxy(170, 55, lv);
	outtextxy(141, 75, exp);

	FlushBatchDraw();

	playerflush = emyflush = 0;
}
void switchPokmon(Npc& charater)
{
	if (charater.fight == true) //如果是npc
	{
		charater.useNo++;
		turn *= -1;
		return;
	}
	else {
		PKlistsys();
		if (turn == -1) return; //如果直接按d退回
		cleardevice();
		Settxt(BLACK);//480 322
		putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
		outtextxy(40, 300, "你已经尽力了");
		FlushBatchDraw();
		Sleep(800);
		string s = player.theMON[player.useNo].name + ",接下来靠你了";
		putimage(0, 279, &msgbk[0]);    //覆盖之前的画面
		outtextxy(40, 300, s.c_str());
		FlushBatchDraw();
		Sleep(800);
	}
}
int is_over(const Npc& emy)   //判断战斗是否结束
{
	if (battle_close == RUN)
		return RUN;
	if (battle_close == CATCH)
		return CATCH;
	if (player.theMON[player.useNo].life <= 0) {
		int flag = 1;
		for (int i = 0; i < player.theMON.size(); i++)
		{
			if (player.theMON[i].life > 0) {
				flag = 0;
				break;
			}
		}
		if (flag) return LOSE;
		else switchPokmon(player);
	}
	if (emy.theMON[emy.useNo].life <= 0) {
		if (emy.useNo == emy.theMON.size() - 1) return VICTOR;
		else switchPokmon((Npc&)emy);
	}
	return 1;
}
void battlefinish(Npc& emy)
{
	cleardevice();
	putimage(0, 0, &battle);  //场景
	putimage(0, 279, &msgbk[0]);  //对话框
	//主角
	putimage(0, 149, 140, 130, &player.battlepicB, 0, 0, NOTSRCERASE);  //原图白底，掩码图黑底
	putimage(0, 149, 140, 130, &player.battlepic, 0, 0, SRCINVERT);
	//敌人
	putimage(330, 64, 120, 120, &emy.battlepicB, 0, 0, NOTSRCERASE);  //原图白底，掩码图黑底
	putimage(330, 64, 120, 120, &emy.battlepic, 0, 0, SRCINVERT);
	char s[100] = "";
	putimage(0, 279, &msgbk[0]);
	stopmic("deadmic");
	switch (battle_close)
	{
	case LOSE:
		strcpy(s, "你输了");
		outtextxy(40, 300, s);
		FlushBatchDraw();
		Sleep(1000);
		battleLose();
		break;
	case VICTOR:		
		stopmic("battlemic");
		playmic("successmic");
		strcpy(s, "你赢的了胜利");
		outtextxy(40, 300, s);
		FlushBatchDraw();
		Sleep(2000);
		battleSuccess(emy);
		break;
	case RUN:		
		stopmic("battlemic");
		playmic("runmic");
		strcpy(s, "逃跑成功了");
		outtextxy(40, 300, s);
		FlushBatchDraw();
		Sleep(2000);
		break;
	case CATCH:
		stopmic("battlemic");
		playmic("successmic");
		strcpy(s, "把捕捉到的宝可梦放回了背包");
		outtextxy(40, 300, s);
		FlushBatchDraw();
		Sleep(2000);
		player.theMON.push_back(emy.theMON[emy.useNo]);
	}
	battlemusic_close();
}
void battlesys(Npc& emy)   //战斗系统
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	battlestart(emy);
	battle_close = 1;
	onBattle = 1;
	turn = -1;				//回合变化
	playerflush = 0;
	point3X = 38;
	point3Y = 299;
	while (battle_close == 1) {
		if (turn == 1)
		{
			emyAttack(emy);
		}
		else if(turn == -1 || turn == -2)
		{
			battleinput(emy);
		}
		else if (turn == -3)
		{
			PokemonCatch(emy);
		}
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		battleshow(emy);
		battle_close = is_over(emy);
	}
	emy.fight = (battle_close == VICTOR) ? 0 : 1;
	battlefinish(emy);
	onBattle = 0;
}
void WildMet()
{
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	/*判定 13.3%*/
	srand(time(0));
	int r = rand() % 15;
	if (r >= 2)return;
	/*创建临时作战敌人*/
	Npc emy_temp;
	emy_temp.useNo = 0;
	emy_temp.money = 0;
	emy_temp.fight = 0;
	emy_temp.theMON.push_back(Wild[rand() % (Wild.size())]);
	battlesys(emy_temp);
}
void PokemonCatch(const Npc& emy)
{
	const Pokemon& pok = emy.theMON[emy.useNo];	
	/*提示信息*/
	putimage(0, 279, &msgbk[0]);//覆盖之前的画面
	outtextxy(40, 300, "去吧！精灵球！");
	FlushBatchDraw();
	Sleep(1000);
	/*扔球动画*/
	playmic("throwmic");
	for (int x = 30; x <= 320; x+=2) {
		int y = 0.0052 * x * x - 2.1 * x + 217.27;
		battleshow(emy);
		putimage(x, y, &ball.picB, NOTSRCERASE);
		putimage(x, y, &ball.pic, SRCINVERT);
		FlushBatchDraw();
		Sleep(1);
	}
	/*把宝可梦变成球*/
	putimage(320, 60,120,120,&battle, 320, 60);
	putimage(360, 160, &ball.picB, NOTSRCERASE);
	putimage(360, 160, &ball.pic, SRCINVERT);
	FlushBatchDraw();
	/*捕捉判定*/
	srand(time(NULL));
	int cnt = 0;		
	int pro = 0;
	if (pok.life <= pok.max_life * 0.25)
		pro = 7;
	else if (pok.life <= pok.max_life * 0.5)
		pro = 6;
	else
		pro = 5;
	putimage(0, 279, &msgbk[0]);//覆盖之前的画面
	for (int i = 1; i <= 3; i++) {
		if ((rand() % 10) < pro) {
			cnt++;
		}
		else
			break;
		outtextxy(40+i*5, 300, ". ");
		FlushBatchDraw();
		Sleep(1000);
	}
	if (cnt == 3) {
		outtextxy(40, 300, "成功了！");
		FlushBatchDraw();
		Sleep(1000);
		battle_close = CATCH;
		return;
	}
	else if (cnt == 2){
		outtextxy(40, 300, "就差一点点！");
		FlushBatchDraw();
		Sleep(1000);
	}
	else {
		outtextxy(40, 300, "不行，失败了！");
		FlushBatchDraw();
		Sleep(1000);
	}
	turn = 1;
}
//____________________________________________
void scanINPUT()
{
	bool can[5];
	can[0] = can[1] = can[2] = can[3] = can[4] = 1;
	cantmove(player, can);
	if (GetAsyncKeyState(VK_RIGHT) & 0x8000) {
		if (cover_glass[player.y - mapY][player.x - mapX] == 1)
			WildMet();
		if (can[cRT]) {
			//if (player.x < 463)
			//	if (mapX + mapSIZEx > 495)
			//		player.x += 2;
			//	else
			//		player.x += 4;
			if (/*mapX + mapSIZEx > 495*/1) {
				mapX -= SPEED;
				npc[0].x -= SPEED;
			}
		}
		else playmic("movemic");
		//改变运动状态
		player.state = player.Ri;
		if (player.Ri++ == 5)
			player.Ri = 3;
	}
	else if (::GetAsyncKeyState(VK_LEFT) & 0x8000) {
		if (cover_glass[player.y - mapY][player.x - mapX] == 1)
			WildMet();
		if (can[cLF]) {
			//if (player.x > 0)
			//	if (mapX < 0)
			//		player.x -= 2;
			//	else
			//		player.x -= 4;
			if (/*mapX < 0*/1) {
				mapX += SPEED;
				npc[0].x += SPEED;
			}
		}
		else playmic("movemic");
		player.state = player.Li;
		if (player.Li++ == 11)
			player.Li = 9;
	}
	else if (GetAsyncKeyState(VK_UP) & 0x8000) {
		if (cover_glass[player.y - mapY][player.x - mapX] == 1)
			WildMet();
		if (can[cUP]) {
			//if (player.y > 0)
			//	if (mapY < 0)
			//		player.y -= 2;
			//	else
			//		player.y -= 3;
			if (/*mapY < 0*/1) {
				mapY += SPEED;
				npc[0].y += SPEED;
			}
		}
		else playmic("movemic");
		player.state = player.Ui;
		if (player.Ui++ == 8)
			player.Ui = 6;
	}
	else if (::GetAsyncKeyState(VK_DOWN) & 0x8000) {
		if (cover_glass[player.y - mapY][player.x - mapX] == 1)
			WildMet();
		if (can[cDN]) {
			//if (player.y < 428)
			//	if (mapY + mapSIZEy > 464)
			//		player.y += 2;
			//	else
			//		player.y += 3;
			if (/*mapY + mapSIZEy > 464*/1) {
				mapY -= SPEED;
				npc[0].y -= SPEED;
			}
		}
		else playmic("movemic");
		player.state = player.Di;
		if (player.Di++ == 2)
			player.Di = 1;
	}
	//对话
	else if (GetAsyncKeyState(0x41) & 0x8000) {
		int i = cover_glass[player.y - mapY - 10][player.x - mapX];
		if (i <= -2) {
			drawmsg(board[abs(i) - 1]);
		}
		else if (CalculateDIS(player, npc[0]) < 15) {
			drawmsg(npc[0]);
			if (npc[0].fight == true && player.theMON[player.useNo].life)
				battlesys(npc[0]);
		}
		Sleep(200);
	}
	//打开菜单
	else if (GetAsyncKeyState(0x44) & 0x8000) {
		MenuSys();
	}
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
}
int main()
{
	time(&gameStart);
	startup();
	FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
	while (1) {
		FlushConsoleInputBuffer(GetStdHandle(STD_INPUT_HANDLE));
		scanINPUT();
		updata();
		Sleep(70);
		npcmove(npc[0]);
	};
	EndBatchDraw();
	closegraph();
	time(&gameEnd);
	mciSendString("close chosemic", 0, 0, 0);
	mciSendString("close backmic", 0, 0, 0);
	mciSendString("close battlemic", 0, 0, 0);
	mciSendString("close movemic", 0, 0, 0);
	mciSendString("close levelupmic", 0, 0, 0);
	return 0;
}
