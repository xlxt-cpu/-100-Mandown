#include <iostream>
#include<graphics.h>    // 引用 EasyX 图形库 
#include <stdlib.h>
#include <conio.h>     // 包含按键检测、接收等函数 
#include <windows.h>
#include <time.h>
#include<process.h>     // 多线程
#include <mmsystem.h>   // 音乐播放的多媒体库
#pragma comment(lib,"winmm.lib")  //链接winmm.lib这个库
#pragma comment(lib, "Msimg32.lib")// 引用该库才能使用 AlphaBlend 函数，实现 png 透明显示 
using namespace std;


IMAGE Background;
IMAGE Brick1;
IMAGE Brick2;
IMAGE Brick3;
IMAGE Brick4;
IMAGE Brick5;
IMAGE Brick6;
IMAGE Brick7;
IMAGE Man0;
IMAGE Man1;
IMAGE Man2;
IMAGE Top;
IMAGE Over;
IMAGE Button;

// 砖块
struct brick
{
	int x;   //坐标
	int y;   //坐标
	int type;  //种类
	int flag;   // 板是否被踩过
	int breakTime; // 破碎时间
};
// 人物
struct man
{
	int x;   // 坐标
	int y;   // 坐标
	int hp;   //血量
	int walk;  // 移动状态：左 右，静止
	int flag;   // 判断人物是否站在板上
};


int xman = 26;      // 人物宽度
int yman = 34;      // 人物高度
int xwin = 432;     // 窗口宽度
int ywin = 559;     // 窗口高度
int xbrick = 97;    // 板子宽度
int ybrick = 16;    // 板子厚度
int xLWall = 25;    // 左墙
int xRWall = 425;   // 右墙
float vybrick = 1;    // 板子上升速度
int vxman = 5;      // 人物水平移速
float vyman = 4;      // 人物下落速度
int index;          // 板子序号
int jump = 45;     // 弹跳板弹跳高度
int fx = 2;          // 水平移动板移动速度
int msType = 0;    // 音效控制变量
char ch;            //接收键盘输入
int score = 0;      //分数
int timesc = 0;     //分数计时
int timehp = 0;     //回血计时
char temp1[10];    // unicode 编码存储字符用的 
char temp2[10];    // unicode 编码存储字符用的 


struct brick brick[11];
struct man man;

// 坐标移动函数
void gotoxy(int a, int b)
{
	int xx = 0x0b;
	HANDLE hOutput;
	COORD loc;
	loc.X = a;
	loc.Y = b;
	hOutput = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleCursorPosition(hOutput, loc);
	CONSOLE_CURSOR_INFO cursor;
	cursor.bVisible = FALSE;  // 隐藏控制台光标
	cursor.dwSize = sizeof(cursor);
	SetConsoleCursorInfo(hOutput, &cursor);  // 设置控制台光标状态


}

// png 图像透明背景输出函数
void transparentimage(IMAGE *dstimg, int x, int y, IMAGE *srcimg) {
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	// 结构体的第三个成员表示额外的透明度，0 表示全透明，255 表示不透明。  
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	// 使用 Windows GDI 函数实现半透明位图  
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}

// 加载资源
void loadRes()
{
	loadimage(&Background, "res\\BJ.bmp");
	loadimage(&Man0, "res\\man0.png", xman, yman);
	loadimage(&Man1, "res\\manleft.png", xman, yman);
	loadimage(&Man2, "res\\manright.png", xman, yman);
	loadimage(&Brick1, "res\\board1.bmp", xbrick, ybrick);
	loadimage(&Brick2, "res\\board2.bmp", xbrick, ybrick);
	loadimage(&Brick3, "res\\board3.bmp", xbrick, ybrick);
	loadimage(&Brick4, "res\\board4.bmp", xbrick, ybrick);
	loadimage(&Brick5, "res\\board5.bmp", xbrick, ybrick);
	loadimage(&Brick6, "res\\board6.bmp", xbrick, ybrick);
	loadimage(&Brick7, "res\\board7.bmp", xbrick, ybrick);
	loadimage(&Top, "res\\TOU.png",450,630);
	loadimage(&Over, "res\\over.png", 400, 304);
	loadimage(&Button, "res\\button.png", 100, 30);
	mciSendString("open res/begin.mp3", 0, 0, 0);
	mciSendString("open res/brick1.mp3", 0, 0, 0);
	mciSendString("open res/brick2.mp3", 0, 0, 0);
	mciSendString("open res/brick3.mp3", 0, 0, 0);
	mciSendString("open res/brick4.mp3", 0, 0, 0);
	mciSendString("open res/brick6.mp3", 0, 0, 0);
	mciSendString("open res/die.mp3", 0, 0, 0);
};

