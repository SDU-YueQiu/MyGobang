#include<cstdio>
#include<cstdlib>
#include<iostream>
#include<graphics.h>
#include<conio.h>
#include<cmath>
#include<Windows.h>
#include"json.hpp"
#include<fstream>
#include<ctime>
#include<string>
#include<vector>

using namespace std;
using namespace nlohmann;
typedef long long ll;

struct mapnode
{
	int x;
	int y;
};
typedef struct treenode
{
	long long val;
	int x;
	int y;
	int player;
	int alpha;
	int beta;

	bool operator < (const treenode& a) const
	{
		return val > a.val;
	}

	vector<treenode*>child;
};

const mapnode zero = { 32,32 };
const int windows_width = 768;
const int windows_height = 1024;
const int mapwid = 50;
const int maphig = 50;
const int chessradius = 25;

void init();
void gotomain(int type);
void gotomain(bool ishistory, int type);
int choose();
void history();
void choosemode();
void choosehard(int type);
void tapto(int type);
mapnode findmap(int x, int y);
mapnode findmapxy(int x, int y);
int checkmap(int x, int y);
long long evaluate(int player);
long long evaluatexy(int x, int y, int player);
long long evaluatexy_defend(int x, int y, int player);
void alphabeta(treenode* root, int depth);


int datamap[20][20] = { 0 };
int pplayer1, pplayer2, eplayer1, eplayer2;
int ehard1, ehard2;
int aiman, tureman;
int nodenum;
treenode* tmpnode;
treenode* rootnode;
ofstream fout;
ifstream fin;
json status;
json save[100];

