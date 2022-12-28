#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<cmath>
#include<Windows.h>
#include"json.hpp"
#include<fstream>

struct mapnode
{
	int x;
	int y;
};

const mapnode zero = { 32,32 };
const int windows_width = 768;
const int windows_height = 1024;
const int mapwid = 50;
const int maphig = 50;
const int chessradius = 25;

using namespace std;
using namespace nlohmann;
typedef long long ll;

void init();
void gotomain(int type);
void gotomain(bool ishistory, int type);
int choose();
void history();
void choosemode();
void choosehard(int type);
void tapto(int type);
mapnode findmap(int x, int y);

int datamap[20][20] = { 0 };
int pplayer1, pplayer2, eplayer1, eplayer2;
int nomode = -1;
ofstream fout;
ifstream fin;
json status;
json save[100];

int main()
{
	while (1)
	{
		init();
		choosemode();
		cin.get();
	}
	//结束
	return 0;
}

void init()
{
	fin.open("./saves/status.json");
	fin >> status;
	fin.close();

	for (int i = 0; i <= 15; ++i)
	{
		for (int j = 0; j <= 15; ++j)
		{
			datamap[i][j] = 0;
		}
	}
	pplayer1 = pplayer2 = eplayer1 = eplayer2 = 0;
	nomode = -1;

	initgraph(windows_width, windows_height);
	setbkcolor(WHITE);
	cleardevice();
	IMAGE bkimg;
	loadimage(&bkimg, L"./imgs/start.png", 768, 1024);
	putimage(0, 0, &bkimg);
	return;
}

void tapto(int type)
{
	if (type == 0)
	{
		pplayer1 = choose();
		pplayer2 = pplayer1 == 1 ? 2 : 1;
		gotomain(0);
		return;
	}
	else if (type == 1)
	{
		choosehard(1);
		return;
	}
	else if (type == 2)
	{
		choosehard(2);
		return;
	}
	else if (type == 3)
	{
		history();
		return;
	}
	return;
}

int choose()
{
	cleardevice();
	// 设置输出效果为抗锯齿
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 72;
	_tcscpy_s(f.lfFaceName, _T("黑体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(BLACK);

	setlinecolor(BLACK);
	circle(168, 418, 78.5);
	setfillcolor(BLACK);
	fillcircle(544, 418, 78.5);

	outtextxy(168, 149, _T("选择你的棋子"));

	while (1)
	{
		ExMessage msg = getmessage();
		if (msg.message == WM_LBUTTONDOWN)
		{
			if (msg.x >= 168 - 78.5 && msg.x <= 168 + 78.5 && msg.y >= 418 - 78.5 && msg.y <= 418 + 78.5)
			{
				cleardevice();
				return 1;
			}
			else if (msg.x >= 544 - 78.5 && msg.x <= 544 + 78.5 && msg.y >= 418 - 78.5 && msg.y <= 418 + 78.5)
			{
				cleardevice();
				return 2;
			}
		}
	}
	return 0;
}

void history()
{
	return;
}

void choosemode()
{
	while (1)
	{
		ExMessage msg = getmessage(EX_MOUSE);
		flushmessage(-1);

		//主页面初始选择
		int butname = -1;
		for (int i = 0; i < 4; ++i)
		{
			if (msg.x >= 42 && msg.x <= 42 + 205 && msg.y >= 614 + i * 99 && msg.y <= 614 + i * 99 + 69)
			{
				if (msg.message == WM_LBUTTONDOWN)
				{
					tapto(i);
					butname = -2;
					break;
				}
				butname = i;
				break;
			}
		}
		if (butname == -2)
		{
			break;
		}
		else if (butname == -1)
		{
			IMAGE bkimg;
			loadimage(&bkimg, L"./imgs/start.png", 768, 1024);
			putimage(0, 0, &bkimg);
			Sleep(5);
			continue;
		}
		else
		{
			IMAGE bkimg;
			loadimage(&bkimg, L"./imgs/start.png", 768, 1024);
			putimage(0, 0, &bkimg);
			rectangle(42, 614 + butname * 99, 42 + 205, 614 + butname * 99 + 69);
			Sleep(5);
			continue;
		}
	}
	return;
}

void choosehard(int type)
{
	return;
}

void gotomain(int type)
{
	if (type == 0)
	{
		IMAGE bkimg;
		loadimage(&bkimg, L"./imgs/main pvpve.png", 768, 1024);
		putimage(0, 0, &bkimg);
		
		status["sum"] = status["sum"] + 1;
		int thistimenum = status["sum"];
		save[thistimenum]["type"] = 0;
		save[thistimenum]["player1"] = pplayer1;
		save[thistimenum]["player2"] = pplayer2;
		save[thistimenum]["stepcnt"] = 0;
		save[thistimenum]["steps"] = {};
		
		vector<int> steps;
		

		
	}
	return;
}
void gotomain(bool ishistory, int type)
{
	return;
}

mapnode findmap(int x, int y)
{
	
}