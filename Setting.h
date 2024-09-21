//=============================主要全局设置=======================//
const int W = 1280, H = 720;
#define		Wn	21	//MazeWidth，迷宫总格子行数
#define		Hn 21	//记住一定要为奇数且Wn=Hn
#define		Wn0	21	
#define		Hn0 21	
#define		Wnbk 5	
#define		Hnbk 5	

#define		OriginX		0		//地图原点x坐标
#define		OriginY		0		//地图原点y坐标
#define     ScaleX	1	
#define     ScaleY	1
#define		cellW	60		//单元格宽度 
#define		playerW	50

const int enemy_maxn = 10, npc_maxn = 10, sth_maxn = 20,object_maxn=20;
const int Background_c =BLUE;

bool mark_mapclose;//全局地图关闭请求
int ENDI= 2 * Wn - 2, ENDJ= 2 * Hn - 2,ENDX= realx(2 * Wn - 2),ENDY= realy(2 * Hn - 2);//全局终点
int mapon;//目前启用了哪个地图
int map[Wn * 4][Hn * 4];//障碍=0红色,ROAD =1绿色,START	=2,END	=3
enum Type_map {
	Wall, Road, Start, End
};
bool vis[Wn*4][Hn*4];//vis=0是标记节点已访问过
int d[10][10],ad[10][10];
NODE n_map[Wn*4][Hn*4];
priority_queue<NODE> open; 

IMAGE KEY,game_finish_img,background_st, background,bigtree_img, backpack_img,map_img;//存储图像信息的结构体变量
IMAGE object[object_maxn];

IMAGE me_img_stand[10], me_img_run[10][10], me_img_at[10][10], me_img_state, me_img_dead[10][10],me_img_df[10][10];
bool mark_me_move,mark_me_at,mark_me_dead,mark_me_df;
int me_move,me_at,me_df;

IMAGE enemy_img_stand[enemy_maxn][5], enemy_img_move[enemy_maxn][5][5], enemy_img_at[enemy_maxn][5][5];
bool mark_enemy_at[enemy_maxn], mark_enemy_move[enemy_maxn];
int enemy_at[enemy_maxn], enemy_move[enemy_maxn];//enemy[]的at和move动画执行记录器
int enemy_lastx[enemy_maxn], enemy_lasty[enemy_maxn];
int at_starttime, mark_me_at_doing;

IMAGE npc_img[npc_maxn][10],npc_img2[npc_maxn], object_img[200], sth_img[200];
int npc_show[npc_maxn];
//int img_n;
const int lux= 200, luy=500, rdx=900, rdy=600;
RECT npc_talk = {lux,luy,rdx,rdy};


PLAYER me;
PLAYER enemy[enemy_maxn], npc[npc_maxn];
int enemy_n = 5, npc_n = 3;
int number1;
NODE enemyrand[Wn * Hn];

PLAYER sth[sth_maxn * 2];
int sth_n=20;

vector<OBJECT> bk(object_maxn);
int object_n=13;
OBJECT prething[object_maxn] = {
{0,0,0},//x,y,seq,num
{0,0,1,0,"黄金","[任务物品]\n看上去就很值钱的样子"},
{0,0,2,1,"钥匙","[任务物品]\n能打开一扇门的钥匙" },
{0,0,3,1,"生命药水","[恢复品]\n红色的药水。\n\n恢复100HP" ,100},
{0,0,4,1,"魔法药水","[恢复品]\n蓝色的药水。\n\n恢复50MP" },
{0,0,5,1,"护佑药水","[恢复品]\n绿色的药水。\n\n防御力增加100，法抗增加10"},
{0,0,6,1,"烈酒","[恢复品]\n露西亚特产，喝了似乎能让人战斗欲望上升\n\n攻击力+20"},
{0,0,7,1,"药丸","[恢复品]\n露西亚最新科学成果，吃下这奇怪的东西似乎就不会头疼了。\n\n恢复100HP",100},
{0,0,8,9,"黒面包","[恢复品]\n黑色、坚硬的面包，是露西亚统一供应的平民二类粮食。\n\n恢复50HP",50},
{0,0,9,1,"巧克力糖浆","[恢复品]\n非~常甜的糖浆，吃下后感觉全身充满了力量，通常是帝国发给前线士兵的能量补充品。\n\n恢复200HP和60MP",200,60},
{0,0,10,2,"轻甲","[武器]\n轻便的装甲，通常是侦察兵的着装，非常适合移动。\n\n防御力+50"},
{0,0,11,1," 洛斯里克之剑","[武器]\n传说古时有一位德高望重的领袖在远东村建立了王国，这是他在抵抗敌人入侵牺牲在前线时所持武器。\n\n攻击力+150"},
{0,0,12,1,"凯瑟琳的长剑","[武器]\n凯瑟琳习武之时就随身携带的。\n\n攻击力+30"},
{0,0,13,1,"物理学圣剑","[武器]\n咯噔！这个战斗力爆表的武器是开发者送的礼物。\n\n攻击力+500",0,0,500}
};