int main()
{
	initgraph(windows_width, windows_height);
	while (1)
	{
		init();
		choosemode();
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
	nodenum = status["nodenum"];

	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 72;
	_tcscpy_s(f.lfFaceName, _T("黑体"));
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	settextcolor(BLACK);

	cleardevice();
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
		pplayer1 = choose();
		eplayer1 = pplayer1 == 1 ? 2 : 1;
		gotomain(1);
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

	setlinecolor(BLACK);
	circle(168, 418, 78.5);
	setfillcolor(BLACK);
	fillcircle(544, 418, 78.5);

	outtextxy(168, 149, _T("选择你的棋子"));

	while (1)
	{
		ExMessage msg = getmessage();
		if (msg.message == WM_LBUTTONDOWN)//白棋是1 黑琪是2
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
	if (type == 1)
	{
		//画图
		IMAGE bkimg;
		loadimage(&bkimg, L"./imgs/choosen.png", 768, 1024);
		putimage(0, 0, &bkimg);

		//选择难度
		while (1)
		{
			ExMessage msg = getmessage();
			if (msg.message == WM_LBUTTONDOWN)
			{
				for (int i = 0; i < 4; ++i)
				{
					if (msg.x >= 384 && msg.x <= 384 + 102 && msg.y >= 615 + 99 * i && msg.y <= 615 + 99 * i + 72)
					{
						ehard1 = status["hards"][i];
						return;
					}
				}
			}
		}
	}
	return;
}

void gotomain(int type)
{
	memset(datamap, 0, sizeof(datamap));
	if (type == 0)
	{
		//初始化gui
		IMAGE bkimg;
		loadimage(&bkimg, L"./imgs/main pvpve.png", 768, 1024);
		putimage(0, 0, &bkimg);

		//json初始化
		status["sum"] = status["sum"] + 1;
		int thistimenum = status["sum"];
		save[thistimenum]["type"] = 0;
		save[thistimenum]["player1"] = pplayer1;
		save[thistimenum]["player2"] = pplayer2;
		save[thistimenum]["stepcnt"] = 0;
		save[thistimenum]["steps"] = {};
		vector<int> steps;
		time_t cotime = time(0);
		time_t pre = time(0);
		time_t now = time(0);
		int noplayer = 2;
		setfillcolor(BLACK);
		fillcircle(310 + 73, 838 + 73, 73);
		save[thistimenum]["starttime"] = cotime;

		//开始下棋
		while (1)
		{
			//鼠标控制下棋点
			ExMessage msg = getmessage(EX_MOUSE);
			if (msg.message != WM_LBUTTONDOWN) continue;
			else
			{
				//得到落子点
				mapnode numnode = findmap(msg.x, msg.y);
				mapnode xynode = findmapxy(msg.x, msg.y);
				if (numnode.x == -1 || numnode.y == -1) continue;
				if (datamap[numnode.x][numnode.y] != 0) continue;

				//存档
				steps.push_back(noplayer);
				pre = now;
				now = time(0);
				steps.push_back(floor(difftime(now, pre)));
				steps.push_back(numnode.x);
				steps.push_back(numnode.y);
				save[thistimenum]["stepcnt"] = save[thistimenum]["stepcnt"] + 1;

				//更新时间
				clearrectangle(512 + 10, 911 + 5, 512 + 233 - 15, 911 + 60 - 10);
				int dif = floor(difftime(now, cotime));
				LOGFONT f;
				gettextstyle(&f);
				f.lfHeight = 48;
				_tcscpy_s(f.lfFaceName, _T("黑体"));
				f.lfQuality = ANTIALIASED_QUALITY;
				settextstyle(&f);
				settextcolor(BLACK);
				TCHAR s[20];
				swprintf_s(s, _T("%d"), dif);
				outtextxy(512 + 20, 911 + 8, s);

				datamap[numnode.x][numnode.y] = noplayer;
				swprintf_s(s, _T("%lld"), evaluate(noplayer));
				clearrectangle(45, 838 + 145, 45 + 219, 838 + 145 + 41);
				outtextxy(45, 838 + 145, s);

				//画图
				if (noplayer == 1)
				{
					setfillcolor(WHITE);
					fillcircle(xynode.x, xynode.y, chessradius);
					noplayer = 2;
					setfillcolor(BLACK);
					fillcircle(310 + 73, 838 + 73, 73);
				}
				else
				{
					setfillcolor(BLACK);
					fillcircle(xynode.x, xynode.y, chessradius);
					noplayer = 1;
					setfillcolor(WHITE);
					fillcircle(310 + 73, 838 + 73, 73);
				}

				//判断胜负
				int winner = checkmap(numnode.x, numnode.y);
				if (winner != 0)
				{
					//画图
					cleardevice();
					if (winner == 1)
					{
						outtextxy(168, 149, _T("白棋获胜！"));
					}
					else
						outtextxy(168, 149, _T("黑棋获胜！"));

					//保存存档
					save[thistimenum]["steps"] = steps;
					string name = "./saves/对局";
					int sum = status["sum"];
					name += to_string(sum);
					name += ".json";
					fout.open(name);
					fout << save[thistimenum];
					fout.close();
					fout.open("./saves/status.json");
					fout << status;
					fout.close();
					Sleep(1000);
					save[thistimenum]["endtime"] = time(0);

					return;
				}
				else
					continue;
			}
		}
	}
	else if (type == 1)
	{
		cleardevice();
		//初始化gui
		IMAGE bkimg;
		loadimage(&bkimg, L"./imgs/main pvpve.png", 768, 1024);
		putimage(0, 0, &bkimg);

		//json初始化
		status["sum"] = status["sum"] + 1;
		int thistimenum = status["sum"];
		save[thistimenum]["type"] = 0;
		save[thistimenum]["player1"] = pplayer1;
		save[thistimenum]["player2"] = pplayer2;
		save[thistimenum]["stepcnt"] = 0;
		save[thistimenum]["steps"] = {};
		vector<int> steps;
		time_t cotime = time(0);
		time_t pre = time(0);
		time_t now = time(0);
		int noplayer = 2;
		setfillcolor(BLACK);
		fillcircle(310 + 73, 838 + 73, 73);
		save[thistimenum]["starttime"] = cotime;

		aiman = pplayer1;
		tureman = eplayer1;

		//开始下棋
		while (1)
		{
			if (noplayer == pplayer1)
			{
				while (1)
				{
					//鼠标控制下棋点
					ExMessage msg = getmessage(EX_MOUSE);
					if (msg.message != WM_LBUTTONDOWN) continue;
					else
					{
						//得到落子点
						mapnode numnode = findmap(msg.x, msg.y);
						mapnode xynode = findmapxy(msg.x, msg.y);
						if (numnode.x == -1 || numnode.y == -1) continue;
						if (datamap[numnode.x][numnode.y] != 0) continue;

						//存档
						steps.push_back(noplayer);
						pre = now;
						now = time(0);
						steps.push_back(floor(difftime(now, pre)));
						steps.push_back(numnode.x);
						steps.push_back(numnode.y);
						save[thistimenum]["stepcnt"] = save[thistimenum]["stepcnt"] + 1;

						//更新时间
						clearrectangle(512 + 10, 911 + 5, 512 + 233 - 15, 911 + 60 - 10);
						int dif = floor(difftime(now, cotime));
						LOGFONT f;
						gettextstyle(&f);
						f.lfHeight = 48;
						_tcscpy_s(f.lfFaceName, _T("黑体"));
						f.lfQuality = ANTIALIASED_QUALITY;
						settextstyle(&f);
						settextcolor(BLACK);
						TCHAR s[20];
						swprintf_s(s, _T("%d"), dif);
						outtextxy(512 + 20, 911 + 8, s);
						datamap[numnode.x][numnode.y] = noplayer;
						swprintf_s(s, _T("%lld"), evaluate(noplayer));
						clearrectangle(45, 838 + 145, 45 + 219, 838 + 145 + 41);
						outtextxy(45, 838 + 145, s);


						//画图
						if (noplayer == 1)
						{
							setfillcolor(WHITE);
							fillcircle(xynode.x, xynode.y, chessradius);
							noplayer = 2;
							setfillcolor(BLACK);
							fillcircle(310 + 73, 838 + 73, 73);
						}
						else
						{
							setfillcolor(BLACK);
							fillcircle(xynode.x, xynode.y, chessradius);
							noplayer = 1;
							setfillcolor(WHITE);
							fillcircle(310 + 73, 838 + 73, 73);
						}

						//判断胜负
						int winner = checkmap(numnode.x, numnode.y);
						if (winner != 0)
						{
							//画图
							cleardevice();
							if (winner == 1)
							{
								outtextxy(168, 149, _T("白棋获胜！"));
							}
							else
								outtextxy(168, 149, _T("黑棋获胜！"));

							//保存存档
							save[thistimenum]["steps"] = steps;
							string name = "./saves/对局";
							int sum = status["sum"];
							name += to_string(sum);
							name += ".json";
							fout.open(name);
							fout << save[thistimenum];
							fout.close();
							fout.open("./saves/status.json");
							fout << status;
							fout.close();
							Sleep(1000);
							save[thistimenum]["endtime"] = time(0);

							return;
						}
						else
							break;
					}
				}
			}
			else
				//aigo
			{
				//得到落子点
				aiman = noplayer;
				tureman = aiman == 1 ? 2 : 1;
				rootnode = new treenode;
				rootnode->player = noplayer;
				rootnode->val = 0;
				rootnode->x = -1;
				rootnode->y = -1;
				alphabeta(rootnode, 0);
				mapnode numnode;
				numnode.x = rootnode->x;
				numnode.y = rootnode->y;
				delete rootnode;
				mapnode xynode;
				xynode.x = zero.x + numnode.x * chessradius * 2;
				xynode.y = zero.y + numnode.y * chessradius * 2;
				if (numnode.x == -1 || numnode.y == -1) continue;
				if (datamap[numnode.x][numnode.y] != 0) continue;

				//存档
				steps.push_back(noplayer);
				pre = now;
				now = time(0);
				steps.push_back(floor(difftime(now, pre)));
				steps.push_back(numnode.x);
				steps.push_back(numnode.y);
				save[thistimenum]["stepcnt"] = save[thistimenum]["stepcnt"] + 1;

				//更新时间
				clearrectangle(512 + 10, 911 + 5, 512 + 233 - 15, 911 + 60 - 10);
				int dif = floor(difftime(now, cotime));
				LOGFONT f;
				gettextstyle(&f);
				f.lfHeight = 48;
				_tcscpy_s(f.lfFaceName, _T("黑体"));
				f.lfQuality = ANTIALIASED_QUALITY;
				settextstyle(&f);
				settextcolor(BLACK);
				TCHAR s[20];
				swprintf_s(s, _T("%d"), dif);
				outtextxy(512 + 20, 911 + 8, s);
				datamap[numnode.x][numnode.y] = noplayer;
				swprintf_s(s, _T("%lld"), evaluate(noplayer));
				clearrectangle(45, 838 + 145, 45 + 219, 838 + 145 + 41);
				outtextxy(45, 838 + 145, s);


				//画图
				if (noplayer == 1)
				{
					setfillcolor(WHITE);
					fillcircle(xynode.x, xynode.y, chessradius);
					noplayer = 2;
					setfillcolor(BLACK);
					fillcircle(310 + 73, 838 + 73, 73);
				}
				else
				{
					setfillcolor(BLACK);
					fillcircle(xynode.x, xynode.y, chessradius);
					noplayer = 1;
					setfillcolor(WHITE);
					fillcircle(310 + 73, 838 + 73, 73);
				}

				//判断胜负
				int winner = checkmap(numnode.x, numnode.y);
				if (winner != 0)
				{
					//画图
					cleardevice();
					if (winner == 1)
					{
						outtextxy(168, 149, _T("白棋获胜！"));
					}
					else
						outtextxy(168, 149, _T("黑棋获胜！"));

					//保存存档
					save[thistimenum]["steps"] = steps;
					string name = "./saves/对局";
					int sum = status["sum"];
					name += to_string(sum);
					name += ".json";
					fout.open(name);
					fout << save[thistimenum];
					fout.close();
					fout.open("./saves/status.json");
					fout << status;
					fout.close();
					Sleep(1000);
					save[thistimenum]["endtime"] = time(0);

					return;
				}
			}
		}
	}
	return;
}
void gotomain(bool ishistory, int type)
{
	return;
}

mapnode findmap(int x, int y)
{
	mapnode ans;
	ans.x = -1;
	ans.x = -1;
	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			if (x >= zero.x - chessradius + i * chessradius * 2 && x <= zero.x - chessradius + (i + 1) * chessradius * 2 && y > zero.y - chessradius + j * chessradius * 2 && y < zero.y - chessradius + (j + 1) * chessradius * 2)
			{
				ans.x = i;
				ans.y = j;
				return ans;
			}
		}
	}
	return ans;
}
mapnode findmapxy(int x, int y)
{
	mapnode ans;
	ans.x = -1;
	ans.x = -1;
	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			if (x >= zero.x - chessradius + i * chessradius * 2 && x <= zero.x - chessradius + (i + 1) * chessradius * 2 && y > zero.y - chessradius + j * chessradius * 2 && y < zero.y - chessradius + (j + 1) * chessradius * 2)
			{
				ans.x = zero.x + i * chessradius * 2;
				ans.y = zero.y + j * chessradius * 2;
				return ans;
			}
		}
	}
	return ans;
}

