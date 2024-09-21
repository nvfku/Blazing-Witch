//��Ŷ������
struct NODE {
	int x, y;
	int type;//��¼map[x][y] 
	int f, g, h;//��gֵ�Ŵ���10������������ 
	NODE* father = NULL;//��¼���ڵ� 
	int backmark;
	bool is_open,is_close;//���ռ��ж� 

	//void print() { printf("%d   %d: f: %d  g: %d  h: %d   type: %d\n", x, y, f, g, h, type); }
	bool operator < (const NODE& cmp) const	{return f > cmp.f;}
};

struct PLAYER
{
	//move
	int x,y,v_x,v_y;
	//state 
	int kind;//0=���ˣ�1=���,2=sth,3=����npc,4=����npc,5=��Ѫnpc,6=װ��npc
	int direction;
	//===============
	int nowhp;
	int hp;//Ѫ������
	 int nowmp;
	int mp;//ħ��ֵ���ޣ�ÿ��ħ����������һЩmp
	int at;
	int df;//����ֵ
	int mdf;//����ֵ
	//��ͨ������ʽ:Ŀ��Ѫ��-=������-Ŀ�������
	//ħ��������ʽ:Ŀ��Ѫ��-=������x(1-Ŀ�귨��x100%)
};
struct OBJECT
{
	int x;//�ڱ����е�λ��
	int y;
	int seq;//��Ʒ���
	int num;
	string name;
	string describe;
	int hp;
	int mp;
	int at;
	int df;
	int mdf;
	int price;
};
