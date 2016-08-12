// 顺序表，链表，互转
#include <iostream>
#include <stdlib.h>

typedef int status;
typedef int elemtype;

#define SIZE  10
#define INC   10
#define OK    1
#define ERROR 0

// 顺序表储存结构
typedef struct 
{
	elemtype *base;
	int length;
	int size;
}list;

// 单链表储存结构
typedef struct node
{
	elemtype data;
	node *next;
}node;

// --------顺序表函数---------
status createlist(list *p)
{
	p->base = (elemtype *)malloc(SIZE*sizeof(elemtype));
	if (!(p->base)) return ERROR;
	p->size = SIZE;
	p->length = 0;

	return OK;
}

status destorylist(list *p)
{
	if (p->base != NULL) free(p->base);  // 有问题<-------
	else return ERROR;

	p->base = NULL;
	p->length = p->size = 0;

	return OK;
}

status insert(list *p, int pos, elemtype *data)
{
	elemtype *end;
	elemtype *now;
	elemtype *cur;
	elemtype *n;
	int newsize;

	// 1 2 3 , lenth==3
	if (pos >= 1 && pos <= p->length+1)
	{
		// 满了没
		if (p->length == p->size)
		{
			newsize = p->size + SIZE;
			n = (elemtype *)malloc(newsize* sizeof(elemtype));
			if (!n) return ERROR;
			p->base = n;
			p->size = newsize;
		}
		// 1 2 3 , pos = 2
		end = p->base + p->length - 1;
		now = p->base + pos - 1;
		for (cur=end; cur>=now; cur--)
		{
			*(cur+1) = *cur;
		}
		*now = *data;
		p->length++;

		return OK;
	}
	else return ERROR;
}

status showlist(list *p)
{
	elemtype *cur;
	elemtype *end;

	if (p->length == 0) return ERROR;
	end = p->base + p->length - 1;
	for (cur=p->base; cur<=end; cur++)
	{
		printf("%d, ", *cur);
	}
	printf("\n");
	return OK;
}

// --------链表函数-----------
// 初始化链表
status initnode(node *p)
{
	p->next = NULL;
	p->data = 0;

	return OK;
}

status shownode(node *p)
{
	node *cur;

	if (p->next == NULL) return ERROR;
	cur = p->next;
	while (cur)
	{
		printf("%d, ", cur->data);
		cur = cur->next;
	}
	printf("\n");

	return OK;
}

status destorynode(node *p)
{
	node *del;
	
	if (p->next == NULL) return ERROR;
	while (p->next)
	{
		del = p->next;
		p->next = p->next->next;
		free(del);
	}
	p->next = NULL;
	p->data = 0;

	return OK;
}

// 追加到 cur 的后面
status append(node *p, elemtype *data)
{
	node *cur;
	node *n;

	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->data = *data;

	cur = p;
	while (cur->next) cur = cur->next;
	n->next = cur->next;
	cur->next = n;
	p->data++;

	return OK;
}

// --------转换函数-----------
// listtonode 遍历顺序表拿元素
// 然后不断追加到 链表中
status listtonode(list *a, node *b)
{
	elemtype *cur;
	elemtype *end;

	// 检查表的状态
	if (a->base == NULL) return ERROR;
	if (a->length == 0)  return ERROR;
	
	end = a->base + a->length - 1;
	for (cur=a->base; cur<=end; cur++)
	{
		// 把 cur 追加到链表 b 中
		if (append(b, cur) == ERROR) break;
	}
	// 看看是不是全部转完了
	if (cur > end) return OK;
	else return ERROR;
}

// nodetolist 遍历链表拿元素
// 然后插入到第 i++ 个位置
status nodetolist(node *p, list *b)
{
	node *cur;
	int pos = 1;

	// 看看是不是空表
	if (p->data == 0) return ERROR;
	// 顺序表在插入时会自动扩展大小
	
	cur = p->next;
	while (cur)
	{
		if (insert(b, pos++, &(cur->data)))
		{
			cur = cur->next;
		}
		else break;
	}
	if (cur) return ERROR;
	else return OK;
}

// ---------main--------------
int main(void)
{
	list a;
	node b;
	elemtype data;
	int num = 5;
	int i;
	
	createlist(&a);
	initnode(&b);
	printf("--- 顺序表转链表:\n");
	printf("--- 向顺序表中输入 %d 个元素\n", num);
	for (i=1; i<=num; i++)
	{
		printf("第 %d 个：", i);
		scanf("%d", &data);
		insert(&a, i, &data);
	}
	printf("表中元素：");
	showlist(&a);
	if (listtonode(&a, &b)) 
	{
		printf("--- 成功转为链表！\n");
		shownode(&b);
	}
	else printf("转换失败！\n");
	destorylist(&a);
	destorynode(&b);

	fflush(stdin);
	getchar();
	system("cls");

	//----------------------------------
	createlist(&a);
	initnode(&b);
	printf("--- 链表转顺序表:\n");
	printf("向链表中输入 %d 个元素", num);
	for (i=1; i<=num; i++)
	{
		printf("第 %d 个：", i);
		scanf("%d", &data);
		append(&b, &data);	// 末尾追加
	}
	printf("表中元素：");
	shownode(&b);

	if (nodetolist(&b, &a)) 
	{
		printf("--- 成功转为顺序表！\n");
		showlist(&a);
	}
	else printf("转换失败！\n");
	destorylist(&a);
	destorynode(&b);

	fflush(stdin);
	getchar();

	return 0;
}
