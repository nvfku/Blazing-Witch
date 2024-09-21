#include"Head.h"
#include"Text.h"
#include"Struct.h"
#include"Setting.h"
bool timer(unsigned int duration, int id)//��ʱ�����ֱ�Ϊ�ӳ�ʱ��ͱ��
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
0~200ΪKeyControl()ר��	//0����ȫ���жϰ���

800~899ΪEnemy		//800~810Ϊ���ͼ��������
900~999Ϊnpc	//900~910Ϊ���ͼ��������,911~920Ϊ�������
>1000Ϊ���ר��
2000+100����backpage
3000+������
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
				if (abs(i) + abs(j) == 2) continue;//�޶��ĸ�����
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
NODE creat_n_map(int starti, int startj, int endi, int endj, int set)//ΪA*�㷨��ʼ������
{
	for (int i = 1; i <= Wn * 2; i++)//��ʼ��n_map
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
//--------------------------------����ΪPLAYER��Ϊ---------------------------------
bool Crash(int x, int y)//��ײ��⣬���ݽ�����Ҫ�ߵ�������
{
	y += 40,x+=10;//����λ�Ʋ���
	int di=mapx(x), dj = mapy(y);
	if (mapon == 1)
	{
		if (!map[di][dj]) return 0;
		//����д�ж�npc��enmey�����غϣ���������������return1
		return 1;
	}
	else if (mapon == 0)
	{
		
		cout << "�жϵ�" << mapon << "," << map0[di][dj] << "=" << map0[di][dj] << endl;
		
		//����д�ж�npc��enmey�����غϣ���������������return1
		if (map0[di][dj] == 1 || map0[di][dj] == 3|| map0[di][dj]==9|| map0[di][dj]==8) return 1;
		else return 0;
	}
	  
}
PLAYER KeyControl(PLAYER sb)//���̿���
{
	if (!timer(15, 0)||mark_me_at||mark_me_df)	return sb;
	mark_me_move =0;
	int nowtime = time(NULL);
	//=================ϵͳ����======================
	if (GetAsyncKeyState(27))//ESC��ͣ�˵�27
	{
		mark_gamepause = 1;
		mark_mapclose = 1;
		//game_pause();
	}
	if (GetAsyncKeyState(9))//TAB����
	{
		if (!timer(500, 9)) return sb;
		//Sys_map = nowtime;
		mark_bk = 1;
	}
	if (GetAsyncKeyState(77)&&mapon)//M��ͼ
	{
		if (!timer(500,77)) return sb;
		Sys_map = nowtime;
		mark_map = 1;
	}
	//==================��ɫ�ƶ�=======================
	if (GetAsyncKeyState(VK_UP) || GetAsyncKeyState(87))
	{
	//	cout << "����λ��" << "x=" << sb.x << ",y=" << sb.y << endl;
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
	//======================��ɫ����========================
	if (GetAsyncKeyState(69))//npc����
	{
		//cout << "press E" << endl;
		for (int i = 1; i <= npc_n; i++)
		{
			
			if (dist(npc[i].x, npc[i].y, sb.x, sb.y) <= 60)
			{//��д��ѡ�����ͣ�����NPC_Touch������
				NPC_Touch(npc[i].kind, i);
				//sb.nowhp += 10;
			}
		}
	}
	if (GetAsyncKeyState(74))//����
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
				//mark_at[i] = mark_at_ = 1;���б����ʱû���ã��õ�ʱ�ǵ������������ʾ��ʱ���жϸ�һ��else�ѱ�Ǽ�Ϊ0
				Sys_hit_starttime[i] = time(NULL);
			}
		}
	}
	if (GetAsyncKeyState(75))//����K
	{
		if (timer(1000, 75)) return sb;
		mark_me_df = 1;

		for (int i = 1; i <= enemy_n; i++)
		{
			if (enemy[i].nowhp <= 0)	continue;
			if (dist(enemy[i].x, enemy[i].y, sb.x, sb.y) <= 50)
			{
				enemy[i].nowhp -= 10;
				//mark_at[i] = mark_at_ = 1;���б����ʱû���ã��õ�ʱ�ǵ������������ʾ��ʱ���жϸ�һ��else�ѱ�Ǽ�Ϊ0
				Sys_hit_starttime[i] = time(NULL);
			}
		}
	}
	return sb;
}
PLAYER Enemy_Move(PLAYER sb)//����ͨ���ƶ���׷�𴥷�����
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
PLAYER Enemy_At(PLAYER sb)//���˹�����������(�����ࣩ
{
	for (int i = 1; i <= enemy_n; i++)
	{
		if (enemy[i].nowhp <= 0) continue;
		if (!enemy[i].kind)//��������������������
		{
			if (time(NULL) - enemy_at_starttime[i] < 1) continue;
			enemy_at_starttime[i] = time(NULL);

			int tdis = dist(enemy[i].x, enemy[i].y, sb.x, sb.y);
			
			if (tdis <= 50)//�жϽ��й���
			{
				mark_enemy_at[i] = 1;


				if (tdis <= 30)//�ж������Ƿ����
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


//-----------------����ΪNPCר��--------
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

//~~~~����Ϊ��Ϸ�ڴ�����Ϣ��ʾ����ģ��~~~~
void bar(int x,int y,int hp,int nowhp)//Ѫ�����ƺ���
{
	double t = nowhp*1.0 / hp;
	//cout << t << endl;
	setlinecolor(BLACK);
	setfillcolor(RGB(255, 250, 157));
	fillrectangle(x, y, x + 100, y + 10);
	setfillcolor(RED);
	fillrectangle(x, y, x + t*100, y + 10);
}     
void State()//��Խ�ɫ�����࣬Ѫ������mp�Ȳ����Լ�ϵͳ������Ϣ��ʾ��
{
	bar(-W / 2 + cellW * 2, -H / 2 + cellW, me.hp, me.nowhp);
	putpng(NULL, -W / 2 + cellW, -H / 2 + cellW, &me_img_state);

}
void State_mes()//��Խ�ɫ�˶��࣬��Ϸ�ڽ�����Ϣ����չʾ
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

//==============����Ϊ������=============
void draw_village()//20x20
{
	
	for (int i = 1; i <= Wn0; i++)
	{
		for (int j = 1; j <= Hn0; j++)
		{	
			if (map0[i][j] == 0)//�����ϰ����ɫ
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

			if (map[i][j] == 0)//�����ϰ����ɫ
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
	setaspectratio(0.13, 0.13);//�������ű��������з���*/
	int loci = mapx(me.x), locj = mapy(me.y);
	for (int i = 1; i <= Wn * 2; i++)
		for (int j = 1; j <= Hn * 2; j++)
		{
			//	//���Ʊ߿�
			//setlinecolor(WHITE);
			//rectangle(map_lu_x , map_lu_y , map_rd_x , map_rd_y );
			if (i == loci && j == locj)
			{
				setfillcolor(RED);

				setlinecolor(BLACK);
				fillrectangle(map_lu_x, map_lu_y, map_rd_x, map_rd_y);
				
			}
			else if (map[i][j] == 0)//�����ϰ���
			{
				setlinecolor(BLACK);
				setfillcolor(YELLOW);
				fillrectangle(map_lu_x , map_lu_y , map_rd_x , map_rd_y );
			}
			else if (map[i][j] == 3)//���ƽ����㣬һ����
			{
				setlinecolor(BLUE);
				line(map_lu_x, map_lu_y, map_rd_x, map_rd_y);
				line(map_ru_x, map_ru_y, map_ld_x, map_ld_y);
			}
			else if (map[i][j] == 2)//������ʼ�㣬һ��Ȧ
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


PLAYER Backpack(PLAYER sb)//�����򿪺���
{
	RECT bkshow = { 915 ,200,1150,640 };
	RECT bkname = { 900, 90, 1150, 190 };
	int ti = 1, tj = 1;
	//�����̲������߼���Ϊ��������ѡ��
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
								drawtext((bk[p].name+string("\n������")+to_string(bk[p].num)).c_str(), &bkname, DT_LEFT | DT_WORDBREAK);
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

		//��Ʒѡ��
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

		//ʹ��
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
void Map()//��ͼ�򿪺���
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
			//================������==========================
		setorigin(W / 2, H / 2);
		putimage(-me.x, -me.y, &background);

		//��ͼ����
		draw_village();

		//PLAYER����
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
		//=========================������=====================
		me = KeyControl(me);

	//=================�ж���====================
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
	map1_pre_dfs();//��ʼ��������ɵ�ͼ
	Set_player1();

	cleardevice();
	BeginBatchDraw();
	while (1)
	{
		//================������==========================
		setorigin(W / 2, H / 2);
		putimage(-300-me.x, -300-me.y, &background);

		//��ͼ����
		draw_maze();

		//----------PLAYER����-------------
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
		
		for (int i = 1; i <= enemy_n; i++)//���˻���
		{
			if (enemy[i].nowhp <= 0)	continue;
				int tx = enemy[i].x - me.x, ty = enemy[i].y - me.y,td = enemy[i].direction;
				ty -= 50;
				if (td == 1)	td = 2;
				else if (td == 3)	td = 4;
				//enemy�ƶ��ж�
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

				//��ʽ��ʼ���Ƶ���
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
		
		//----------------��Ϣ����----------------------------
		State_mes();//��̬��Ϣ
		State();//��̬��Ϣ

		if (mark_map)//��߲���Ƶ�ͼ����m�ж��Ƿ�
		{
			Map();
		}
		else if (mark_bk)
		{
			me=Backpack(me);
		}
		//=========================������=====================
		me = KeyControl(me);


		//=================�ж���====================
		me = Enemy_At(me);//�жϵ��˹����Ƿ����

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

//---����ΪԤ��������---
void putpng(IMAGE* dstimg, int x, int y, IMAGE* srcimg) //�°�png
{
	x -= playerW/2, y -= playerW/2;
	HDC dstDC = GetImageHDC(dstimg);
	HDC srcDC = GetImageHDC(srcimg);
	int w = srcimg->getwidth();
	int h = srcimg->getheight();
	BLENDFUNCTION bf = { AC_SRC_OVER, 0, 255, AC_SRC_ALPHA };
	AlphaBlend(dstDC, x, y, w, h, srcDC, 0, 0, w, h, bf);
}
void map1_dfs(int x, int y)//map1��dfs�����ͼ
{
	vis[x][y] = 0;

	if (map[x][y] == 3)
	{
		return;
	}
	srand(time(0) % 1926 + x * y  - clock() * 3 );//���ڲ�ͬx��y���벻ͬseed��ʹÿ�β�����ͬ��������Բ������dfs����

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
void map1_pre_dfs()//��ʼ��dfs����
{
	/*for(int y=0; y<480; y++)
{
   setcolor(RGB(100,100,y/3+20));
   line(0, y, 640, y);
   line(9999, 0, -9999, 0);
line(0, 99999, 0, -99990);
}*/
//����Ϊdfs�������ͼ
	d[1][1] = 2, d[2][1] = -2, d[3][2] = 2, d[4][2] = -2;
	ad[1][1] = 1, ad[2][1] = -1, ad[3][2] = 1, ad[4][2] = -1;
	memset(map, 0, sizeof(map));
	for (int i = 1; i <= Wn; i++)
		for (int j = 1; j <= Hn; j++)
		{
			vis[i][j] = 1;
			if (i % 2 == 0 && j % 2 == 0) map[i][j] = 1;
		}
	map[2][2] = 2;//���������յ㣬����ĻḲ�ǵ�
	map[Wn - 1][Hn - 1] = 3;

	map1_dfs(2, 2);

	//����Ϊ������ͼ
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
void pre()//��Ԥ����
{
	for (int i = 0; i <= 13; i++)
	{
		bk[i] = prething[i];
	}
//=============��ʼ��ͼ��=====================
	loadimage(&KEY, "./data/img/KEY.jpg");
	loadimage(&background, "./data/img/Forest.png", W*5, H*5);
	loadimage(&background_st, ("./data/img/background_st.jpg"));
	loadimage(&game_finish_img, "./data/img/game_finish.jpg");
	loadimage(&bigtree_img, "./data/img/big tree.png");
	loadimage(&backpack_img, "./data/img/backpack.png");
	loadimage(&map_img, "./data/img/map.png");

	//me��ض���
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


	//enemy����
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
		
	//======================��ʼ������=========================
	mciSendString("open ./data/mus/village.wma alias village", NULL, 0, NULL);
	mciSendString("open ./data/mus/dead.wma alias dead", NULL, 0, NULL);
	mciSendString("open ./data/mus/battle.mp3 alias battle", NULL, 0, NULL);
	mciSendString("open ./data/mus/hit.mp3 alias hit", NULL, 0, NULL);
	mciSendString("open ./data/mus/op.mp3 alias op", NULL, 0, NULL);
//=============��ʼ������=====================
	initgraph(W, H, EX_SHOWCONSOLE);//
	setbkcolor(Background_c);
	cleardevice();
	
//	setorigin(OriginX, OriginY);//��������ԭ��d
	setaspectratio(ScaleX, ScaleY);//�������ű��������з���*/
	settextstyle(50, 0, "΢���ź�");
	setbkmode(TRANSPARENT);
}


void start_menu()//��ʼ�˵�
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
		putimage(0, 0, &background_st);//����v.��ͼ
		setlinecolor(BLACK);
		setlinestyle(PS_SOLID, 1);
		setfillcolor(RED);
		fillrectangle(W / 6, H / 6, W / 4 + 100, H / 6 + 120);
		fillrectangle(W / 6, H / 6 + 180, W / 4 + 100, H / 6 + 300);
	//	rectangle(W / 6, H / 6 + 360, W / 4 + 100, H / 6 + 480);
		fillrectangle(W / 20, H * 5 / 6, W / 20 + 100, H * 5 / 6 + 60);
		LOGFONT f;//���������ʽ
		gettextstyle(&f);//��ȡ�����ʽ
		f.lfHeight = 30;
		_tcscpy_s(f.lfFaceName, _T("����"));
		settextstyle(&f);
		setbkmode(TRANSPARENT);
		settextcolor(BLACK);
		RECT ra = { W / 6,H / 6,W / 4 + 100,H / 6 + 120 },
			rb = { W / 6,H / 6 + 180,W / 4 + 100,H / 6 + 300 },
			rc = { W / 6,H / 6 + 360,W / 4 + 100,H / 6 + 480 },
			rd = { W / 20,H * 5 / 6,W / 20 + 100,H * 5 / 6 + 60 };
		drawtext(_T("��W�����Թ�"), &ra, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��A�������"), &rb, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		//drawtext(_T("��S"), &rc, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
		drawtext(_T("��D�˳�"), &rd, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
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