#pragma once
void pokemon_start();
void skill_startup();
void npc_strat();
void itemstart();

void pokemon_start()
{
	//图片
	/*小火龙*/
	loadimage(&Charmander.pic[0], "test\\firedragon_self.png");
	loadimage(&Charmander.picB[0], "test\\firedragonblack_self.png");
	loadimage(&Charmander.pic[1], "test\\firedragon_emy.png");
	loadimage(&Charmander.picB[1], "test\\firedragonblack_emy.png");
	loadimage(&Charmander.piclist, "test\\小火龙图标.png");
	loadimage(&Charmander.piclistB, "test\\小火龙图标B.png");
	/*水稚鸡*/
	loadimage(&Ciken.pic[0], "test\\ciken_self.png");
	loadimage(&Ciken.picB[0], "test\\cikenblack_self.png");
	loadimage(&Ciken.pic[1], "test\\ciken_emy.png");
	loadimage(&Ciken.picB[1], "test\\cikenblack_emy.png");
	loadimage(&Ciken.piclist, "test\\水稚鸡图标.png");
	loadimage(&Ciken.piclistB, "test\\水稚鸡图标B.png");
	//基本属性
	Ciken.No = 0;
	Ciken.max_life = 100;
	Ciken.level = 1;
	Ciken.ATK = 10;
	Ciken.INK = 12;
	Ciken.need_EXP = 50;
	Ciken.EXP = 0;
	Ciken.life = Ciken.max_life;
	Ciken.property = WATER;
	Ciken.skill.push_back(n_1);
	Ciken.skill.push_back(w_1);
	Ciken.name = "水稚鸡";

	Charmander.No = 1;
	Charmander.max_life = 100;
	Charmander.level = 1;
	Charmander.ATK = 12;
	Charmander.INK = 10;
	Charmander.need_EXP = 50;
	Charmander.EXP = 0;
	Charmander.life = Charmander.max_life;
	Charmander.property = FIRE;
	Charmander.skill.push_back(f_1);
	Charmander.skill.push_back(n_1);
	Charmander.name = "小火龙";

	Wild.push_back(Ciken);
	Wild.push_back(Charmander);
}
void skill_startup()
{
	//特效
	loadimage(&f_1.pic[0], "test\\火苗part1B.png");
	loadimage(&f_1.pic[1], "test\\火苗part1.png");
	loadimage(&f_1.pic[2], "test\\火苗part2B.png");
	loadimage(&f_1.pic[3], "test\\火苗part2.png");
	loadimage(&w_1.pic[0], "test\\水枪part1B.png");
	loadimage(&w_1.pic[1], "test\\水枪part1.png");
	loadimage(&w_1.pic[2], "test\\水枪part2B.png");
	loadimage(&w_1.pic[3], "test\\水枪part2.png");

	f_1.No = 0;
	f_1.property = FIRE;
	f_1.power = 15;
	f_1.name = "火苗";
	f_1.PP = 20;
	f_1.PPmax = 20;
	f_1.type = INT;
	f_1.play = f1;

	w_1.No = 1;
	w_1.property = WATER;
	w_1.power = 15;
	w_1.name = "水枪";
	w_1.PP = 20;
	w_1.PPmax = 20;
	w_1.type = INT;
	w_1.play = w1;

	n_1.No = 2;
	n_1.property = NORMAL;
	n_1.power = 12;
	n_1.name = "撞击";
	n_1.PP = 20;
	n_1.PPmax = 20;
	n_1.type = ATN;
	n_1.play = n1;
}
void npc_strat()
{
	//角色初始化
	player.x = 240;// 126;
	player.y = 231;// 250;
	player.sizex = 35;
	player.sizey = 30;
	player.Ri = 3;				//人物向右走坐标移动   3-->5
	player.Di = 0;				//1-->2
	player.Ui = 6;				//6-->8
	player.Li = 9;				//9-->11
	player.state = 0;
	player.fight = -1;
	player.useNo = 0;
	player.money = 100;
	//player.theMON.push_back(Charmander);
	//player.theMON.push_back(Ciken);
	hp_potion.count = 5;
	player.item.push_back(hp_potion);
	ball.count = 10;
	player.item.push_back(ball);


	npc[0].x = 100;
	npc[0].y = 150;
	npc[0].sizex = 35;
	npc[0].sizey = 30;
	npc[0].Ri = 3;
	npc[0].Di = 0;
	npc[0].Ui = 6;
	npc[0].Li = 9;
	npc[0].state = 0;
	npc[0].text = "npc1text.txt";
	npc[0].fight = true;
	npc[0].useNo = 0;
	npc[0].money = 100;
	npc[0].theMON.push_back(Ciken);
	hp_potion.count = 2;
	npc[0].item.push_back(hp_potion);

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