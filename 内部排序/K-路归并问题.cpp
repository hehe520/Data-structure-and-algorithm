
// K 个有序的表, 合并为一个有序表
// 把每个表的当前元素放入堆中,  每次删除最小值并放入新表中, 
// 然后加入这个最小值所在的表的下一个元素

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

// 10 个长度为 100 的有序表
int table[10][100];
int top[10];
int cur[10];

typedef struct node		// 结点
{
	int data;		// 数据
	int num;		// 表的编号
	bool operator < (const node &a) const
	{ return data > a.data; }
}node;

int main(void)
{
	void sort(int *num, int left, int right);
	std::priority_queue<node> q;
	node tmp;
	int i, j;

	srand(time(NULL));
	for (i=0; i<10; i++)		// 随机生成 10 个表
	{
		cur[i] = 0;
		top[i] = rand() % 10 + 1;		// 产生长度
		for (j=0; j<top[i]; j++)
		{
			table[i][j] = rand() % 1000;
		}
		sort(table[i], 0, top[i]-1);	// 有序
	}
	printf("10 个有序表:\n");
	for (i=0; i<10; i++)
	{
		printf("(%d): ", i+1);
		for (j=0; j<top[i]; j++)
		{
			printf("%d, ", table[i][j]);
		}
		printf("\n");
	}
	printf("\n合并后: ");
	for (i=0; i<10; i++)		// 首元入队
	{
		if (cur[i] < top[i])
		{
			tmp.data = table[i][cur[i]++];
			tmp.num = i;
			q.push(tmp);
		}
	}
	while (!q.empty())
	{
		tmp = q.top();
		q.pop();
		printf("%d, ", tmp.data);
		i = tmp.num;		// 同时把编号 i 的表入队
		if (cur[i] < top[i])
		{
			tmp.data = table[i][cur[i]++];
			tmp.num = i;
			q.push(tmp);
		}
	}
	putchar('\n');
	return 0;
}

// 快速排序
int move(int *num, int left, int right)
{
	int key = num[left];

	while (left < right)
	{
		while (left < right && num[right] >= key) right--;
		num[left] = num[right];
		while (left < right && num[left] <= key) left++;
		num[right] = num[left];
	}
	num[left] = key;
	return left;
}

void sort(int *num, int left, int right)
{
	int m;

	if (left < right)
	{
		m = move(num, left, right);
		sort(num, left, m);
		sort(num,  m + 1, right);
	}
}