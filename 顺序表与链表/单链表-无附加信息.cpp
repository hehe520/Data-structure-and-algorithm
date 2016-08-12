#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK         1
#define ERROR      0
#define OVERFLOW  -2

typedef struct node
{
	elemtype data;
	node *next;
}node;

// 创建单链表,不赋值
status createlist(node *p, int num)
{	
	node *newbase = NULL;
	int i;

	// 初始化链表
	p->next = NULL;

	// 开始创建结点
	for (i=0; i<num; i++)
	{
		newbase = (node *)malloc(sizeof(node));
		if (newbase)
		{
			newbase->next = p->next;
			p->next = newbase;
			p->data++;
			newbase = NULL;
		}
		else return ERROR;
	}
	return OK;
}

// 释放单链表
status destorylist(node *p)
{
	node *cur;
	node *pos;

	// [head]   [delete]    [3 th] 
	// cur      cur->next   cur->next->next
	while (p->next)
	{
		cur = p;
		pos = cur->next;  // 待删除的结点
		cur = pos->next;  // cur->next->next 也可
		p->next = cur;
		free(pos);
	}
	p->data = 0;
	return OK;
}

// 修改第N个结点的数据 
status update(node *p, int i, elemtype *data)
{
	node *cur;
	int j = 0;

	// i 不能是负数
	if (i>=1)
	{
		cur = p;
		// 如果 下一个不是 NULL
		// 还没到 i 处
		while (cur->next && j<i)
		{
			cur = cur->next;
			j++;
			// 若 j++ 后等于 i 了 j<i 就不成立了
		}
		// 到了 i 处
		if (j == i)
		{
			cur->data = *data;
			return OK;
		}
		else return ERROR;
	}
	else return ERROR;
}

// 插入结点
status insert(node *p, int i, elemtype *data)
{
	node *newbase;
	node *cur;
	int j = 0;
	
	// [1] -> [2] -> [3]...
	// 在 i 处插入结点,先要到 i-1 处
	cur = p;
	while (cur->next && j<i-1)
	{
		cur = cur->next;
		j++;
	}
	
	if (j == i-1)
	{
		newbase = (node *)malloc(sizeof(node));
		if (newbase)
		{
			newbase->next = cur->next;
			cur->next = newbase;
			newbase->data = *data;
			return OK;
		}
		else return ERROR;
	}
	else return ERROR;
}

// 删除一个结点
status remove(node *p, int i)
{
	node *del;
	node *cur;
	int j = 0;
	
	//先去到 i-1 的结点
	cur = p;
	while (cur->next && j<i-1)
	{
		cur = cur->next;
		j++;
	}
	if (j == i-1)
	{
		del = cur->next;  // 待删除的结点
		cur->next = del->next;
		free(del);
		return OK;
	}
	else return ERROR;
}

// 获得一个结点的数据
status getnode(node *p, int i, elemtype *back)
{
	node *cur = p;
	int j = 0;

	cur = p;
	while (cur->next && j<i)
	{
		cur = cur->next;
		j++;
	}

	if (j == i)
	{
		*back = cur->data;
		return OK;
	}
	else return ERROR;
}

// 获得链表的长度
int listlenth(node *p)
{
	node *cur;
	int i = 0;

	// 附加信息有长度的话
	// 直接 return p->data
	// 下面试验没有的情况
	cur = p;
	while (cur->next) i++;
	return i;
}

void shownode(node *p)
{
	node *cur;
	
	cur = p;
	cur = cur->next;
	while (cur)
	{
		printf("%-4d", cur->data);
		cur = cur->next;
	}
	printf("\n");
}

int main(void)
{
	node list;
	int i;
	int lenth;
	elemtype data;

	printf("请输入新建链表的长度：");
	scanf("%d", &lenth);
	createlist(&list, lenth);

	for (i=0; i<lenth; i++)
	{
		printf("请输入第 %d 个数据：",i+1);
		scanf("%d",&data);
		update(&list, i+1, &data);
	}
	shownode(&list);

	printf("请输入插入的[位置]和[数据]:");
	scanf("%d%d", &i, &data);

	if (insert(&list, i, &data) == ERROR) 
		printf("插入失败！\n");
	shownode(&list);

	printf("请输入删除结点的位置:");
	scanf("%d", &i);
	if (remove(&list, i) == ERROR)
		printf("删除失败！\n");
	shownode(&list);

	printf("请输入访问结点的位置：");
	scanf("%d", &i);
	if (getnode(&list, i, &data)) printf("数据为：%d\n",data);
	else printf("访问失败!\n");

	destorylist(&list);

	return 0;
}