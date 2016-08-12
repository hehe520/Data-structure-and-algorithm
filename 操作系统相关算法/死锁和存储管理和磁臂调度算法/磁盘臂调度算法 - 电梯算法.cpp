
// 磁盘臂调度算法之电梯算法
// 假设有 100 条磁道, 随机产生 15 个寻道请求, 
// 随机产生一个磁头位置, 随机产生一个移动方向
// 输出服务顺序, 磁头总移动距离

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define NUM   100
int s[NUM];			// 100 条磁道
int DIR;			// 移动方向, 左-1, 右+1
int d[2] = {-1, +1};

// 电梯算法, 磁头一直向一个方向移动, 直到该方向没有请求了
// 反向继续扫描
int dian_ti(int pos)
{
	int dis = 0;
	int cur = pos;
	int flag;
	int count;
	// count 统计寻找的次数, 如果超过 NUM 次还没找到说明处理完了
	// 如果找了一个请求, 则清空 count
	
	printf("\n服务顺序: ");
	while (1)
	{
		flag = 0;
		count = 0;
		while (1)		// 找一个有请求的磁道
		{
			cur += DIR;
			if (cur <= -1)		// 左端越界, 反向
			{
				cur = 0;
				DIR = 1;
			}
			else if (cur >= NUM)	// 右端越界, 反向
			{
				cur = NUM - 1;
				DIR = -1;
			}
			if (s[cur] != 0)		// 找到
			{
				s[cur]--;
				count = 0;
				flag = 1;
				break;
			}
			count++;
			if (count > NUM)		// 没有请求了
			{
				count = 0;
				flag = 0;
				break;
			}
		}
		if (!flag) break;		// 找不到了
		printf("%d, ", cur);
		dis += abs(pos - cur);
		pos = cur;
	}
	return dis;
}

int main(void)
{
	int pos;		// 磁头位置
	int dis;		// 移动总距离
	int n = 15;
	int i, tmp;

	memset(s, 0, sizeof(s));
	srand(time(NULL));
	printf("请求顺序: ");
	for (i=0; i<n; i++)
	{
		tmp = rand() % NUM;
		s[tmp]++;
		printf("%d, ", tmp);
	}
	DIR = d[rand() % 2];
	pos = rand() % NUM;
	printf("\n磁头初始位置: %d\n", pos);
	dis = dian_ti(pos);
	printf("\n磁头移动总距离: %d\n", dis);

	return 0;
}