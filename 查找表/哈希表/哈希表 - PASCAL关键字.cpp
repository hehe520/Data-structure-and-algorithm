
// 哈希表 - PASCAL关键字
// 采用链地址法, 解决冲突

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef char keytype;	// 采用非数值数据
typedef int status;
#define OK 1
#define ERROR 0
#define FOUND -1
#define R 13		// 取余因子, 可以调大一点

typedef struct node		// 结点结构
{
	keytype *str;	// 字符串地址
	node *next;
}node;

typedef struct		// 哈希表结构
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
			if (del->str) free(del->str);
			free(del);
		}
	}
}

// 首位字符 ASCLL 码相加,再对 R 取余, 得到 hash 值
int hash(keytype *data)
{
	return (data[0] + data[strlen(data)-1]) % R;
}

// 查找一个关键字
status search(hashtable *t, keytype *data, int *pos)
{
	node *cur;
	
	*pos = hash(data);		// 计算 hash 值
	cur = t->base[*pos];
	while (cur)			// 类似遍历邻接点
	{
		if (cur->str && !strcmp(data, cur->str)) 
			return OK;
		cur = cur->next;
	}
	return ERROR;
}

// 插入一个关键字
status insert(hashtable *t, keytype *data)
{
	int len = strlen(data);
	int add;
	node *n;

	if (search(t, data, &add)) return ERROR;
	n = (node *)malloc(sizeof(node));
	n->str = (keytype *)malloc((len + 1) * sizeof(keytype));
	strcpy(n->str, data);

	n->next = t->base[add];		// 建立新结点, 并插入
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
			printf(" -> [%s]", cur->str);
			cur = cur->next;
		}
		printf("\n");
	}
}

int main(void)
{
	hashtable table;
	keytype data[50];

	create(&table);
	while (1)		// 插入测试
	{
		printf("请输入要查找的元素: ");
		scanf("%s", &data);
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
/* 
// 测试数据, 部分 PASCAL 语言关键字, 
// 用 C 语言也是可以的, 是字符串就行
and

array

as

or

asm

begin

end

for

while

case

cdecl

const

dynamic

if

else

*/