int checkmap(int x, int y)
{
	int noplayer = datamap[x][y];
	int turnx[4] = { 1,0,1,1 };
	int turny[4] = { 0,1,1,-1 };

	for (int i = 0; i < 4; ++i)
	{
		int tx = x;
		int ty = y;

		while (tx + turnx[i] >= 0 && ty + turny[i] >= 0 && datamap[tx + turnx[i]][ty + turny[i]] == noplayer)
		{
			tx += turnx[i];
			ty += turny[i];
		}
		int cnt = 1;
		while (tx - turnx[i] >= 0 && ty - turny[i] >= 0 && datamap[tx - turnx[i]][ty - turny[i]] == noplayer)
		{
			tx -= turnx[i];
			ty -= turny[i];
			++cnt;
		}
		if (cnt >= 5) return noplayer;
	}

	return 0;
}

long long evaluate(int player)
{
	bool checkmap[4][20][20];
	memset(checkmap, 0, sizeof(checkmap));
	int turnx[4] = { 1,0,1,1 };
	int turny[4] = { 0,1,1,-1 };
	long long score_player = 0;
	long long score_opponent = 0;
	int otherplayer = player == 1 ? 2 : 1;

	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			if (datamap[i][j] == 0) continue;
			if (datamap[i][j] == player)
			{
				for (int ti = 0; ti < 4; ++ti)
				{
					int tx = i;
					int ty = j;

					if (checkmap[ti][i][j] == 1) continue;

					bool ishuoleft = true;
					bool ishuoright = true;

					while (tx + turnx[ti] >= 0 && ty + turny[ti] >= 0 && datamap[tx + turnx[ti]][ty + turny[ti]] == player)
					{
						tx += turnx[ti];
						ty += turny[ti];
						checkmap[ti][tx][ty] = 1;
					}
					if (tx + turnx[ti] >= 0 && ty + turny[ti] >= 0 && datamap[tx + turnx[ti]][ty + turny[ti]] == otherplayer)
						ishuoright = false;

					int cnt = 1;
					while (tx - turnx[ti] >= 0 && ty - turny[ti] >= 0 && datamap[tx - turnx[ti]][ty - turny[ti]] == player)
					{
						tx -= turnx[ti];
						ty -= turny[ti];
						checkmap[ti][tx][ty] = 1;
						++cnt;
					}
					if (tx - turnx[ti] >= 0 && ty - turny[ti] >= 0 && datamap[tx - turnx[ti]][ty - turny[ti]] == otherplayer)
						ishuoleft = false;
					int ishuo = ishuoleft && ishuoright;
					ishuo = ishuo ? 10 : 1;

					if (cnt == 5) score_player += 1000000;
					else if (cnt == 4) score_player += 1000 * ishuo;
					else if (cnt == 3) score_player += 100 * ishuo;
					else if (cnt == 2) score_player += 10 * ishuo;
					else if (cnt == 1) score_player += 1 * ishuo;
				}
			}
			if (datamap[i][j] == otherplayer)
			{
				for (int ti = 0; ti < 4; ++ti)
				{
					int tx = i;
					int ty = j;

					if (checkmap[ti][i][j] == 1) continue;

					bool ishuoleft = true;
					bool ishuoright = true;

					while (tx + turnx[ti] >= 0 && ty + turny[ti] >= 0 && datamap[tx + turnx[ti]][ty + turny[ti]] == otherplayer)
					{
						tx += turnx[ti];
						ty += turny[ti];
						checkmap[ti][tx][ty] = 1;
					}
					if (tx + turnx[ti] >= 0 && ty + turny[ti] >= 0 && datamap[tx + turnx[ti]][ty + turny[ti]] == player)
						ishuoright = false;

					int cnt = 1;
					while (tx - turnx[ti] >= 0 && ty - turny[ti] >= 0 && datamap[tx - turnx[ti]][ty - turny[ti]] == otherplayer)
					{
						tx -= turnx[ti];
						ty -= turny[ti];
						checkmap[ti][tx][ty] = 1;
						++cnt;
					}
					if (tx - turnx[ti] >= 0 && ty - turny[ti] >= 0 && datamap[tx - turnx[ti]][ty - turny[ti]] == player)
						ishuoleft = false;
					int ishuo = ishuoleft && ishuoright;
					ishuo = ishuo ? 10 : 1;

					if (cnt == 5) score_opponent += 1000000;
					else if (cnt == 4) score_opponent += 1000 * ishuo;
					else if (cnt == 3) score_opponent += 100 * ishuo;
					else if (cnt == 2) score_opponent += 10 * ishuo;
					else if (cnt == 1) score_opponent += 1 * ishuo;
				}
			}
		}
	}

	return score_player - score_opponent;
}

