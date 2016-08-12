#include <stdio.h>
#include <stdlib.h>

// 块状数组 - 数组 + 链表

typedef int elemtype;
typedef int status;
#define ERROR 0
#define OK 1
#define SIZE 5		// 每个结点放 5 个元素
// 储存密度 = (SIZE * sizeof(elemtype)) / (sizeof(node));

typedef struct node
{
	elemtype data[SIZE];
	node *next;
}node, array;

void create(array **a, int size)		// 创建
{
	int n = size / SIZE + 1;
	node **cur = a;
	
	while (n--)
	{
		*cur = (node *)malloc(sizeof(node));
		cur = &((*cur)->next);
	}
	*cur = NULL;
}

void destory(array *a)		// 销毁
{
	node *del;
	while (a)
	{
		del = a;
		a = a->next;
		free(del);
	}
}

// 在 pos 处, 插入 data
status insert(array *a, int pos, int data)
{
	int n = pos / SIZE;
	int p = pos % SIZE;
	// 第 n 号结点的, 第 p 号位置
	node *cur = a;

	while (n--) 
	{
		cur = cur->next;
		if (!cur) return ERROR;
	}
	cur->data[p] = data;
	return OK;
}

// 获得一个结点
elemtype getelem(array *a, int pos)
{
	int n = pos / SIZE;
	int p = pos % SIZE;
	node *cur = a;

	while (n--)
	{
		cur = cur->next;
		if (!cur) return -0xFFFFFF;
	}
	return cur->data[p];
}

int main(void)
{
	array *arr = NULL;
	int i, n;
	elemtype data;
	
	printf("建立数组, 输入 n, 及 n 个元素: ");
	scanf("%d", &n);
	create(&arr, n);
	for (i=0; i<n; i++)
	{
		scanf("%d", &data);
		insert(arr, i, data);
	}
	printf("\n遍历数组: ");
	for (i=0; i<n; i++)
		printf("%d, ", getelem(arr, i));
	printf("\n");

	destory(arr);
	return 0;
}