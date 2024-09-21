//说明：基于dfs（主路扭曲法）随机生成迷宫地图
#include"Head.h"
#include"Struct.h"
#include"Setting.h"		//BUG:不知道为什么调用这个头文件，dfs函数的ad[][]就溢出
#include"Text.h"
// ===========Maze_generate专用全局变量========
int position_x = 2, position_y = 2;//显示当前角色走到的位置
int is_gameover;

int readtime;
IMAGE testimg;
//=================================================
void gameover();

void Astar()
{
	while (!open.empty())
	{
		NODE from = open.top();
		printf("现在位于 (%d,%d) \n", from.x, from.y);
		open.pop();
		n_map[from.x][from.y].is_close = true;
		if (n_map[endX][endY].is_close)
		{
			int nx = endX, ny = endY;
			while (n_map[nx][ny].type != Start)
			{
				NODE* x = n_map[nx][ny].father;
				nx = x->x;
				ny = x->y;
				if (n_map[nx][ny].type != Start)	map[nx][ny] = 6;
			}
			break;
		}
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (j == 0 && i == 0)continue; 
				if (abs(i) + abs(j) == 2) continue;//限定四个方向
				printf("i=%d, j=%d\n", i, j);
				int tx = from.x + i, ty = from.y + j;
				if (tx <1 || ty <1 || tx > Wn || ty > Hn)continue;
				if (n_map[tx][ty].type == Wall)continue;
				if (n_map[tx][ty].is_close || n_map[tx][ty].is_open) continue;

				cout << "Sucess" << endl;
			
				NODE& u = n_map[tx][ty];
				u.father = &(n_map[from.x][from.y]);

				if (i && j)		u.g = n_map[from.x][from.y].g + 14; 
				else 	u.g = n_map[from.x][from.y].g + 10;	
				u.h = mh_dist(u.x, u.y, n_map[endX][endY].x, n_map[endX][endY].y);
				u.f = u.g + u.f;

				
				u.is_open = true;
				open.push(n_map[tx][ty]);
			}
	}
}
void creat_n_map() 
{
	for (int i = 1; i <= Wn; i++)
		for (int j = 1; j <= Hn; j++)
		{
			n_map[i][j].x = i;
			n_map[i][j].y = j;
			n_map[i][j].type = n_map[i][j].backmark=map[i][j];
			n_map[i][j].f = n_map[i][j].g = n_map[i][j].h = 0;
			n_map[i][j].is_open = n_map[i][j].is_close = false;
			if (map[i][j] == Start) 
			{
				n_map[i][j].is_open = true;
				open.push(n_map[i][j]);
			}
			/*if (map[i][j] == end)
			{
				endx = i;
				endy = j;
			}*/
		}

}

void dfs(int x, int y)
{
	vis[x][y] = 0;

	//cout << "x=" << x << "  y=" << y << "(map[x][y]=" << map[x][y] << endl;

	if (map[x][y] == 3)
	{
		cout << "找到终点" << endl;
		return;
	}
	srand(time(0) % 1926 + x - y + position_x * position_y);//对于不同x、y引入不同seed，使每次产生不同的随机数以产生随机dfs方向
	//cout << rand() << endl;

	if (rand() % 2 == 0)
		for (int i = 1; i <= 4; i++)
		{
			int xx = x + d[i][1], yy = y + d[i][2];
			//cout << "xx=" << xx << "  yy=" << yy << endl;

			if (xx > Wn || xx<1 || yy>Hn || yy < 1) continue;
			if (!vis[xx][yy]) continue;
			dfs(xx, yy);

			map[x + ad[i][1]][y + ad[i][2]] = 1;

		}
	else
		for (int i = 4; i >= 1; i--)
		{
			int xx = x + d[i][1], yy = y + d[i][2];
			//cout << "xx=" << xx << "  yy=" << yy << endl;

			if (xx > Wn || xx<1 || yy>Hn || yy < 1) continue;
			if (!vis[xx][yy]) continue;
			dfs(xx, yy);

			map[x + ad[i][1]][y + ad[i][2]] = 1;

		}


}