//物体交互类间隔时间
int  enemy_at_starttime[100];//玩家、敌人攻击强制间隔时间
int touch_starttime[npc_maxn];//npc交互强制间隔时间

//标签刷新类间隔时间
int Sys_hit_starttime[enemy_maxn];//玩家击中敌人i标签强制间隔时间
int Sys_enemy_hit_starttime[enemy_maxn];//敌人i攻击成功标签强制刷新间隔时间
int Sys_map;//地图打开强制间隔

bool mark_at[100], mark_at_;//玩家at触发器
bool mark_npc[100];//npc交互触发器

bool mark_sth[sth_maxn];//全局sth允许绘画记录器
bool mark_nenmy_dead[enemy_maxn];
bool mark_map,mark_bk,mark_gamepause;

long long gold, xp;//全局玩家拥有的金币和经验
ExMessage m;
//============================================
void Set_player0()
{
	/*一到四int x,y,v_x,v_y;
	五int kind;//0=敌人，1=玩家,2=sth,3=任务npc,4=商人npc,5=回血npc,6=装饰npc
	六int direction;
	七int nowhp;
	八int hp;*/
	me = { realx(17) + 10 , realy(20) + 10,5,5,1,4,1234,1234 ,20 };
	npc[1] = { realx(5) ,realy(10)};
	npc[2] = { realx(10) ,realy(10) };
	npc[1].kind = 3,npc[2].kind=4;
}
void Set_player1()
{
	number1 = 0;
	for (int i = 1; i <= Wn*2; i++)
	{
		for (int j = 1; j <= Hn*2; j++)
		{
			if (map[i][j] == 1)
			{ 
				++number1;
				enemyrand[number1].x = i;
				enemyrand[number1].y = j;
			}
		}
	} 
	me = { realx(3) + 10 , realy(3) + 10,5,5,1,4,120,120 ,20 };
	enemy[1] = { ENDX+ 10 , ENDY+ 10 ,1,1,0,2,200,200,20 };
	cout << number1 << endl;
	for (int i = 2; i <= 5; i++)
	{
		
		srand(time(NULL) * i+i);
		int rr = (rand() % (number1 - 10) + 10);
		enemy[i] = { realx(enemyrand[rr].x) + 10 ,realy(enemyrand[rr].y) + 10 ,1,1,0,2,30,30 ,20 };
	}
	sth[1] = { 100,100 };
}


int map0[Wn * 3][Hn * 3] = {
{0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0},
{0,  0, 0, 0, 5, 5, 5, 5, 5, 2, 0, 0, 0, 0, 5, 5, 5, 5, 5, 2, 0, 0, 0},
{0,  0, 1, 1, 6, 3, 3, 3, 3, 2, 1, 1, 1, 1, 6, 3, 3, 3, 3, 2, 1, 1, 0},
{0,  0, 1, 1, 6, 3, 3, 3, 3, 2, 1, 1, 1, 1, 6, 3, 3, 3, 3, 2, 1, 1, 0},
{0,  0, 1, 1, 6, 3, 3, 3, 3, 2, 1, 1, 1, 1, 6, 3, 3, 3, 3, 2, 1, 1, 0},
{0,  0, 1, 1, 4, 4, 4, 3, 4, 2, 1, 1, 1, 1, 4, 4, 3, 4, 4, 2, 1, 1, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0,  5, 5, 5, 5, 2, 1, 1, 9, 9, 9, 9, 9, 9, 9, 9, 1, 1, 5, 5, 5, 5, 5},
{0,  6, 3, 3, 3, 2, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 6, 3, 3, 3, 2},
{0,  6, 3, 3, 3, 3, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 3, 3, 3, 3, 2},
{0,  6, 3, 3, 3, 2, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 6, 3, 3, 3, 2},
{0,  6, 3, 3, 3, 2, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 6, 3, 3, 3, 2},
{0,  4, 4, 4, 4, 2, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 4, 4, 4, 4, 4},
{0,  0, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 8, 1, 1, 1, 1, 1, 1, 0},
{0,  11, 9, 9, 9, 9, 9, 9, 9, 1, 1, 1, 1, 1, 1, 9, 9, 9, 9, 9, 9, 9, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0,  0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 0},
{0,  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0}
};

#define		map_lu_x	cellW * i + OriginX
#define		map_ld_x	cellW * i + OriginX
#define		map_ru_x	cellW * (i+1) + OriginX
#define		map_rd_x	cellW * (i+1) + OriginX

#define		map_lu_y	cellW * j + OriginY
#define		map_ru_y	cellW * j + OriginY
#define		map_ld_y	cellW * (j+1) + OriginY
#define		map_rd_y	cellW * (j+1) + OriginY


int realx(int x)
{
	return cellW * x + OriginX;
}
int realy(int y)
{
	return cellW * y + OriginY;
}
int mapx(int x)
{
	return (x - OriginX) / cellW;
}
int mapy(int y)
{
	return (y - OriginY) / cellW;
}