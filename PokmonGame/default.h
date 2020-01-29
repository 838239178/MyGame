#pragma once

void playmic(string name, string odds = "");
void stopmic(string name);
void Settxt(int color = BLACK, int hight = 0, int weight = 400, int width = 0);
void darwmsg(string m);
template <class T, class S> int  CalculateDIS(T n1, S n2);		//计算距离

void playmic(string name, string odds)
{
	string ply = "play " + name + " " + odds;
	string sek = "seek " + name + " to start";
	mciSendString(sek.c_str(), 0, 0, 0);
	mciSendString(ply.c_str(), 0, 0, 0);
}
void stopmic(string name)
{
	string stp = "stop " + name;
	mciSendString(stp.c_str(), 0, 0, 0);
}
template <class T, class S>
int CalculateDIS(T n1, S n2)   //计算对象间的距离
{
	int x = n1.x - n2.x;
	int y = n1.y - n2.y;
	int dis = (int)sqrt(x * x + y * y);
	return dis;
}
void Settxt(int color, int hight, int weight, int width)
{
	LOGFONT f;
	settextcolor(color);
	gettextstyle(&f);					// 获取当前字体设置
	f.lfHeight = hight;						// 设置字体高度为
	f.lfWeight = weight;					//字体粗细
	f.lfWidth = width;                     //设置宽度
	_tcscpy(f.lfFaceName, _T("微软雅黑"));    // 设置字体
	//f.lfQuality = ANTIALIASED_QUALITY;    // 设置输出效果为抗锯齿  
	settextstyle(&f);                     // 应用字体样式
	setbkmode(TRANSPARENT);
}
