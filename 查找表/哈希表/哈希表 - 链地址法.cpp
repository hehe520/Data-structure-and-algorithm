
// 哈希表解决冲突 = 链地址法

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int status;
typedef int keytype;
#define OK 1
#define ERROR 0
#define FOUND -1
#define R 11		// 取余因子

// 链地址法的哈希表的结构类似邻接表
// 结点结构  [data | next]
typedef struct node
{
	keytype data;
	node *next;
}node;

typedef struct	// 头结构 [下标][first]
{
	node *base[R];
}hashtable;

status create(hashtable *t)		// 建立哈希表
{
	int i;
	if (!t) return ERROR;
	for (i=0; i<R; i++) t->base[i] = NULL;
	return OK;
}

void destory(hashtable *t)		// 销毁哈希表
{
	int i;
	node *del;

	if (!t) return ;
	for (i=0; i<R; i++)
	{
		while (t->base[i])
		{
			del = t->base[i];
			t->base[i] = del->next;
			free(del);
		}
	}
}

int hash(keytype n)		// 哈希函数可自行构造
{ return n % R; }

// 查找一个元素
int search(hashtable *t, keytype data)
{
	int add;
	node *cur;

	add = hash(data);		// 求 data 的哈希值
	cur = t->base[add];
	while (cur)
	{
		if (cur->data == data) return FOUND;
		cur = cur->next;
	}
	return add;
}

status insert(hashtable *t, keytype data)
{
	node *n;
	int add;

	add = search(t, data);

	if (add == FOUND) return ERROR;	// 存在了
	
	n = (node *)malloc(sizeof(node));
	n->data = data;
	// 建立结点, 插到表中
	n->next = t->base[add];
	t->base[add] = n;
	return OK;
}

float ASL(hashtable *t)
{
	node *cur;
	int len = 0, num = 0, i, c;

	for (i=0; i<R; i++)
	{
		cur = t->base[i];
		c = 0;		// c 是查找次数
		while (cur)
		{
			c++;   num++;   len += c;
			cur = cur->next;
		}
	}
	return (float)len / num;
}

void show(hashtable *t)
{
	int i;
	node *cur;
	for (i=0; i<R; i++)
	{
		printf("[%d]", i);
		cur = t->base[i];
		while (cur)
		{
			printf(" -> [%d]", cur->data);
			cur = cur->next;
		}
		printf("\n");
	}
}

int main(void)
{
	hashtable table;
	keytype data;

	create(&table);
	while (1)		// 插入测试
	{
		printf("请输入要查找的元素: ");
		scanf("%d", &data);

		if (insert(&table, data))	// 插入
			printf("\n该元素不存在, 已插入!");
		else printf("\n已找到!");

		printf("\n\n哈希表: \n");   show(&table);
		printf("\nASL = %.3f", ASL(&table));
		printf("\n\n按 c 退出, 按回车继续...\n");
		getchar();
		if (getch() == 'c') break;
		system("cls");
	}
	destory(&table);
	return 0;
}
