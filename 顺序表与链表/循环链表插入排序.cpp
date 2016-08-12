// 循环链表排序，
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
}node;  

status create(node *p, int num)
{
	node *n;

	p->next = p;
	num--;
	while (num--)
	{
		n = (node *)malloc(sizeof(node));
		if (!n) return ERROR;
		n->next = p->next;
		p->next = n;
	}
	return OK;
}

status input(node *p)
{
	node *cur;

	printf("Enter num:");
	scanf("%d", &p->data);
	cur = p->next;
	while (cur != p)
	{
		printf("Enter num:");
		scanf("%d", &cur->data);
		cur = cur->next;
	}
	return OK;
}

void sort(node *p)
{
	node *cur;
	int flag;
	elemtype t;

	while (1)
	{
		// 跑一圈，冒泡
		flag = 1;
		cur = p;
		while (cur->next != p)
		{
			if (cur->data > cur->next->data)
			{
				t = cur->data;
				cur->data = cur->next->data;
				cur->next->data = t;
				flag = 0;
			}
			cur = cur->next;
		}
		if (flag) break; 
	}
}

void show(node *p)
{
	node *cur; 
	cur = p;
	
	while (cur->next != p)
	{
		printf("%d, ", cur->data);
		cur = cur->next;
	}
	printf("%d\n", cur->data);
}

// 插入后自动排序
status insert(node *p, elemtype *data)
{
	node *cur;
	node *n;
	elemtype t;
	
	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;
	n->data = *data;
	cur = p;
	// 如果比头结点还小
	// 就插在头结点的位置
	if (*data < cur->data && cur == p)
	{
		n->next = cur->next;
		cur->next = n;
		// [p] -> [n]
		// n 本来应该插在 p 的前面
		// 但是 p 是头结点，要兜一个大圈才能在p后面 O(n)
		// 所以，先插在 P 后面，
		// 然后，两变量交换 O(1)
		t = p->data;
		p->data = n->data;
		n->data = t;
	}
	else
	{
		// 1 2 3  data == 10
		// 跑到最后还没发现比我大的，
		// 就会死循环, 所以，只跑一圈
		while (*data > cur->data && cur->next!=p) cur = cur->next;
		// 插在 cur 后
		n->next = cur->next;
		cur->next = n;
	}
	return OK;
}

int main(void)
{ 
	node list;
	int times = 5;
	elemtype data;
  
	// 不带头，
 	create(&list, 10);
	input(&list);
	show(&list);
	sort(&list);
	show(&list);
	while (times--)
	{
		scanf("%d", &data);
		insert(&list, &data);
		show(&list);
	}

	return 0;
}
