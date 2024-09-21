//存放功能类函数
#include<math.h>
double dist(double x1, double y1, double x2, double y2)
{
	return sqrt((x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2));
}

int mh_dist(int x1, int y1, int x2, int y2)
{
	return abs(x2 - x1) + abs(y2 - y1);
}

//int direction(double x1, double y1, double x2, double y2)
//{
//	if (1)
//	{
//
//	}
//	return 1;
//}
//
