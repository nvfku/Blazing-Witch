#include"Head.h"
#include"Text.h"
#include"Struct.h"
#include"Setting.h"
bool timer(unsigned int duration, int id)//定时器，分别为延迟时间和编号
{
	static DWORD startTime[10000];
	int endTime = clock();
	if (endTime - startTime[id] > duration)
	{
		startTime[id] = endTime;
		return 1;
	}
	return 0;
}
/*	id
0~200为KeyControl()专用	//0用作全局判断按键

800~899为Enemy		//800~810为大地图动画绘制
900~999为npc	//900~910为大地图动画绘制,911~920为立绘绘制
>1000为玩家专用
2000+100用于backpage
3000+调试用
*/

NODE Astar(int starti,int startj,int endi,int endj,int set)
{
	while (!open.empty())
	{
		NODE from = open.top();
		open.pop();
		n_map[from.x][from.y].is_close = true;
		if (n_map[endi][endj].is_close)
		{
			NODE* p=NULL;
			if (set == 1)
			{
				p = n_map[endi][endj].father;
				return *p;
			}
			else if (set == 2)
			{
				while (n_map[endi][endj].type != Start)
				{
					p = n_map[endi][endj].father;
					endi = p->x;
					endj = p->y;
					if (n_map[endi][endj].type != Start)
					{
						n_map[endi][endj].backmark = 6;
					}

				}
				return *p;
			}
			
		}
		for (int i = -1; i <= 1; i++)
			for (int j = -1; j <= 1; j++)
			{
				if (j == 0 && i == 0)continue;
				if (abs(i) + abs(j) == 2) continue;//限定四个方向
				int ti = from.x + i, tj = from.y + j;
				//if (ti <1 || tj <1 || ti > Wn || tj > Hn)continue;
				if (n_map[ti][tj].type == Wall)continue;
				if (n_map[ti][tj].is_close || n_map[ti][tj].is_open) continue;
				NODE& u = n_map[ti][tj];
				u.father = &(n_map[from.x][from.y]);

				if (i && j)		u.g = n_map[from.x][from.y].g + 14;
				else 	u.g = n_map[from.x][from.y].g + 10;
				u.h = mh_dist(u.x, u.y, n_map[endi][endj].x, n_map[endi][endj].y);
				u.f = u.g + u.f;

				u.is_open = true;
				open.push(n_map[ti][tj]);
			}
	}
}
NODE creat_n_map(int starti, int startj, int endi, int endj, int set)//为A*算法初始化数组
{
	for (int i = 1; i <= Wn * 2; i++)//初始化n_map
		for (int j = 1; j <= Hn * 2; j++)
			n_map[i][j] = { 0,0,0,0,0,0,NULL,0,0 };
	for (int i = 1; i <= Wn*2; i++)
		for (int j = 1; j <= Hn*2; j++)
		{
			n_map[i][j].x = i;
			n_map[i][j].y = j;
			n_map[i][j].type = n_map[i][j].backmark = map[i][j];
			if (map[i][j] == 2)
				n_map[i][j].type = n_map[i][j].backmark = 1;
			else if (map[i][j] == 3)
				n_map[i][j].type = n_map[i][j].backmark = 1;
			n_map[i][j].f = n_map[i][j].g = n_map[i][j].h = 0;
			n_map[i][j].is_open = n_map[i][j].is_close = false;
		}
	n_map[starti][startj].type = n_map[starti][startj].backmark = Start;
	n_map[starti][startj].is_open = true;
	open.push(n_map[starti][startj]);

	n_map[endi][endj].type = n_map[endi][endj].backmark = End;

	return Astar(starti,startj,endi,endj,set);
}
//--------------------------------以下为PLAYER行为---------------------------------
bool Crash(int x, int y)//碰撞检测，传递接下来要走到的坐标
{
	y += 40,x+=10;//动画位移补偿
	int di=mapx(x), dj = mapy(y);
	if (mapon == 1)
	{
		if (!map[di][dj]) return 0;
		//接着写判定npc和enmey不能重合，最后所有满足才能return1
		return 1;
	}
	else if (mapon == 0)
	{
		
		cout << "判断点" << mapon << "," << map0[di][dj] << "=" << map0[di][dj] << endl;
		
		//接着写判定npc和enmey不能重合，最后所有满足才能return1
		if (map0[di][dj] == 1 || map0[di][dj] == 3|| map0[di][dj]==9|| map0[di][dj]==8) return 1;
		else return 0;
	}
	  
}
PLAYER KeyControl(PLAYER sb)//键盘控制
{
	if (!timer(15, 0)||mark_me_at||mark_me_df)	return sb;
	mark_me_move =0;
	int nowtime = time(NULL);
	//=================系统按键======================
	if (GetAsyncKeyState(27))//ESC暂停菜单27
	{
		mark_gamepause = 1;
		mark_mapclose = 1;
		//game_pause();
	}
	if (GetAsyncKeyState(9))//TAB背包
	{
		if (!timer(500, 9)) return sb;
		//Sys_map = nowtime;
		mark_bk = 1;
	}
	if (GetAsyncKeyState(77)&&mapon)//M地图
	{
		if (!timer(500,77)) return sb;
		Sys_map = nowtime;
		mark_map = 1;
	}
	//==================角色移动=======================
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(87))
	{
	//	cout << "现在位于" << "x=" << sb.x << ",y=" << sb.y << endl;
		if (!Crash(sb.x,sb.y- sb.v_y)) return sb;
		mark_me_move = 1;
		sb.direction = 1;
		sb.y -= sb.v_y;
	}
	if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(65))
	{
		if (!Crash(sb.x - sb.v_x, sb.y)) return sb;
		mark_me_move = 1;
		sb.direction = 4;
		sb.x -= sb.v_x;
	}
	
	if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(83))
	{
		if (!Crash(sb.x, sb.y + sb.v_y)) return sb;
		mark_me_move = 1;
		sb.direction = 3;
		sb.y += sb.v_y;
	}
	if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(68))
	{
		if (!Crash(sb.x + sb.v_x, sb.y)) return sb;
		mark_me_move = 1;
		sb.direction = 2;
		sb.x += sb.v_x;
	}
	//======================角色动作========================
	if (GetAsyncKeyState(69))//npc交互
	{
		//cout << "press E" << endl;
		for (int i = 1; i <= npc_n; i++)
		{
			
			if (dist(npc[i].x, npc[i].y, sb.x, sb.y) <= 60)
			{//改写成选择类型，并入NPC_Touch函数里
				NPC_Touch(npc[i].kind, i);
				//sb.nowhp += 10;
			}
		}
	}
	if (GetAsyncKeyState(74))//攻击
	{
		if (timer(800,74)) return sb;
		mark_me_at= 1;
		
		for (int i = 1; i <= enemy_n; i++)
		{
			if (enemy[i].nowhp <= 0)	continue;
			if (dist(enemy[i].x, enemy[i].y, sb.x, sb.y) <= 50)
			{
				mciSendString("play hit", NULL, 0, NULL);
				enemy[i].nowhp -= 10;
				//mark_at[i] = mark_at_ = 1;击中标记暂时没有用，用到时记得在输出击中提示那时间判断给一个else把标记记为0
				Sys_hit_starttime[i] = time(NULL);
			}
		}
	}
	if (GetAsyncKeyState(75))//防御K
	{
		if (timer(1000, 75)) return sb;
		mark_me_df = 1;

		for (int i = 1; i <= enemy_n; i++)
		{
			if (enemy[i].nowhp <= 0)	continue;
			if (dist(enemy[i].x, enemy[i].y, sb.x, sb.y) <= 50)
			{
				enemy[i].nowhp -= 10;
				//mark_at[i] = mark_at_ = 1;击中标记暂时没有用，用到时记得在输出击中提示那时间判断给一个else把标记记为0
				Sys_hit_starttime[i] = time(NULL);
			}
		}
	}
	return sb;
}
PLAYER Enemy_Move(PLAYER sb)//敌人通常移动、追逐触发函数
{
	if (mapx(sb.x)==mapx(me.x)&&mapy(sb.y)==mapy(me.y)) return sb;
	
	//if (dist(sb.x, sb.x,me.x,me.y) < 51) return sb;
	while (!open.empty()) open.pop();
	NODE move=creat_n_map(mapx(me.x),  mapy(me.y), mapx(sb.x),mapy(sb.y),1);

	if (move.x == mapx(sb.x))
	{
		if (move.y == mapy(sb.y) + 1)  sb.y += sb.v_y ;
		else sb.y -= sb.v_y;
	}
	else
	{
		if (move.x == mapx(sb.x) + 1)	sb.x += sb.v_x;
		else sb.x -= sb.v_x;
	}
	//sb.x += sb.v_x * 2;
	//sb.y += sb.v_y / 2;
	return sb;
}
PLAYER Enemy_At(PLAYER sb)//敌人攻击触发函数(近身类）
{
	for (int i = 1; i <= enemy_n; i++)
	{
		if (enemy[i].nowhp <= 0) continue;
		if (!enemy[i].kind)//加上其他攻击触发条件
		{
			if (time(NULL) - enemy_at_starttime[i] < 1) continue;
			enemy_at_starttime[i] = time(NULL);

			int tdis = dist(enemy[i].x, enemy[i].y, sb.x, sb.y);
			
			if (tdis <= 50)//判断进行攻击
			{
				mark_enemy_at[i] = 1;


				if (tdis <= 30)//判定攻击是否击中
				{
					//printf_s("enemy[i].x=%d        enemy[i].y=%d            sb.x=%d          sb.y=%d\n", enemy[i].x, enemy[i].y, sb.x, sb.y);
					sb.nowhp -= 10;
					//mark_enemy_at_ = mark_enemy_at[i] = 1;
					Sys_enemy_hit_starttime[i] = time(NULL);
				}
			}
		}
	}
	return sb;
}