long long evaluatexy(int x, int y, int player)
{
	int otherplayer = player == 1 ? 2 : 1;
	int turnx[4] = { 1,0,1,1 };
	int turny[4] = { 0,1,1,-1 };
	long long ans = 0;
	datamap[x][y] = player;

	//四个方向评分
	for (int i = 0; i < 4; ++i)
	{
		int tx = x;
		int ty = y;

		bool ishuoleft = true;
		bool ishuoright = true;
		while (tx + turnx[i] >= 0 && ty + turny[i] >= 0 && datamap[tx + turnx[i]][ty + turny[i]] == player)
		{
			tx += turnx[i];
			ty += turny[i];
		}
		if (tx + turnx[i] >= 0 && ty + turny[i] >= 0 && datamap[tx + turnx[i]][ty + turny[i]] == otherplayer)
			ishuoright = false;

		int cnt = 1;
		while (tx - turnx[i] >= 0 && ty - turny[i] >= 0 && datamap[tx - turnx[i]][ty - turny[i]] == player)
		{
			tx -= turnx[i];
			ty -= turny[i];
			++cnt;
		}
		if (tx - turnx[i] >= 0 && ty - turny[i] >= 0 && datamap[tx - turnx[i]][ty - turny[i]] == otherplayer)
			ishuoleft = false;
		int ishuo = ishuoleft && ishuoright;
		ishuo = ishuo ? 10 : 1;

		if (cnt == 5) ans += 100000000;
		else if (cnt == 4) ans += 100000 * ishuo;
		else if (cnt == 3) ans += 10000 * ishuo;
		else if (cnt == 2) ans += 1000 * ishuo;
		else if (cnt == 1) ans += 10 * ishuo;
	}

	//位置加分
	ans += 100 - (x - 7) * (x - 7) - (y - 7) * (y - 7);
	datamap[x][y] = 0;

	return ans;
}

