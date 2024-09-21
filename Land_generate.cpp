#include<iostream>
using namespace std;
int map[30][30];
int main()
{
	const int wn = 20;
	freopen("ss.txt", "w", stdout);

	for (int i = 1; i <= wn; i++)
	{
		{
			if (i == 1)
				printf("{0,  ");
			else if (i == wn)
				printf("0}, ");
			else printf("0, ");
		}
	}
	cout << endl;
	for (int j = 2; j < wn; j++)
	{
		for (int i = 1; i <= wn; i++)
		{
			if (i == 1)
				printf("{0,  ");
			else if (i == wn)
				printf("0}, ");
			else printf("1, ");
		}
		cout << endl;
	}
	for (int i = 1; i <= wn; i++)
	{
		{
			if (i == 1)
				printf("{0,  ");
			else if (i == wn)
				printf("0} ");
			else printf("0, ");
		}
	}

	return 0;
}