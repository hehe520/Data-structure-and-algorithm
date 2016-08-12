#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;
typedef int keytype;	// 关键字类型

#define ERROR 0
#define OK 1

typedef struct
{
	elemtype *base;
	int lenth;
}table;

status init(table *t)
{
	// 申请 0 号单元作哨兵
	t->lenth = 1;
	t->base = (elemtype *)malloc(sizeof(elemtype));
	if (!t->base) return ERROR;
	return OK;
}

// 在末尾追加一个数据
status append(table *t, elemtype *data)
{
	elemtype *n;

	if (!t || !t->base || !t->lenth) return ERROR;
	
	t->lenth += 1;
	n = (elemtype *)realloc(t->base, t->lenth * sizeof(elemtype));
	if (!n) return ERROR;
	t->base = n;
	t->base[t->lenth - 1] = *data;
	
	return OK;
}

void show(table *t)
{
	int i;
	if (t && t->base)
		for (i=1; i<t->lenth; i++)
			printf("%d, ", t->base[i]);
}

// 顺序查找函数
// 如果找到返回该元素位置, 找不到返回 0 
int search(table *t, elemtype *data)
{
	int i;

	if (!t || !t->base) return 0;
	// 设置哨兵
	t->base[0] = *data;
	for (i=t->lenth-1; i>=0; i--)
		if (t->base[i] == *data) return i;
	return 0;
}

int main(void)
{
	table t;
	keytype tmp;
	int pos;

	init(&t);
	printf("请输入静态查找表: ");
	while (scanf("%d", &tmp) != 0)
		append(&t, &tmp);

	show(&t);
	printf("\n请输入待查找元素：");
	fflush(stdin);
	scanf("%d", &tmp);
	pos = search(&t, &tmp);		// 顺序查找函数
	if (pos) 
		printf("找到，位置：%d， 数据：%d\n", pos, t.base[pos]);
	else printf("未找到\n");

	return 0;
}