//-----------------以下为NPC专区--------
void drawnpc3(int i)
{
	int k = 0;
	const char tes[100] = "aaaaaaa";
	while (1)
	{
		putpng(NULL, W - 600, 100, &npc_img2[i]);
		setlinecolor((BLACK));
		setlinestyle(PS_DASH);
		setfillcolor(RGB(182, 252, 254));
		//outtextxy()
		FlushBatchDraw();

		while(1)
		{
			if (timer(2000,911))
			{
				++k;
				fillrectangle(lux, luy, rdx, rdy);
				drawtext((npc3_talk[k]).c_str(), &npc_talk, DT_LEFT | DT_TOP|| DT_WORDBREAK);
				FlushBatchDraw();
			}
			if (k == 8) break;
		}
		break;
			//
		FlushBatchDraw();
	}
}
void NPC_Touch(int kind,int i)
{
	setorigin(0, 0);
	switch (kind)
	{
		case 3:
		{
			drawnpc3(i);
			break;
		}
	default:
		break;
	}
	//
	setlinestyle(PS_SOLID);
	setorigin(W / 2, H / 2);
}

//~~~~以下为游戏内窗口消息显示调用模板~~~~
void bar(int x,int y,int hp,int nowhp)//血条绘制函数
{
	double t = nowhp*1.0 / hp;
	//cout << t << endl;
	setlinecolor(BLACK);
	setfillcolor(RGB(255, 250, 157));
	fillrectangle(x, y, x + 100, y + 10);
	setfillcolor(RED);
	fillrectangle(x, y, x + t*100, y + 10);
}     
void State()//相对角色不动类，血量经验mp等侧栏以及系统文字信息提示栏
{
	bar(-W / 2 + cellW * 2, -H / 2 + cellW, me.hp, me.nowhp);
	putpng(NULL, -W / 2 + cellW, -H / 2 + cellW, &me_img_state);

}
void State_mes()//相对角色运动类，游戏内交互信息窗口展示
{
	int nowtime = time(NULL);
	setfillcolor(BLUE);
	
	

	settextcolor(RED);
	//setfillcolor(RED);
	sprintf_s(t_hp, "%d", me.nowhp);
	outtextxy(0, -100, t_hp);
	bar(0, -40, me.hp, me.nowhp);

	for (int i = 1; i <= enemy_n; i++)
	{
		if (enemy[i].nowhp <= 0)	continue;
		settextcolor(RED);
		sprintf_s(t_hp, "%d", enemy[i].nowhp);
		outtextxy(enemy[i].x - me.x, enemy[i].y - 120 - me.y, t_hp);
		bar(enemy[i].x - me.x, enemy[i].y - 70 - me.y, enemy[i].hp, enemy[i].nowhp);
		if (nowtime - Sys_hit_starttime[i] < 3)
			outtextxy(enemy[i].x - me.x, enemy[i].y - 150 - me.y, Sys_hit);
		if (nowtime - Sys_enemy_hit_starttime[i] < 3)
		{
			settextcolor(BLUE);
			outtextxy(enemy[i].x - me.x, enemy[i].y - 200 - me.y, Sys_enemy_hit);
		}
	}
}