long long evaluatexy_defend(int x, int y, int player)
{
	long long ans = 0;
	int otherplayer = player == 1 ? 2 : 1;
	int turnx[8] = { 1,0,-1,0,1,-1,1,-1 };
	int turny[8] = { 0,1,0,-1,1,-1,-1,1 };
	datamap[x][y] = player;

	//八个方向查找
	for (int i = 0; i < 8; ++i)
	{
		int ishuo = 1;
		int tx = x;
		int ty = y;
		if ((tx + turnx[i] < 0 || ty + turny[i] < 0) || datamap[tx + turnx[i]][ty + turny[i]] != otherplayer) continue;
		while (tx + turnx[i] >= 0 && ty + turny[i] >= 0 && datamap[tx + turnx[i]][ty + turny[i]] == otherplayer)
		{
			tx += turnx[i];
			ty += turny[i];
		}
		if ((tx + turnx[i] < 0 || ty + turny[i] < 0) || datamap[tx + turnx[i]][ty + turny[i]] == player) ishuo = 0;
		int cnt = 1;
		while (tx - turnx[i] >= 0 && ty - turny[i] >= 0 && datamap[tx - turnx[i]][ty - turny[i]] == otherplayer)
		{
			tx -= turnx[i];
			ty -= turny[i];
			++cnt;
		}
		ishuo = ishuo ? 20 : 1;
		if (cnt == 4) ans += 1000000 * ishuo;
		else if (cnt == 3) ans += 10000 * ishuo;
		else if (cnt == 2) ans += 100 * ishuo;
		else if (cnt == 1) ans += 1 * ishuo;
	}
	datamap[x][y] = 0;

	return ans;
}