// 初始化
void start()
{
	score = 0;
	vyman = 4;      // 人物下落速度
	vybrick = 1;    // 板子上升速度
	srand(time(NULL));  //随机数种子
	for (index = 1; index < 11; index++)
	{
		brick[index].type = rand() % 6 + 1;
		brick[index].flag = 1;
		brick[index].x = rand() % (400 - xbrick) + 25;
		brick[index].y = 610 + 60 * index;
	}
	brick[0].x = 180;
	brick[0].y = 614;
	brick[0].type = 1;
	man.hp = 100;
	man.walk = 0;
	man.x = 210;
	man.y = 614 - yman;
	man.flag = 1;
	//putimage(brick[0].x, brick[0].y, &Brick1);
	//putimage(man.x, man.y, &Man0);

};

// 批量绘制画面
void draw()
{
	BeginBatchDraw();
	putimage(0, 0, &Top);   //背景
	putimage(5, 70, &Background);  //游戏边框
	//游戏界面
	for (index = 0; index < 11; index++)
	{
		if (brick[index].type == 1)
			putimage(brick[index].x, brick[index].y, &Brick1);
		else if (brick[index].type == 2)
			putimage(brick[index].x, brick[index].y, &Brick2);
		else if (brick[index].type == 3)
			putimage(brick[index].x, brick[index].y, &Brick3);
		else if (brick[index].type == 4)
			putimage(brick[index].x, brick[index].y, &Brick4);
		else if (brick[index].type == 5)
			putimage(brick[index].x, brick[index].y, &Brick5);
		else if (brick[index].type == 6 && brick[index].flag == 1 && brick[index].breakTime < 6)
			putimage(brick[index].x, brick[index].y, &Brick6);
		else if (brick[index].type == 6 && brick[index].flag == 1 && brick[index].breakTime >= 6)
			putimage(brick[index].x, brick[index].y, &Brick7);
	}
	if (man.walk == 0)
		transparentimage(NULL, man.x, man.y, &Man0);
	else if (man.walk == 1)
		transparentimage(NULL, man.x, man.y, &Man1);
	else if (man.walk == 2)
		transparentimage(NULL, man.x, man.y, &Man2);

	// 数据绘制
	setbkmode(TRANSPARENT);     // 设置文字背景模式为 TRANSPARENT 实现输出文字无背景色 
	settextcolor(RGB(255, 0, 0));    // 设置文字颜色为 红色 
	settextstyle(25, 0, _T("楷体")); // 设置文字大小和字体，参数依次为：高、宽、字体。其中0为自适应。
	itoa(man.hp, temp1, 10);
	itoa(score, temp2, 10);
	outtextxy(70, 45, temp1);
	outtextxy(330, 45, temp2);

	FlushBatchDraw();
};

// 回血,计分功能
void updateData()
{
	// 人物血量低于100时，回血
	if (man.hp < 100 && man.hp>0)
	{
		timehp++;
		if (timehp == 60)    //更新一个板加一点血
		{
			man.hp++;
			timehp = 0;
		}
	}
	// 计分
	timesc++;
	if (timesc % 60 == 0)   //每更新一个板加一分
	{
		score++;
	}
};

// 更新板子
void newBrick()
{
	for (index = 0; index < 11; index++)
	{
		if (brick[index].y <= 90)
		{
			brick[index].type = rand() % 6 + 1;
			brick[index].flag = 1;
			brick[index].y = 750;
			brick[index].x = rand() % (400 - xbrick) + 25;
		}
	}
};

