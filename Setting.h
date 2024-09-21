//=============================��Ҫȫ������=======================//
const int W = 1280, H = 720;
#define		Wn	21	//MazeWidth���Թ��ܸ�������
#define		Hn 21	//��סһ��ҪΪ������Wn=Hn
#define		Wn0	21	
#define		Hn0 21	
#define		Wnbk 5	
#define		Hnbk 5	

#define		OriginX		0		//��ͼԭ��x����
#define		OriginY		0		//��ͼԭ��y����
#define     ScaleX	1	
#define     ScaleY	1
#define		cellW	60		//��Ԫ���� 
#define		playerW	50

const int enemy_maxn = 10, npc_maxn = 10, sth_maxn = 20,object_maxn=20;
const int Background_c =BLUE;

bool mark_mapclose;//ȫ�ֵ�ͼ�ر�����
int ENDI= 2 * Wn - 2, ENDJ= 2 * Hn - 2,ENDX= realx(2 * Wn - 2),ENDY= realy(2 * Hn - 2);//ȫ���յ�
int mapon;//Ŀǰ�������ĸ���ͼ
int map[Wn * 4][Hn * 4];//�ϰ�=0��ɫ,ROAD =1��ɫ,START	=2,END	=3
enum Type_map {
	Wall, Road, Start, End
};
bool vis[Wn*4][Hn*4];//vis=0�Ǳ�ǽڵ��ѷ��ʹ�
int d[10][10],ad[10][10];
NODE n_map[Wn*4][Hn*4];
priority_queue<NODE> open; 

IMAGE KEY,game_finish_img,background_st, background,bigtree_img, backpack_img,map_img;//�洢ͼ����Ϣ�Ľṹ�����
IMAGE object[object_maxn];

IMAGE me_img_stand[10], me_img_run[10][10], me_img_at[10][10], me_img_state, me_img_dead[10][10],me_img_df[10][10];
bool mark_me_move,mark_me_at,mark_me_dead,mark_me_df;
int me_move,me_at,me_df;

IMAGE enemy_img_stand[enemy_maxn][5], enemy_img_move[enemy_maxn][5][5], enemy_img_at[enemy_maxn][5][5];
bool mark_enemy_at[enemy_maxn], mark_enemy_move[enemy_maxn];
int enemy_at[enemy_maxn], enemy_move[enemy_maxn];//enemy[]��at��move����ִ�м�¼��
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
{0,0,1,0,"�ƽ�","[������Ʒ]\n����ȥ�ͺ�ֵǮ������"},
{0,0,2,1,"Կ��","[������Ʒ]\n�ܴ�һ���ŵ�Կ��" },
{0,0,3,1,"����ҩˮ","[�ָ�Ʒ]\n��ɫ��ҩˮ��\n\n�ָ�100HP" ,100},
{0,0,4,1,"ħ��ҩˮ","[�ָ�Ʒ]\n��ɫ��ҩˮ��\n\n�ָ�50MP" },
{0,0,5,1,"����ҩˮ","[�ָ�Ʒ]\n��ɫ��ҩˮ��\n\n����������100����������10"},
{0,0,6,1,"�Ҿ�","[�ָ�Ʒ]\n¶�����ز��������ƺ�������ս����������\n\n������+20"},
{0,0,7,1,"ҩ��","[�ָ�Ʒ]\n¶�������¿�ѧ�ɹ�����������ֵĶ����ƺ��Ͳ���ͷ���ˡ�\n\n�ָ�100HP",100},
{0,0,8,9,"�\���","[�ָ�Ʒ]\n��ɫ����Ӳ���������¶����ͳһ��Ӧ��ƽ�������ʳ��\n\n�ָ�50HP",50},
{0,0,9,1,"�ɿ����ǽ�","[�ָ�Ʒ]\n��~������ǽ������º�о�ȫ�������������ͨ���ǵ۹�����ǰ��ʿ������������Ʒ��\n\n�ָ�200HP��60MP",200,60},
{0,0,10,2,"���","[����]\n����װ�ף�ͨ������������װ���ǳ��ʺ��ƶ���\n\n������+50"},
{0,0,11,1," ��˹���֮��","[����]\n��˵��ʱ��һλ�¸����ص�������Զ���彨�����������������ڵֿ���������������ǰ��ʱ����������\n\n������+150"},
{0,0,12,1,"��ɪ�յĳ���","[����]\n��ɪ��ϰ��֮ʱ������Я���ġ�\n\n������+30"},
{0,0,13,1,"����ѧʥ��","[����]\n���⣡���ս��������������ǿ������͵����\n\n������+500",0,0,500}
};


//���彻������ʱ��
int  enemy_at_starttime[100];//��ҡ����˹���ǿ�Ƽ��ʱ��
int touch_starttime[npc_maxn];//npc����ǿ�Ƽ��ʱ��

//��ǩˢ������ʱ��
int Sys_hit_starttime[enemy_maxn];//��һ��е���i��ǩǿ�Ƽ��ʱ��
int Sys_enemy_hit_starttime[enemy_maxn];//����i�����ɹ���ǩǿ��ˢ�¼��ʱ��
int Sys_map;//��ͼ��ǿ�Ƽ��

bool mark_at[100], mark_at_;//���at������
bool mark_npc[100];//npc����������

bool mark_sth[sth_maxn];//ȫ��sth����滭��¼��
bool mark_nenmy_dead[enemy_maxn];
bool mark_map,mark_bk,mark_gamepause;

long long gold, xp;//ȫ�����ӵ�еĽ�Һ;���
ExMessage m;
//============================================
void Set_player0()
{
	/*һ����int x,y,v_x,v_y;
	��int kind;//0=���ˣ�1=���,2=sth,3=����npc,4=����npc,5=��Ѫnpc,6=װ��npc
	��int direction;
	��int nowhp;
	��int hp;*/
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