void alphabeta(treenode* root, int depth)
{
	if (depth != 0) datamap[root->x][root->y] = root->player;
	if (depth == ehard1)
	{
		root->val = evaluate(root->player);
		datamap[root->x][root->y] = 0;
		return;
	}
	//生成子节点
	for (int i = 0; i < 15; ++i)
	{
		for (int j = 0; j < 15; ++j)
		{
			if (datamap[i][j] != 0)continue;
			tmpnode = new treenode;
			tmpnode->x = i;
			tmpnode->y = j;
			tmpnode->player = depth % 2 == 0 ? aiman : tureman;
			tmpnode->val = evaluatexy(i, j, tmpnode->player);
			tmpnode->val += evaluatexy_defend(i, j, tmpnode->player);
			root->child.push_back(tmpnode);
		}
	}

	//只保留前八个子节点
	sort(root->child.begin(), root->child.end());
	for (int i = root->child.size() - 1; i >= nodenum; --i)
	{
		delete root->child[i];
		root->child.pop_back();
	}

	//遍历子节点
	for (int i = 0; i < root->child.size(); ++i)
	{
		root->child[i]->alpha = root->alpha;
		root->child[i]->beta = root->beta;
		alphabeta(root->child[i], depth + 1);

		if (depth % 2 == 1)//MAX层更新上一个MIN层的上界beta
		{
			if (root->child[i]->val < root->beta) {
				root->beta = root->child[i]->val;
				root->val = root->child[i]->val;
			}
		}
		else if (depth % 2 == 0)//MIN层更新上一个MAX层的下界alpha
		{
			if (root->child[i]->val > root->alpha)
			{
				if (depth == 0)
				{
					root->x = root->child[i]->x, root->y = root->child[i]->y;
				}
				root->alpha = root->child[i]->val;
				root->val = root->child[i]->val;
			}
		}
		if (root->alpha > root->beta)
		{
			break;
		}
	}
	for (int i = 0; i < root->child.size(); ++i)
	{
		delete root->child[i];
	}
	while (root->child.empty() != true) root->child.pop_back();
	datamap[root->x][root->y] = 0;
	return;
}
