
// 磁盘臂调度算法之最短寻道优先
// 假设有 100 条磁道, 随机产生 15 个寻道请求, 随机产生一个磁头位置
// 输出服务顺序, 磁头总移动距离

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <math.h>

#define NUM 100
int s[NUM];			// 100 条磁道

// 以 pos 为中心, 向两边扫描, 找最近的寻道请求
int short_path(int pos)
{
	int dis = 0;
	int left, right;	// left 向左遍历, right 向右遍历
	int zuo, you;
	int flag = 1;

	printf("\n服务顺序: ");
	left = right = pos;
	while (1)			// 向两端扫描
	{
		for ( ; left  > -1  && s[left]==0;  left--);	// 左扫
		for ( ; right < NUM && s[right]==0; right++);	// 右扫

		// left == -1 表示左边没有了, right == NUM 表示右边没有了
		// 利用二进制的思想判断, left, right 的四种状态
		switch ((left == -1) + (right == NUM) * 2)
		{
		case 0 :		// s[left], s[right] 均有, 找一个最近的
			zuo = abs(pos - left);
			you = abs(pos - right);
			if (zuo < you)
			{
				printf("%d, ", left);
				dis += zuo;
				pos = zuo;
				s[left]--;		// 清空标记
			}
			else 
			{
				printf("%d, ", right);
				dis += you;
				pos = you;
				s[right]--;
			}
			break;
		case 1 :		// 仅 s[right] 有请求
			you = abs(pos - right);
			printf("%d, ", right);
			dis += you;
			pos = you;
			s[right]--;
			break;
		case 2 :		// 仅 s[left]  有请求
			zuo = abs(pos - left);
			printf("%d, ", left);
			dis += zuo;
			pos = zuo;
			s[left]--;
			break;
		case 3 :		// s[left], s[right] 均没请求了
			flag = 0;
			break;
		}
		if (!flag) break;
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
	pos = rand() % NUM;
	printf("\n磁头初始位置: %d\n", pos);
	dis = short_path(pos);
	printf("\n磁头移动总距离: %d\n", dis);

	return 0;
}