void KeyControl()
{
	int readendtime = clock();
	if (readendtime - readtime < 100) return;
	readtime = readendtime;
	if (position_x<1 || position_x>Wn || position_y<1 || position_x>Hn) return;
	//==================角色移动=======================
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(87))
	{
		if (!map[position_x][position_y - 1]) return;
		cout << "wwwww" << endl;
		position_y--;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(65))
	{
		if (!map[position_x - 1][position_y]) return;
		position_x--;

	}
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(83))
	{
		if (!map[position_x][position_y + 1]) return;
		position_y++;
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(68))
	{
		if (!map[position_x + 1][position_y]) return;
		position_x++;
	}
}
//=====预处理======
void pre()
{
	initgraph(960, 640, EX_SHOWCONSOLE);
	setbkcolor(BLUE);
	cleardevice();
	BeginBatchDraw();
	/*for(int y=0; y<480; y++)
{
   setcolor(RGB(100,100,y/3+20));
   line(0, y, 640, y);
   line(9999, 0, -9999, 0);
line(0, 99999, 0, -99990);
}*/

	//以下为dfs随机生成图
	d[1][1] = 2, d[2][1] = -2, d[3][2] = 2, d[4][2] = -2;
	ad[1][1] = 1, ad[2][1] = -1, ad[3][2] = 1, ad[4][2] = -1;
	for (int i = 1; i <= Wn; i++)
		for (int j = 1; j <= Hn; j++)
		{
			vis[i][j] = 1;
			if(i%2==0&&j%2==0) map[i][j] = 1;
		}
	map[2][2] = 2;//设置起点和终点，上面的会覆盖掉
	map[endX][endY] = 3;

	dfs(2, 2);

	loadimage(&testimg, "./data/img/text透明.png", 50, 50);
}
void putpng(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //新版png
{
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}
void drawpng(PLAYER sb, IMAGE sm)//直接在PLAYER的位置显示png图像
{
	putpng(NULL, sb.x, sb.y, &sm);
}

//-------绘制函数------
void draw_map()
{
	for (int x = 1; x <= Wn; x++)
	{
		for (int y = 1; y <= Hn; y++)
		{
			//绘制边框
			setlinecolor(WHITE);
			rectangle(cellW * y + OriginX, cellW * x + OriginY, cellW * (y + 1) + OriginX, cellW * (x + 1) + OriginY);

			if (x == position_x && y == position_y)//绘制角色点
			{
				setfillcolor(YELLOW);
				fillrectangle(cellW * x + OriginX, cellW * y + OriginY, cellW * (x + 1) + OriginX, cellW * (y + 1) + OriginY);
				putpng(NULL, cellW * x + OriginX, cellW * y + OriginY, &testimg);
			}
			else if (map[x][y] == 0)//绘制障碍物，红色
			{
				setfillcolor(RED);
				fillrectangle(cellW * x + OriginX, cellW * y + OriginY, cellW * (x + 1) + OriginX, cellW * (y + 1) + OriginY);
				putimage(cellW * x + OriginX, cellW * y + OriginY, &testimg);
			}
			else if (map[x][y] == 3)//绘制结束点，一个叉
			{
				line(cellW * x + OriginX, cellW * y + OriginY, cellW * (x + 1) + OriginX, cellW * (y + 1) + OriginY);
				line(cellW * (x + 1) + OriginX, cellW * y + OriginY, cellW * x + OriginX, cellW * (y + 1) + OriginY);
			}
			else if (map[x][y] == 2)//绘制起始点，一个圈
			{
				circle(cellW * x + OriginX + cellW / 2, cellW * y + OriginY + cellW / 2, cellW / 3);
			}
		}
	}
}


int main()
{
	// 数据初始化
	pre();	

	for (int j = 1; j <= Wn; j++)
	{
		for (int i = 1; i <= Hn; i++)
			cout << map[i][j] << " ";
		cout << endl;
	}

	readtime = clock();
	while (!is_gameover)  //  游戏循环执行
	{
		draw_map();
		/*	for (int j = 1; j <= Wn; j++)
			{
				for (int i = 1; i <= Hn; i++)
					cout << map[i][j] << " ";
				cout << endl;
			}*/
			//游戏内侧栏信息显示
		settextstyle(24, 0, ("隶书"));
		settextcolor(YELLOW);
		outtextxy(650, 440, ("asdw移动"));
		outtextxy(650, 490, ("esc变换地图"));
		outtextxy(650, 540, ("空格显示提示(最多两次)"));
		FlushBatchDraw();
		//===================

		if (position_x == (Wn - 1) && position_y == (Hn - 1))
			is_gameover = 1;//到达终点判定游戏结束

		KeyControl();

	//	cout << position_x << "  ,   " << position_y << endl;
		creat_n_map();
		Astar();
		for (int j = 1; j <= Wn; j++)
		{
			for (int i = 1; i <= Hn; i++)
				cout << map[i][j] << " ";
			cout << endl;
		}
		cout << endl;
		for (int j = 1; j <= Wn; j++)
		{
			for (int i = 1; i <= Hn; i++)
				cout << n_map[i][j].backmark << " ";
			cout << endl;
		}


		Sleep(1000000);
	}
	gameover(); //游戏结束
	return 0;
}

void gameover()
{
	{
		setbkcolor(BLUE);
		cleardevice();
		settextstyle(64, 0, ("华文彩云"));
		settextcolor(YELLOW);
		outtextxy(120, 100, ("大吉大利，今晚吃鸡！"));

		EndBatchDraw();
		_getch();
		closegraph();
	}
}