// 与键盘输入无关的位置更新
void updateWithoutInput()
{
	//板子上升运动
	for (index = 0; index < 11; index++)
	{
		brick[index].y -= vybrick;
	}
	// 人物竖直方向运动
	for (index = 0; index < 11; index++)
	{
		// 在板上 
		if ((man.y <= brick[index].y - yman + vyman ) && (man.y >= brick[index].y - yman - vyman) && (man.x > brick[index].x - xman + 6) && (man.x < brick[index].x + xbrick - xman / 2))
		{
			if (brick[index].type == 1 || (brick[index].type == 2 && brick[index].flag == 0))
			{
				man.y -= vybrick;
				man.flag = 1;
				if (brick[index].flag == 1)
					msType = 1;
				brick[index].flag = 0;
			}
			else if (brick[index].type == 2 && brick[index].flag == 1)
			{
				man.hp -= 40;
				man.y -= vybrick;
				man.flag = 1;
				msType = 2;
				brick[index].flag = 0;
			}
			else if (brick[index].type == 3)
			{
				man.y -= jump;
				man.flag = 0;
				msType = 3;
				brick[index].flag = 0;
			}
			else if (brick[index].type == 4)
			{
				if (man.x > 25 && man.x < 425 - xman)
				{
					man.x -= fx;
				}
				man.y -= vybrick;
				man.flag = 1;
				if (brick[index].flag == 1)
					msType = 4;
				brick[index].flag = 0;
			}
			else if (brick[index].type == 5)
			{
				if (man.x > 25 && man.x < 425 - xman)
				{
					man.x += fx;
				}
				man.y -= vybrick;
				man.flag = 1;
				if (brick[index].flag == 1)
					msType = 4;
				brick[index].flag = 0;
			}
			else if (brick[index].type == 6 && brick[index].flag == 1)
			{
				man.y -= vybrick;
				if (brick[index].flag == 1 && brick[index].breakTime == 0)
					msType = 5;
				man.flag = 1;
				brick[index].breakTime++;
				if (brick[index].breakTime == 10)
				{
					brick[index].flag = 0;
					brick[index].breakTime = 0;
				}
			}
			vyman = 4;
			break;
		}
		// 不在板上
		man.flag = 0;
	}
	// 重力加速度实现
	if (man.flag == 0)
	{
		man.y += vyman;
		vyman += 0.1;
	}
};

// 与键盘输入有关的位置更新
void updateWithInput()
{
	// 判断用户输入
	if (GetAsyncKeyState(VK_LEFT) & 0x8000)
	{
		if (man.x > 25)
			man.x -= vxman;
		man.walk = 1;
	}
	else if (GetAsyncKeyState(VK_RIGHT) & 0x8000)
	{
		if (man.x < 425 - xman)
			man.x += vxman;
		man.walk = 2;
	}
	else
	{
		man.walk = 0;
	}
};

//加速功能
void aSpeed()
{
	if (score > 11&&score<80)
	{
		vybrick += 0.001;
		vyman += 0.0001;
	}
	else if (score > 80)
	{
		vybrick += 0.003;
		vyman += 0.003;
	}
}

// 游戏结束画面
void gameover()
{
	transparentimage(NULL, 25, 163, &Over);
	setbkmode(TRANSPARENT);     // 设置文字背景模式为 TRANSPARENT 实现输出文字无背景色 
	settextcolor(RGB(255, 0, 0));    // 设置文字颜色为 红色 
	settextstyle(125, 0, _T("楷体")); // 设置文字大小和字体，参数依次为：高、宽、字体。其中0为自适应。
	outtextxy(185, 310, temp2);
	
}

// 多线程播放音乐
void music(void *pArg)
{
	switch (msType)
	{
	case -1:
		mciSendString("play res/begin.mp3 from 0", 0, 0, 0);
		break;
	case 1:
		mciSendString("play res/brick1.mp3 from 0", 0, 0, 0);
		break;
	case 2:
		mciSendString("play res/brick2.mp3 from 0", 0, 0, 0);
		break;
	case 3:
		mciSendString("play res/brick3.mp3 from 0", 0, 0, 0);
		break;
	case 4:
		mciSendString("play res/brick4.mp3 from 0", 0, 0, 0);
		break;
	case 5:
		mciSendString("play res/brick6.mp3 from 0", 0, 0, 0);
		break;
	case 6:
		mciSendString("play res/die.mp3 from 0", 0, 0, 0);
		break;
	default:
		break;
	}
	msType = 0;
	//_endthread();
}

int game()
{
	loadRes();  //加载资源
	start();    //初始化
	while (man.hp > 0 && man.y < 630 && man.y>90)
	{
		draw();   //绘制画面
		updateWithInput();   // 与输入有关的位置更新
		updateWithoutInput();   //与输入无关的位置更新
		_beginthread(music, 0, 0);   //背景音效多线程
		newBrick();    //更新板块
		updateData();   //回血，计分功能
		aSpeed();     // 加速功能
		Sleep(40);    // 暂停40毫秒
	}
	EndBatchDraw();  //结束批量绘图 
	mciSendString("open res/die.mp3", 0, 0, 0);
	mciSendString("play res/die.mp3 from 0", 0, 0, 0);
	gameover();  // 游戏结束画面
	while (1)   //判断用户是否输入空格，输入空格则继续
	{
		if(getch() == ' ')
			break;
	};   //暂停看结果
	return score;   //返回当局分数
}

