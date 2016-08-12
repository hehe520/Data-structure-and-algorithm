#include <stdio.h>
#include <malloc.h>

typedef int elemtype;
typedef int status;

#define OK 1
#define ERROR 0
#define SIZE 20		// 大小已固定

// 循环队列 顺序结构储存
typedef struct 
{
	elemtype *base;
	int front;
	int rear;
}queue;

status create(queue *p)
{
	p->base = (elemtype *)malloc(SIZE*sizeof(elemtype));
	if (!p->base) return ERROR;
	p->front = p->rear = 0;

	return OK;
}

status destory(queue *p)
{
	if (p->base)
	{
		free(p->base);
		p->front = p->rear = 0;

		return OK;
	}
	else return ERROR;
}

status clear(queue *p)
{
	p->front = p->rear = 0;
	return OK;
}

status enqueue(queue *p, elemtype *data)
{
	// 没建好的队列不能入
	if (!p->base) return ERROR;

	// 看看队列满了没有
	if ((p->rear+1) % SIZE == p->front) return ERROR;

	p->base[p->rear] = *data;
	p->rear = (p->rear + 1) % SIZE;

	return OK;
}

status dequeue(queue *p, elemtype *back)
{
	// 没建好的队列不能处、出
	if (!p->base) return ERROR;

	// 看看是不是空队列
	if (p->rear == p->front) return ERROR;

	*back = p->base[p->front];
	p->front = (p->front + 1) % SIZE;

	return OK;
}

int lenth(queue *p)
{
	if (!p->base) return 0;
	return (p->rear - p->front + SIZE) % SIZE;
	//  0 1 2 3 4 5 
	//  rear  front 
}

// 避免考虑循环的细节，
// inc 将front rear 下移一位
void inc(int *pos)
{
	*pos = (*pos + 1) % SIZE;
}

status onlyshow(queue *p)
{
	int i;

	if (!p->base) return ERROR;
	if (p->rear == p->front) return ERROR;
	
	// 0 1 2 3 
	i = p->front;
	while ( i % SIZE != p->rear )
	{
		printf("%d, ", p->base[i]);
		inc(&i);
	}
	// 此时 i % SIZE == p->rear

	return OK;
}

status gethead(queue *p, elemtype *back)
{
	if (!p->base) return ERROR;
	if (p->rear == p->front) return ERROR;

	*back = p->base[ p->front ];

	return OK;
}

int main(void)
{
	queue q;
	elemtype data;
	int i;
	int times = 10;

	create(&q);
	// 5 个入队
	for (i=1; i<=times; i++)
	{
		if (!enqueue(&q, &i)) break;
		printf("入队元素：%d\n", i);
	}

	printf("\n队长：%d\n", lenth(&q));
	gethead(&q, &data);
	printf("队头元素: %d\n", data);
	printf("遍历队列：");
//	clear(&q);
	onlyshow(&q);
	getchar();

	// 出队
	for (i=1; i<=times; i++)
	{
		if (!dequeue(&q, &data)) break;
		printf("出队元素：%d\n", data);
	}
	destory(&q);

	return 0;
}
