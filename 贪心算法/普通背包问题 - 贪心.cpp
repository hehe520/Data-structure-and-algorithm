#include <stdio.h>
#include <stdlib.h>

#define SIZE 200

typedef struct
{
	int weight;
	int value;
	float avg;
}recond;

// result[] 记录拿了多少, result[k] = x, 第 k 个拿了 result[k] 重量
float result[SIZE];
int top;

// void *x 转为 recond* 类型是 (recond *)x, 在提取内容 *(recond *)x
// 再提取 avg, (*(recond *)x).ave
int cmp(const void *x, const void *y)
{
	return (*(recond *)x).avg < (*(recond *)y).avg;
}

// 普通背包的贪心解法, 按价值重量比排序, 每次放最有价值的
float tanxin(recond *r, int c, int n)
{
	float sum = 0.0f;
	int i;
	
	for (i=0; i<n; i++)		// 按照价值重量比排序
		r[i].avg = (float)(r[i].value) / r[i].weight;
	qsort(r, n, sizeof(r[0]), cmp);

	for (i=0, top=0; i<n; i++)
	{
		if (r[i].weight <= c)		// 全部放入
		{
			sum += r[i].value;
			c -= r[i].weight;
			result[top++] = (float)r[i].value;		// 记录拿了多少
		}
		else		// 部分放入, 并退出
		{
			sum += c * r[i].avg;
			result[top++] = ((float)c / r[i].weight) * r[i].value;
			break;
		}
	}
	return sum;
}

int main(void)
{
	recond r[SIZE];
	float sum;
	int c, n;
	int i;

	printf("输入背包最大容量及物品总数: ");
	scanf("%d %d", &c, &n);
	printf("输入 %d 件物品: (重量 价值)\n", n);
	for (i=0; i<n; i++)
		scanf("%d %d", &(r[i].weight), &(r[i].value));
	sum = tanxin(r, c, n);
	printf("\n最大价值: %0.2f\n", sum);
	for (i=0; i<top; i++)
		printf("\n重量为 %d 的, 拿了 %.2f", r[i].weight, result[i]);
	printf("\n");
	return 0;
}