#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;
typedef int keytype;

#define OK 1
#define ERROR 0

// 单链表结点结构
typedef struct node
{
	elemtype data;
	node *next;
}node;

// 头结构
typedef struct
{
	node *first;
	node *last;
	int lenth;
}linklist;


// 链表的末尾做哨兵，则每次插在链表的倒数第二位
status init(linklist *t)
{
	// 申请一个空间放哨兵
	t->first = (node *)malloc(sizeof(node));
	if (!t->first) return ERROR;
	t->first->next = NULL;
	t->last = t->first;
	t->lenth = 1;
	return OK;
}

status destory(linklist *t)
{
	node *cur;
	if (!t) return ERROR;
	
	cur = t->first;
	while (cur != NULL)
	{
		t->first = cur->next;
		free(cur);
		cur = t->first;
	}
	t->first = NULL;
	t->last  = NULL;
	t->lenth = 0;
	return OK;
}

void show(linklist *t)
{
	node *cur;
	if (t)
	{
		cur = t->first;
		while (cur != t->last)
		{
			printf("%d, ", cur->data);
			cur = cur->next;
		}
	}
}

// 每次追加到链表的倒数第二位
// 因为最后一位是哨兵
status append(linklist *t, elemtype *data)
{
	node *n;

	if (!t) return ERROR;

	// 采用交换插入，插在后面，前后交换
	// 创建新结点, 并插入
	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	// n 尾插法
	n->next = NULL;
	t->last->data = *data;
	t->last->next = n;
	t->last = n;
	t->lenth++;

	return OK;
}

// 顺序查找函数
// 找到返回位序，找不到返回 0
int search(linklist *t, elemtype *data)
{
	node *cur;
	int pos = 0;

	if (!t) return ERROR;
	// 设置哨兵
	t->last->data = *data;
	cur = t->first;
	while (cur)
	{
		pos++;
		if (cur->data == *data) return pos;
		cur = cur->next;
	}
	return t->lenth;
}

int main(void)
{
	linklist list;
	elemtype tmp;
	int pos;

	init(&list);
	printf("输入链表：");
	while (scanf("%d", &tmp) != 0)
		append(&list, &tmp);	// 追加到末尾
	show(&list);
	printf("\n请输入待查找的元素：");
	fflush(stdin);
	scanf("%d", &tmp);
	pos = search(&list, &tmp);		// 顺序查找函数
	if (pos != list.lenth) 
		printf("找到：位置：%d, 数据：%d\n", pos, tmp);
	else printf("未找到\n");

	destory(&list);
	return 0;
}
