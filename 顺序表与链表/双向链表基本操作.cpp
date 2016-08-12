#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK 1
#define ERROR 0

typedef struct node
{
	elemtype data;
	node *next;
	node *prior;
}node;

// 创建双链表
status createlist(node *p, int size)
{
	node *n;
	int i;

	p->next = p;
	p->prior = p;
	p->data = 0;	// 记录长度
	for (i=0; i<size; i++)
	{
		n = (node *)malloc(sizeof(node));
		if (!n) return ERROR;

		//  [][p][]   [][2][]
		//       [][n][]
		n->next = p->next;
		n->prior = p;
		p->next->prior = n;
		p->next = n;

		p->data++;
	}
	return OK;
}

// 更新第I个数据
status update(node *p, int pos, elemtype *data)
{
	int j;
	node *cur;

	// 1 2 3  lenth == 3
	if (pos>=1 && pos<=p->data)
	{
		cur = p->next;
		j = 1;
		while (j < pos)
		{
			cur = cur->next;
			j++;
		}
		// 此时 j == pos
		cur->data = *data;

		return OK;
	}
	else return ERROR;
}

void show(node *p)
{
	node *cur;

	cur = p->next;
	while (cur != p)
	{
		printf(" %d, ", cur->data);
		cur = cur->next;
	}
	printf("\n");
}

int lenth(node *p)
{
	node *cur;
	int count = 0;

	cur = p->next;
	while (cur != p)
	{
		count++;
		cur = cur->next;
	}

	return count;
}

status insert(node *p, int pos, elemtype *data)
{
	node *n;
	node *cur;
	int j;

	// 1 2 3  data = 3
	if (pos>=1 && pos<=p->data+1)
	{
		n = (node *)malloc(sizeof(node));
		if (!n) return ERROR;
		cur = p->next;
		j = 1;
		while (j < pos)
		{
			cur = cur->next;
			j++;
		}
		// 插入到 cur的前面
		n->next = cur;
		n->prior = cur->prior;
		cur->prior->next = n;
		cur->prior = n;

		n->data = *data;
		p->data++;
		return OK;
	}
	else return ERROR;
}

status remove(node *p, int pos)
{
	node *cur;
	int j;

	if (pos>=1 && pos<=p->data)
	{
		cur = p->next;
		j = 1;
		while (j < pos)
		{
			cur = cur->next;
			j++;
		}
		cur->prior->next = cur->next;
		cur->next->prior = cur->prior;
		free(cur);
		p->data--;
		return OK;
	}
	else return ERROR;
}

status getelem(node *p, int pos, elemtype *back)
{
	node *cur;
	int j;

	if (pos>=1 && pos<=p->data)
	{
		cur = p->next;
		j = 1;
		while (j < pos)
		{
			cur = cur->next;
			j++;
		}
		*back = cur->data;
		return OK;
	}
	else return ERROR;
}

status clearlist(node *p)
{
	node *cur;
	
	if (p->data == 0) return ERROR;
	cur = p->next;
	while (cur != p)
	{
		cur->data = 0;
		cur = cur->next;
	}
	return OK;
}

void destory(node *p)
{
	node *cur;

	cur = p->next;
	// [][p][]  [][cur][]  [][2][]
	while (cur != p)
	{
		cur->next->prior = cur->prior;
		cur->prior->next = cur->next;
		free(cur);
		cur = p->next;
	}
}

int main(void)
{
	node list;
	elemtype data;
	int num, i;

	printf("请输入双向链表的初始长度:");
	scanf("%d", &num);
	createlist(&list, num);
	for (i=1; i<=num; i++)
	{
		printf("请输入第 %d 个值：", i);
		scanf("%d", &data);
		update(&list, i, &data);
	}
	show(&list);
	printf("请输入插入的数据和位置:");
	scanf("%d%d", &data, &i);
	if (!insert(&list, i, &data)) 
		printf("插入失败！位置错误！\n");
	show(&list);

	printf("请输入要删除的位置:");
	scanf("%d", &i);
	if (!remove(&list, i)) 
		printf("删除失败！位置错误！\n");
	show(&list);

	printf("请输入要访问的位置:");
	scanf("%d", &i);
	if (!getelem(&list, i, &data)) 
		printf("访问失败！位置错误！\n");
	printf("该元素：%d\n", data);
	
	printf("重置所有元素，");
	clearlist(&list);
	show(&list);

	destory(&list);

	return 0;
}
