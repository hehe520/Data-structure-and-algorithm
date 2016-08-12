
// 集合 A 中有一堆数字, 集合 B 中有一堆数字, 求共有的数字
// 如果集合中的元素不是数字, 可以提取"信息指纹", 将其数字化
// 例如, 如果是字符串的集合可以将其哈希成一个整数, 再存入哈希表

// 解法1: 将集合 A 存入哈希表, 集合 B 的每个元素进入每个桶中进行比对
// 解法2: 数据范围在 1000 内, 可用 map[] 标记

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

typedef struct node
{
	int data;
	node *next;
}node;

node *table[10];		// 哈希表

inline int hash(int data)		// 哈希函数
{
	return data % 10;
}

void insert(int data)
{
	int add = hash(data);
	node *cur = (node *)malloc(sizeof(node));
	
	cur->data = data;
	cur->next = table[add];
	table[add] = cur;
}

int main(void)
{
	int a[100];
	int b[100];
	int i, add;
	node *tmp;

	// 初始化哈希表
	for (i=0; i<10; i++) table[i] = NULL;

	// 初始化数据
	srand(time(NULL));
	for (i=0; i<100; i++)
	{
		a[i] = rand() % 1000;
		b[i] = rand() % 1000;
	}
	printf("\nA 中元素: ");
	for (i=0; i<100; i++) printf("%d, ", a[i]);
	printf("\nB 中元素: ");
	for (i=0; i<100; i++) printf("%d, ", b[i]);
	printf("\n\n相同的元素: ");
	for (i=0; i<100; i++)		// 将 a 插入哈希表
		insert(a[i]);
	for (i=0; i<100; i++)		// 遍历 b 进行判断
	{
		add = hash(b[i]);
		tmp = table[add];
		while (tmp != NULL)
		{
			if (tmp->data == b[i])
			{
				printf("%d, ", b[i]);
				break;
			}
			else tmp = tmp->next;
		}
	}
	putchar('\n');
	for (i=0; i<10; i++)		// 销毁哈希表
	{
		while (table[i] != NULL)
		{
			tmp = table[i];
			table[i] = table[i]->next;
			free(tmp);
		}
	}
	return 0;
}
