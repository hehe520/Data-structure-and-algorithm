// 链式队列
#include <stdio.h>
#include <malloc.h>

typedef int status;
typedef int elemtype;

#define OK 1
#define ERROR 0

// 结点结构
typedef struct node 
{
	elemtype data;
	node *next;
}node;

// 队列头结构
typedef struct queue
{
	node *front;
	node *rear;
	int lenth;
}queue;

// 单链表结构  [front] -> [] -> [] -> [rear]

// 创建时创建一个空结点,来标志空队列
// if front == rear 表示空队列
status create(queue *p)
{
	p->rear = (node *)malloc(sizeof(node));
	if (!p->rear) return ERROR;
	p->rear->next = NULL;		// 安全性
	p->front = p->rear;
	p->lenth = 0;

	return OK;
}

status destory(queue *p)
{
	node *del;

	while (p->front)
	{
		del = p->front;
		p->front = del->next;
		free(del);
	}
	p->rear = p->front = NULL;
	p->lenth = 0;

	return OK;
}

status in(queue *p, elemtype *data)
{
	node *n;

	// 创建结点,赋值,连上去,rear++,计数器++
	if (!p->rear) return ERROR;
	n = (node *)malloc(sizeof(node));
	if (!n) return ERROR;

	p->rear->data = *data;		// 放入元素
	n->next = p->rear->next;	// 插入结点
	p->rear->next = n;
	p->rear = n;		// 把 rear 移到 n 的位置,rear++
	p->lenth++;

	return OK;
}

// 第二种入队方法,少用一个变量
status in2(queue *p, elemtype *data)
{
	if (!p->rear) return ERROR;

	p->rear->data = *data;		// 赋值, 创建结点, 再下移一位
	p->rear->next = (node *)malloc(sizeof(node));
	if (!p->rear->next) return ERROR;

	p->rear = p->rear->next;	// 相当于 rear++
	p->rear->next = NULL;		// 安全性,可以不要
	p->lenth++;

	return OK;
}

status out(queue *p, elemtype *back)
{
	node *del;

	if (!p->rear) return ERROR;
	// 空队列
	if (p->rear == p->front) return ERROR;

	// 先下移一位,取出数据,释放结点,front++,计数器--
	del = p->front;
	p->front = del->next;
	*back = del->data;
	free(del);
	p->lenth--;

	return OK;
}

int lenth(queue *p)
{
	return p->lenth;
}

status onlyshow(queue *p)
{
	node *cur;

	if (!p->rear) return ERROR;
	// 空队列
	if (p->rear == p->front) return ERROR;

	cur = p->front;
	while (cur != p->rear)
	{
		printf("%d, ", cur->data);
		cur = cur->next;
	}
	printf("\n");

	return OK;
}

status gethead(queue *p, elemtype *back)
{
	if (!p->rear) return ERROR;
	if (p->rear == p->front) return ERROR;

	*back = p->front->data;

	return OK;
}

int main(void)
{
	queue q;
	elemtype data;
	int i;
	int times = 10;

	create(&q);
	// 入队
	for (i=1; i<=times; i++)
	{
		if (!in2(&q, &i)) break;
		printf("入队元素: %d\n", i);
	}

	// 显示队长
	printf("\n队长: %d\n", lenth(&q));
	gethead(&q, &data);
	printf("队头元素: %d\n", data);
	printf("遍历队列: ");
	onlyshow(&q);
	printf("\n");

	// 出队
	for (i=1; i<=times; i++)
	{
		if (!out(&q, &data)) break;
		printf("出队元素: %d\n", data);
	}

	destory(&q);

	return 0;
}