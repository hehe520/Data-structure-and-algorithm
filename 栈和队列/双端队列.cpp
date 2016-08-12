
// 双端队列
// push_front()		前端入队
// pop_front()		前端出队
// push_rear()		后端入队
// pop_rear()		后端出队
// get_front()		获得队头
// get_rear()		获得队尾

#include <stdio.h>

typedef int elemtype;
#define NUM 50		// 队列大小
#define INC(a) ((a + 1) % NUM)
#define DEC(a) ((a - 1 + NUM) % NUM)

// 双端队列结构: 
//  ...... 0 1 2 3 4 5 ..........
//         |           |
//       front        rear

typedef struct
{
	elemtype base[NUM];
	int front;		// 队头
	int rear;		// 队尾
}queue;

void init(queue *q)
{
	q->front = q->rear = 0;
}

// 前端压入 front--
int push_front(queue *q, elemtype data)
{
	if (DEC(q->front) == q->rear) return 0;	// 满了
	q->front = DEC(q->front);
	q->base[q->front] = data;

	return 1;
}

// 前端弹出 front++
int pop_front(queue *q)
{
	if (q->front == q->rear) return 0;	// 空
	q->front = INC(q->front);

	return 1;
}

// 获得队头
elemtype get_front(queue *q)
{
	if (q->front == q->rear) return -1;
	return q->base[q->front];
}

// 后端入队 rear++
int push_rear(queue *q, elemtype data)
{
	if (INC(q->rear) == q->front) return 0;
	q->base[q->rear] = data;
	q->rear = INC(q->rear);

	return 1;
}

// 后端出队 rear--
int pop_rear(queue *q)
{
	if (q->rear == q->front) return 0;
	q->rear = DEC(q->rear);

	return 1;
}

// 获得队尾 
elemtype get_rear(queue *q)
{
	if (q->rear == q->front) return -1;
	return q->base[DEC(q->rear)];
}

int main(void)
{
	queue q;
	int i, tmp;

	init(&q);
	printf("\n后端压入: ");
	for (i=0; i<55; i++)
	{
		if (push_rear(&q, i)) printf("%d, ", i);
		else printf("满, ");
	}
	printf("\n\n前端弹出: ");
	for (i=0; i<55; i++)
	{
		if ((tmp = get_front(&q)) != -1) 
		{
			pop_front(&q);
			printf("%d, ", tmp);
		}
		else printf("空, ");
	}
	printf("\n\n前端压入: ");
	for (i=0; i<55; i++)
	{
		if (push_front(&q, i)) printf("%d, ", i);
		else printf("满, ");
	}
	printf("\n\n后端弹出: ");
	for (i=0; i<55; i++)
	{
		if ((tmp = get_rear(&q)) != -1) 
		{
			pop_rear(&q);
			printf("%d, ", tmp);
		}
		else printf("空, ");
	}
	return 0;
}