#include <iostream>
#include"game.h"
#include<graphics.h>    // 引用 EasyX 图形库 
#include <stdlib.h>
#include <conio.h>     // 包含按键检测、接收等函数 
#include <windows.h>
#include <time.h>
#include<process.h>     // 多线程
#include <mmsystem.h>   // 音乐播放的多媒体库
#pragma comment(lib,"winmm.lib")  //链接winmm.lib这个库
#pragma comment(lib, "Msimg32.lib")

int fun;   // 功能序号
int sc[7];  //存储分数
int newS;   // 新的分数
int height = 50;   // 排行榜间隔
struct Player
{
	int score[5];
}player[7];

IMAGE Fun1;
IMAGE Fun2;
IMAGE Fun3;
IMAGE Fun4;
IMAGE Details;
IMAGE Hint;
IMAGE Sort;
IMAGE Begin;

// 加载资源
void loadResources()
{
	loadimage(&Fun1, "res\\menu1.png", 450, 630);
	loadimage(&Fun2, "res\\menu2.png", 450, 630);
	loadimage(&Fun3, "res\\menu3.png", 450, 630);
	loadimage(&Fun4, "res\\menu4.png", 450, 630);
	loadimage(&Details, "res\\details.png", 450, 630);
	loadimage(&Hint, "res\\hint.png", 450, 630);
	loadimage(&Sort, "res\\sort.png", 450, 630);
	loadimage(&Begin, "res\\begin.png", 450, 630);

}

//提示窗
void promt()
{
	MessageBox(NULL, _T("游戏界面中：\n使用上下方向键控制，\n回车键进入相关功能!"), _T("温馨提示"), MB_OK | MB_ICONWARNING);
	
}

// 数据存储
void saveData(int newS)
{
	FILE *fp2;
	if ((fp2 = fopen("score.dat", "r")) == NULL)
		outtextxy(0, 0, _T("数据文件异常！"));
	for (int i = 0; i < 7; i++)
	{
		fscanf_s(fp2, "%d", player[i].score);
	}
	fclose(fp2);
	*player[6].score = newS;
	for (int i = 0; i < 7; i++)
	{
		for (int j = i + 1; j < 7; j++)
		{
			if (*player[i].score < *player[j].score)
			{
				int temp = *player[i].score;
				*player[i].score = *player[j].score;
				*player[j].score = temp;
				
			}
		}
	}
	if ((fp2 = fopen("score.dat", "w")) == NULL)
		outtextxy(0, 0, _T("数据文件异常！"));
	for (int i = 0; i < 7; i++)
	{
		fprintf_s(fp2, "%d\n", *player[i].score);
	}
	fclose(fp2);
}

// 功能1
void fun1()
{
	newS = game();
	saveData(newS);
}

// 功能2
void fun2()
{
	FILE *fp2;
	if ((fp2 = fopen("score.dat", "r")) == NULL)
		outtextxy(0, 0, _T("数据文件异常！"));
	int ii = 0;
	char mid2[11];
	putimage(0, 0, &Sort);
	for (int i = 0; i < 7; i++)
	{
		fscanf_s(fp2,"%d",player[i].score);
	}
	fclose(fp2);
	setbkmode(TRANSPARENT);  // 设置文字背景模式为 TRANSPARENT 实现输出文字无背景色 
	settextcolor(RGB(0, 0, 0));  // 2.设置文字颜色为 黑色 
	settextstyle(25, 0, _T("楷体")); // 3.设置文字大小和字体，参数依次为：高、宽、字体。
	for (int ii = 0; ii < 6; ii++)
	{
		itoa(*player[ii].score, mid2, 10);
		outtextxy(320, 233 + ii * height, mid2);
	}
	getch();
}

// 功能3
void fun3()
{
	putimage(0,0, &Details);
	getch();
}

// 接收用户选择
void Choice()
{
	int choice;
	choice = 1;
	putimage(0, 0, &Fun1);
	while (_kbhit)
	{
		char ch = getch();
		if (ch == 72)
		{
			choice--;
			if (choice == 0)
				choice = 4;
			
		}
		else if (ch == 80)
		{
			choice++;
			if (choice == 5)
				choice = 1;
		}
		else if (ch == '\r')
			break;
		if (choice==1)
		{
			putimage(0, 0, &Fun1);
		}
		else if (choice == 2)
		{
			putimage(0, 0, &Fun2);
		}
		else if (choice == 3)
		{
			putimage(0, 0, &Fun3);
		}
		else if (choice == 4)
		{
			putimage(0, 0, &Fun4);
		}
	}
	switch (choice)
	{
	case 1:
		fun1();
		break;
	case 2:
		fun2();
		break;
	case 3:
		fun3();
		break;
	case 4:
		exit(0);
	default:
		break;
	}
}

// 分支主函数
void menu()
{
	initgraph(450, 630);   //创建画布
	loadResources();   //加载资源
	mciSendString("open res/begin.mp3", 0, 0, 0);   //打开音乐
	mciSendString("play res/begin.mp3 from 0", 0, 0, 0);  //播放音乐
	putimage(0, 0, &Hint);   // 健康游戏忠告
	Sleep(2000);   // 暂停2秒
	promt();   //弹窗提示
	putimage(0, 0, &Begin);  //欢迎界面
	while (1)   //判断用户是否输入空格，输入空格则继续
	{
		if(getch() == ' ')
			break;
	}
	while (1)
	{
		Choice();  //菜单
	
	}
	
}