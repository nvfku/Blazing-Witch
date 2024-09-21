#pragma once
//#define _CRT_SECURE_NO_WARNINGS 1

#undef UNICODE
#undef _UNICODE
#pragma comment(lib,"Winmm.lib")
#pragma comment( lib, "MSIMG32.LIB")
#include<stdio.h>
#include<stdlib.h>
#include<math.h>
#include<time.h>
#include<conio.h>
#include<graphics.h>
#include<Windows.h>
#include<mmsystem.h>
#include<iostream>
#include<string>
#include<algorithm>
#include<vector>
#include<queue>
using namespace std;

#ifndef MATH_H
#define MATH_H
	double dist(double x1, double y1, double x2, double y2);
	int mh_dist(int x1, int y1, int x2, int y2);
#endif
	void putpng(IMAGE* dstimg, int x, int y, IMAGE* srcimg);
	int game_pause();
	void Set_player1();
	void Set_player0();
	void map1_pre_dfs();

	void NPC_Touch(int kind, int i);

	int realx(int x);
	int realy(int y);
	int mapx(int x);
	int mapy(int y);


#define FF FlushBatchDraw();
//暂定名：blazing witch闪红的魔女