//==============以下为绘制区=============
void draw_village()//20x20
{
	
	for (int i = 1; i <= Wn0; i++)
	{
		for (int j = 1; j <= Hn0; j++)
		{	
			if (map0[i][j] == 0)//绘制障碍物，红色
			{
				setfillcolor(RED);
				fillrectangle(map_lu_x - me.x, map_lu_y - me.y, map_rd_x - me.x, map_rd_y - me.y);
			}
			else if (map0[i][j] != 0)
			{
				putimage(map_lu_x - me.x, map_lu_y - me.y, &sth_img[map0[i][j]]);
			}
		}
	}
	//putpng(NULL, cellW * (Wn0 / 2) + OriginX, cellW * (Hn0 / 2) + OriginY, &bigtree_img);
}
void draw_maze()
{
	for (int i = 1; i <= Wn * 2; i++)
	{
		for (int j = 1; j <= Hn * 2; j++)
		{

			if (map[i][j] == 0)//绘制障碍物，红色
			{
				setfillcolor(RED);
				fillrectangle(map_lu_x - me.x, map_lu_y - me.y, map_rd_x - me.x, map_rd_y - me.y);
				putimage(map_lu_x - me.x, map_lu_y - me.y, &sth_img[1]);
			}

		}
	}
}
void draw_map()
{

	setorigin(0, 0);
	putpng(NULL,10,10,&map_img);
	setorigin(110, 120);
	setaspectratio(0.13, 0.13);//设置缩放比例，其中符号*/
	int loci = mapx(me.x), locj = mapy(me.y);
	for (int i = 1; i <= Wn * 2; i++)
		for (int j = 1; j <= Hn * 2; j++)
		{
			//	//绘制边框
			//setlinecolor(WHITE);
			//rectangle(map_lu_x , map_lu_y , map_rd_x , map_rd_y );
			if (i == loci && j == locj)
			{
				setfillcolor(RED);

				setlinecolor(BLACK);
				fillrectangle(map_lu_x, map_lu_y, map_rd_x, map_rd_y);
				
			}
			else if (map[i][j] == 0)//绘制障碍物
			{
				setlinecolor(BLACK);
				setfillcolor(YELLOW);
				fillrectangle(map_lu_x , map_lu_y , map_rd_x , map_rd_y );
			}
			else if (map[i][j] == 3)//绘制结束点，一个叉
			{
				setlinecolor(BLUE);
				line(map_lu_x, map_lu_y, map_rd_x, map_rd_y);
				line(map_ru_x, map_ru_y, map_ld_x, map_ld_y);
			}
			else if (map[i][j] == 2)//绘制起始点，一个圈
			{
				setlinecolor(WHITE);
				circle((map_lu_x + map_ru_x) / 2, (map_lu_y + map_ld_y) / 2, cellW / 3);
			}
			if (n_map[i][j].backmark == 6)
			{
				setlinecolor(GREEN);
				setfillcolor(GREEN);
				fillrectangle(map_lu_x, map_lu_y, map_rd_x, map_rd_y);
			}

		}
	setlinecolor(RED);
	if (me.direction == 1)
	{
		line((cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * locj + OriginX,
			(cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj - 1) + OriginX);

		line(cellW * loci + OriginX, (cellW * locj + OriginX + cellW * (locj - 1) + OriginX) / 2,
			(cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj - 1) + OriginX);

		line(cellW * (loci + 1) + OriginX, (cellW * locj + OriginX + cellW * (locj - 1) + OriginX) / 2,
			(cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj - 1) + OriginX);
	}
	else if (me.direction == 2)
	{
		line(cellW * (loci + 1) + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2,
			cellW * (loci + 2) + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2);

		line((cellW * (loci + 2) + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * locj + OriginX,
			cellW * (loci + 2) + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2);

		line((cellW * (loci + 2) + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj + 1) + OriginX,
			cellW * (loci + 2) + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2);
	}
	else if (me.direction == 3)
	{
		line((cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj + 1) + OriginX,
			(cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj + 2) + OriginX);

		line(cellW * loci + OriginX, (cellW * (locj + 2) + OriginX + cellW * (locj + 1) + OriginX) / 2,
			(cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj + 2) + OriginX);

		line(cellW * (loci + 1) + OriginX, (cellW * (locj + 2) + OriginX + cellW * (locj + 1) + OriginX) / 2,
			(cellW * loci + OriginX + cellW * (loci + 1) + OriginX) / 2, cellW * (locj + 2) + OriginX);
	}
	else
	{
		line(cellW * loci + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2,
			cellW * (loci - 1) + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2);

		line((cellW * (loci - 1) + OriginX + cellW * loci + OriginX) / 2, cellW * locj + OriginX,
			cellW * (loci - 1) + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2);

		line((cellW * (loci - 1) + OriginX + cellW * loci + OriginX) / 2, cellW * (locj + 1) + OriginX,
			cellW * (loci - 1) + OriginX, (cellW * locj + OriginX + cellW * (locj + 1) + OriginX) / 2);
	}
	setaspectratio(1, 1);
	setorigin(W / 2, H / 2);
}


PLAYER Backpack(PLAYER sb)//背包打开函数
{
	RECT bkshow = { 915 ,200,1150,640 };
	RECT bkname = { 900, 90, 1150, 190 };
	int ti = 1, tj = 1;
	//主键盘操作，逻辑仍为上下左右选择
	while (1)
	{
		printf("ti=%d,tj=%d\n", ti, tj);
		setorigin(0, 0);
		//setaspectratio(1, 1);
		putpng(NULL, 340, 68, &backpack_img);

		int p = 1,markp=0;
		setlinecolor(BLUE);
		for (int j = 1; j <= Hnbk; j++)
			for (int i = 1; i <= Wnbk; i++)
			{
				if(i==ti&&j==tj)
					fillrectangle(80 * i + 363, 80 * j + 75,
						80 * (i + 1) + 363, 80 * (j + 1) + 75);
				else
					rectangle(80 * i + 363, 80 * j + 75,
				80 * (i + 1) + 363, 80 * (j + 1) + 75);
				if (p < 14)
					while (1)
					{
						if (bk[p].num > 0)
						{
							putpng(NULL, 80 * i + 390, 80 * j + 100, &object_img[bk[p].seq]);
							if (i == ti && j == tj)
							{			
								markp = p;
								settextcolor(BLACK);
								drawtext((bk[p].name+string("\n数量：")+to_string(bk[p].num)).c_str(), &bkname, DT_LEFT | DT_WORDBREAK);
								settextcolor(BLUE);
								drawtext((bk[p].describe).c_str(), &bkshow,DT_LEFT|DT_WORDBREAK);

							}
							++p;
							break;
						}
						else
						{
							++p;
						}
						if (p > 13)break;
					}
			}


		if (GetAsyncKeyState(9) && timer(500, 9))
		{
			mark_bk = 0;
			return sb;
		}

		//物品选择
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(87))
		{
			if (tj-1>0&&timer(300,2087))
				tj -= 1;
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(65))
		{
			if (ti - 1 > 0 && timer(300, 2065))
				ti -= 1;
		}

		if (GetAsyncKeyState(VK_DOWN) || GetAsyncKeyState(83))
		{
			if (tj + 1 <= Hnbk && timer(300, 2083))
				tj += 1;
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(68))
		{
			if (ti +1 <= Wnbk && timer(300, 2068))
				ti += 1;
		}

		//使用
		p = markp;
		if (bk[p].seq != 1 || bk[p].seq != 2)
		if (GetAsyncKeyState(69)&&timer(800,2069))
		{
			//cout << bk[p].seq << "\n";
			//system("pause");
				sb.nowhp += bk[p].hp;
				sb.nowmp += bk[p].mp;
				if (sb.nowhp > sb.hp)	sb.nowhp = sb.hp;
				if (sb.nowmp > sb.mp)	sb.nowmp = sb.mp;
				sb.at += bk[p].at;
				sb.df += bk[p].df;
				sb.at += bk[p].at;
				sb.mdf += bk[p].mdf;
				bk[p].num--;
		}
		FlushBatchDraw();
		
	}
	return sb;
}
void Map()//地图打开函数
{
	//cleardevice();	
		NODE tt = creat_n_map(mapx(me.x), mapy(me.y), ENDI, ENDJ, 2);
		draw_map();

	while (1)
	{
		if (GetAsyncKeyState(77) && timer(500, 77))
		{
			mark_map = 0;
			
			return;
		}
		FlushBatchDraw();
	}
}
void load_map0()
{
	Set_player0();

	cleardevice();
	BeginBatchDraw();
	//Sleep(100000000);
	
	mciSendString("play village repeat", NULL, 0, NULL);
	while (1)
	{
			//================绘制区==========================
		setorigin(W / 2, H / 2);
		putimage(-me.x, -me.y, &background);

		//地图绘制
		draw_village();

		//PLAYER绘制
		if (mark_me_move)
		{
			if (timer(100, 10001))++me_move;
			if (me_move == 5)	me_move = 1;
			putpng(NULL, 0, 0, &me_img_run[me.direction][me_move]);

		}
		else	putpng(NULL, 0, 0, &me_img_stand[me.direction]);
		
		for (int i = 1; i <= npc_n; i++)
		{
			if (timer(300, 900+i)) ++npc_show[i];
			if (npc_show[i] == 5)	npc_show[i] = 1;
			putpng(NULL, npc[i].x - me.x, npc[i].y - me.y, &npc_img[i][npc_show[i]]);
		}
		//=========================控制区=====================
		me = KeyControl(me);

	//=================判断区====================
		if (mapx(me.x) == 17 && mapy(me.y) == 2)
		{
			mciSendString("close village", NULL, 0, NULL);
			EndBatchDraw();
			return;
		}
		FlushBatchDraw();
	}
	EndBatchDraw();

}
void load_map1()
{
	mciSendString("play battle repeat", NULL, 0, NULL);
	map1_pre_dfs();//初始化随机生成地图
	Set_player1();

	cleardevice();
	BeginBatchDraw();
	while (1)
	{
		//================绘制区==========================
		setorigin(W / 2, H / 2);
		putimage(-300-me.x, -300-me.y, &background);

		//地图绘制
		draw_maze();

		//----------PLAYER绘制-------------
		if (me.nowhp<=0)
		{
			int t=0;
			while (t<3)
			{
				if (timer(2000,1000)) ++t;
				setfillcolor(WHITE);
				setlinecolor(WHITE);
				fillcircle(10, 0, 100);
				putpng(NULL, 0, 0, &me_img_dead[me.direction][t]);
				FlushBatchDraw();
			}
			cleardevice();
			EndBatchDraw();
			return;
		}
		else if (mark_me_df)
		{
			if (timer(300, 1003))++me_df;
			if (me_df == 5)
			{
				me_df = 1; mark_me_df = 0;
			}
			if(me.direction==1|| me.direction == 2)
				putpng(NULL, 0, 0, &me_img_df[2][me_df]);
			else 
				putpng(NULL, 0, 0, &me_img_df[4][me_df]);
		}
		else if (mark_me_at)
		{
			if(timer(200, 10001))
			++me_at;
			if (me_at == 5)
			{
				me_at = 0; mark_me_at = 0;
			}
			cout << "d="<< me.direction<<" " <<" me_at"<<me_at << endl;
			putpng(NULL, 0, 0, &me_img_at[me.direction][me_at]);
		}
		else if (mark_me_move)
		{
			if (timer(100, 1002))++me_move;
			if (me_move == 5)	me_move = 1;
			putpng(NULL, 0, 0, &me_img_run[me.direction][me_move]);

		}
		else	putpng(NULL, 0, 0, &me_img_stand[me.direction]);
		
		for (int i = 1; i <= enemy_n; i++)//敌人绘制
		{
			if (enemy[i].nowhp <= 0)	continue;
				int tx = enemy[i].x - me.x, ty = enemy[i].y - me.y,td = enemy[i].direction;
				ty -= 50;
				if (td == 1)	td = 2;
				else if (td == 3)	td = 4;
				//enemy移动判断
				if (enemy_lastx[i] >enemy[i].x)
				{
					mark_enemy_move[i] = 1;
					enemy[i].direction = 4;
				}
				else if (enemy_lastx[i] < enemy[i].x)
				{
					mark_enemy_move[i] = 1;
					enemy[i].direction = 2;
				}
				else if (enemy_lasty[i] > enemy[i].y)
				{
					mark_enemy_move[i] = 1;
					enemy[i].direction = 1;

				}
				else if (enemy_lastx[i] < enemy[i].x)
				{
					mark_enemy_move[i] = 1;
					enemy[i].direction = 3;
				}
				enemy_lastx[i] = enemy[i].x;
				enemy_lasty[i] = enemy[i].y;

				//正式开始绘制敌人
				if (mark_enemy_at[i])
				{
					if (timer(200, 810+i))	++enemy_at[i];
					if (enemy_at[i] == 5)
					{
						enemy_at[i] = 1;  mark_enemy_at[i] = 0;
					}
					putpng(NULL, tx, ty, &enemy_img_at[i][td][enemy_at[i]]);
				}
				else if (mark_enemy_move[i])
				{
					if (timer(100, 820+i))++enemy_move[i];
					if (enemy_move[i] == 5)
					{
						enemy_move[i] = 1;
						mark_enemy_move[i] = 0;
					}
					putpng(NULL, tx, ty, &enemy_img_move[i][td][enemy_move[i]]);

				}
				else	putpng(NULL, tx, ty, &enemy_img_stand[i][td]);
		}
		
		//----------------信息绘制----------------------------
		State_mes();//动态信息
		State();//静态信息

		if (mark_map)//最高层绘制地图，按m判断是否
		{
			Map();
		}
		else if (mark_bk)
		{
			me=Backpack(me);
		}
		//=========================控制区=====================
		me = KeyControl(me);


		//=================判断区====================
		me = Enemy_At(me);//判断敌人攻击是否击中

		for (int i = 1; i <= enemy_n; i++)
			if (enemy[i].nowhp > 0 && dist(me.x, me.y, enemy[i].x, enemy[i].y) < cellW * 7)
			{
				if(timer(8, 3000+i))enemy[i] = Enemy_Move(enemy[i]);
			}

		if (mapx(me.x) == ENDI && mapy(me.y) == ENDJ)
		{
			cout << "yse" << endl;
			setorigin(0, 0);
			putimage(0, 0, &game_finish_img);
			Sleep(100);
		//	_getch();
			return;
			FlushBatchDraw();
		}

		FlushBatchDraw();
	}
	
}

//---以下为预处理区域---
void putpng(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //新版png
{
	x -= playerW/2, y -= playerW/2;
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}
void map1_dfs(int x, int y)//map1的dfs随机造图
{
	vis[x][y] = 0;

	if (map[x][y] == 3)
	{
		return;
	}
	srand(time(0) % 1926 + x * y  - clock() * 3 );//对于不同x、y引入不同seed，使每次产生不同的随机数以产生随机dfs方向

	if (rand() % 2 == 0)
		for (int i = 1; i <= 4; i++)
		{
			int xx = x + d[i][1], yy = y + d[i][2];

			if (xx > Wn || xx<1 || yy>Hn || yy < 1) continue;
			if (!vis[xx][yy]) continue;
			map1_dfs(xx, yy);

			map[x + ad[i][1]][y + ad[i][2]] = 1;

		}
	else
		for (int i = 4; i >= 1; i--)
		{
			int xx = x + d[i][1], yy = y + d[i][2];

			if (xx > Wn || xx<1 || yy>Hn || yy < 1) continue;
			if (!vis[xx][yy]) continue;
			map1_dfs(xx, yy);
		

			map[x + ad[i][1]][y + ad[i][2]] = 1;

		}


}
void map1_pre_dfs()//初始化dfs数据
{
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
	memset(map, 0, sizeof(map));
	for (int i = 1; i <= Wn; i++)
		for (int j = 1; j <= Hn; j++)
		{
			vis[i][j] = 1;
			if (i % 2 == 0 && j % 2 == 0) map[i][j] = 1;
		}
	map[2][2] = 2;//设置起点和终点，上面的会覆盖掉
	map[Wn - 1][Hn - 1] = 3;

	map1_dfs(2, 2);

	//以下为倍增地图
	for (int i = Wn; i >= 1; i--)
		for (int j = Hn; j >= 1; j--)
			map[i][2 * j] = map[i][2 * j - 1] = map[i][j];

	for (int i = Wn; i >= 1; i--)
		for (int j = Hn; j >= 1; j--)
			map[2 * i][j] = map[2 * i - 1][j] = map[i][j];

	for (int j = Hn + 1; j <= Hn + Hn; j++)
		for (int i = Wn; i >= 1; i--)
			map[2 * i][j] = map[2 * i - 1][j] = map[i][j];


	
}
void pre()//总预处理
{
	for (int i = 0; i <= 13; i++)
	{
		bk[i] = prething[i];
	}
//=============初始化图像=====================
	loadimage(&KEY, "./data/img/KEY.jpg");
	loadimage(&background, "./data/img/Forest.png", W*5, H*5);
	loadimage(&background_st, ("./data/img/background_st.jpg"));
	loadimage(&game_finish_img, "./data/img/game_finish.jpg");
	loadimage(&bigtree_img, "./data/img/big tree.png");
	loadimage(&backpack_img, "./data/img/backpack.png");
	loadimage(&map_img, "./data/img/map.png");

	//me相关读入
	loadimage(&me_img_state, "./data/img/me_state.png", cellW, cellW);
	for (int i = 1; i <= 4; i++)
	{
		loadimage(&me_img_df[2][i], (string("./data/img/me_df") + to_string(2) + string("_") + to_string(i) + string(".png")).c_str());
		loadimage(&me_img_df[4][i], (string("./data/img/me_df") + to_string(4) + string("_") + to_string(i) + string(".png")).c_str());

		loadimage(&me_img_dead[i][1], (string("./data/img/me_dead") + to_string(i) + string("_") + to_string(1) + string(".png")).c_str());
		loadimage(&me_img_dead[i][2], (string("./data/img/me_dead") + to_string(i) + string("_") + to_string(2) + string(".png")).c_str());
		loadimage(&me_img_stand[i], (string("./data/img/me_stand") + to_string(i) + string(".png")).c_str());
		for(int j=1;j<=5;j++)
			loadimage(&me_img_run[i][j], (string("./data/img/me_run") + to_string(i) + string("_") + to_string(j)+string(".png")).c_str());
		for (int j = 1; j <= 4; j++)
			loadimage(&me_img_at[i][j], (string("./data/img/me_at") + to_string(i) + string("_") + to_string(j) + string(".png")).c_str());
	}


	//enemy读入
	for (int i = 1; i <= enemy_n; i++)
	{
		loadimage(&enemy_img_stand[i][2], (string("./data/img/enemy") + to_string(1) + string("_stand") + to_string(2) + string(".png")).c_str());
		loadimage(&enemy_img_stand[i][4], (string("./data/img/enemy") + to_string(1) + string("_stand") + to_string(4) + string(".png")).c_str());
		
			for(int j = 1;  j<=4; j++)
			{

				loadimage(&enemy_img_move[i][2][j], (string("./data/img/enemy") + to_string(1) + string("_move") + to_string(2) + string("_") + to_string(j) + string(".png")).c_str());
				loadimage(&enemy_img_move[i][4][j], (string("./data/img/enemy") + to_string(1) + string("_move") + to_string(4) + string("_") + to_string(j) + string(".png")).c_str());
					
				loadimage(&enemy_img_at[i][2][j], (string("./data/img/enemy") + to_string(1) + string("_at") + to_string(2) + string("_") + to_string(j) + string(".png")).c_str());
				loadimage(&enemy_img_at[i][4][j], (string("./data/img/enemy") + to_string(1) + string("_at") + to_string(4) + string("_") + to_string(j) + string(".png")).c_str());
			}

		
	}
	
	for (int i = 0; i <= sth_n; i++)
		loadimage(&sth_img[i], (string("./data/img/sth") + to_string(i) + string(".jpg")).c_str(),cellW,cellW);

	for (int i = 1; i <= object_n; i++)
		loadimage(&object_img[i], (string("./data/ico/") + to_string(i) + string(".png")).c_str(), 75, 75);


	for (int i = 1; i <= npc_n; i++)
	{
		loadimage(&npc_img2[i], (string("./data/img/npc") + to_string(i) + string("lh.png")).c_str());
		for(int j=1;j<=4;j++)
		loadimage(&npc_img[i][j], (string("./data/img/npc") + to_string(i) + string("_") + to_string(j) + string(".png")).c_str());
	}
		
	//======================初始化音乐=========================
	mciSendString("open ./data/mus/village.wma alias village", NULL, 0, NULL);
	mciSendString("open ./data/mus/dead.wma alias dead", NULL, 0, NULL);
	mciSendString("open ./data/mus/battle.mp3 alias battle", NULL, 0, NULL);
	mciSendString("open ./data/mus/hit.mp3 alias hit", NULL, 0, NULL);
	mciSendString("open ./data/mus/op.mp3 alias op", NULL, 0, NULL);
//=============初始化画布=====================
	initgraph(W, H, EX_SHOWCONSOLE);//
	setbkcolor(Background_c);
	cleardevice();
	
//	setorigin(OriginX, OriginY);//设置坐标原点d
	setaspectratio(ScaleX, ScaleY);//设置缩放比例，其中符号*/
	settextstyle(50, 0, "微软雅黑");
	setbkmode(TRANSPARENT);
}


void start_menu()//开始菜单
{
	bool key = 0;
	putimage(0, 0, &KEY);
	_getch();
	Sleep(500);

	//_getch();
	BeginBatchDraw();
	mciSendString("play op", NULL, 0, NULL);
	while (1)
	{
		setorigin(0, 0);
		putimage(0, 0, &background_st);//贴（v.）图
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		setfillcolor(RED);
		fillrectangle(W / 6, H / 6, W / 4 + 100, H / 6 + 120);
		fillrectangle(W / 6, H / 6 + 180, W / 4 + 100, H / 6 + 300);
	//	rectangle(W / 6, H / 6 + 360, W / 4 + 100, H / 6 + 480);
		fillrectangle(W / 20, H * 5 / 6, W / 20 + 100, H * 5 / 6 + 60);
		LOGFONT f;//设置字体格式
		gettextstyle(&f);//获取字体格式
		f.lfHeight = 30;
		_tcscpy_s(f.lfFaceName, _T("黑体"));
		settextstyle(&f);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		RECT ra = { W / 6,H / 6,W / 4 + 100,H / 6 + 120 },
			rb = { W / 6,H / 6 + 180,W / 4 + 100,H / 6 + 300 },
			rc = { W / 6,H / 6 + 360,W / 4 + 100,H / 6 + 480 },
			rd = { W / 20,H * 5 / 6,W / 20 + 100,H * 5 / 6 + 60 };
		drawtext(_T("按W进入迷宫"), &ra, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("按A进入村子"), &rb, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//drawtext(_T("按S"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("按D退出"), &rd, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		char c;
		FlushBatchDraw();
		
		if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(87))
		{
			mciSendString("close op", NULL, 0, NULL);
			mapon = 1;
			EndBatchDraw();
			load_map1();
			BeginBatchDraw();
			continue;
		}
		if (GetAsyncKeyState(VK_LEFT) || GetAsyncKeyState(65))
		{
			mciSendString("close op", NULL, 0, NULL);
			mapon = 0;
			EndBatchDraw();
			load_map0();
			BeginBatchDraw();
			continue;
		}
		if (GetAsyncKeyState(VK_RIGHT) || GetAsyncKeyState(68))
		{
				EndBatchDraw();
				return;
		}	
	}
	EndBatchDraw();

}

int main()
{
	pre();
	start_menu();
	//setorigin(0, 0);
	//for (int j = 1; j <= 4; j++)
	//putpng(NULL,j*100, j*100, &me_img_at[2][j]);

	int _t = _getch();
	closegraph();
	return 0;
}