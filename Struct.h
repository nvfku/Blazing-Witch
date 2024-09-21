//存放定义的类
struct NODE {
	int x, y;
	int type;//记录map[x][y] 
	int f, g, h;//把g值放大了10倍，便于运算 
	NODE* father = NULL;//记录父节点 
	int backmark;
	bool is_open,is_close;//开闭集判断 

	//void print() { printf("%d   %d: f: %d  g: %d  h: %d   type: %d\n", x, y, f, g, h, type); }
	bool operator < (const NODE& cmp) const	{return f > cmp.f;}
};

struct PLAYER
{
	//move
	int x,y,v_x,v_y;
	//state 
	int kind;//0=敌人，1=玩家,2=sth,3=任务npc,4=商人npc,5=回血npc,6=装饰npc
	int direction;
	//===============
	int nowhp;
	int hp;//血量上限
	 int nowmp;
	int mp;//魔法值上限，每次魔法攻击消耗一些mp
	int at;
	int df;//防御值
	int mdf;//法抗值
	//普通攻击公式:目标血量-=攻击力-目标防御力
	//魔法攻击公式:目标血量-=攻击力x(1-目标法抗x100%)
};
struct OBJECT
{
	int x;//在背包中的位置
	int y;
	int seq;//物品编号
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
