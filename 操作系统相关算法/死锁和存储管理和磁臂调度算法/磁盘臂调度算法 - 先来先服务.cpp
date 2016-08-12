
// 磁盘臂调度之先来先服务
// 假设有 100 条磁道, 随机产生 15 个寻道请求, 随机产生一个磁头位置
// 输出服务顺序, 磁头总移动距离

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <math.h>
#include <queue>

#define NUM 100
int s[NUM];			// 100 条磁道
std::queue<int> q;		// 处理队列

// 先来先服务
int FIFO(int pos)
{
	int dis = 0;
	int tmp;

	printf("\n服务顺序: ");
	while (!q.empty())
	{
		tmp = q.front();   q.pop();
		s[tmp]--;
		printf("%d, ", tmp);
		dis += abs(tmp - pos);		// 移动距离
		pos = tmp;			// 去到当前位置
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
		q.push(tmp);
		printf("%d, ", tmp);
	}
	pos = rand() % NUM;
	printf("\n磁头初始位置: %d\n", pos);
	dis = FIFO(pos);
	printf("\n磁头移动总距离: %d\n", dis);
	return 0;
}