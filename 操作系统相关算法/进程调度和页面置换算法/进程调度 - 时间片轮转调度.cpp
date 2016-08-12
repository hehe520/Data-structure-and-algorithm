#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// 交互式系统中的调度, 时间片轮转调度 (抢占式调度)

#define TS 80		// 时间片 (time slice) 80 ms
// 调整时间片大小可观察总耗时, 因为进程切换要花时间
// 时间片过小, 交互性好, 但是 CPU 利用率低, 
// 时间片过大, CPU 利用率高, 但是交互性差

typedef struct		// PCB 进程控制块
{
	char name[30];		// 进程名
	unsigned pid;		// 进程 pid
	long int start_time;
	long int end_time;
	long int service_time;
	long int mission;		// 作业量, 假设一个作业要 1 毫秒
}PCB;


#define SIZE 20		// 队列相关操作
typedef struct
{
	PCB *base[SIZE];
	int rear;
	int front;
}queue;
queue q;		
void initialize(queue *q);
void enqueue(queue *q, PCB *data);
void dequeue(queue *q);
PCB *getfirst(queue *q);


// 创建进程并入队
PCB *createprocess(char *name, int i, int m)
{
	PCB *p;

	p = (PCB *)malloc(sizeof(PCB));
	p->mission = m;
	strcpy(p->name, name);
	p->pid = i;
	p->start_time = clock();
	p->end_time = 0;
	p->service_time = 0;
	enqueue(&q, p);

	return p;
}

inline void run(PCB *tmp)		// 运行中, 每次分 TS 时间
{
	if (tmp->mission >= TS)
	{
		Sleep(TS);
		tmp->mission -= TS;
	}
	else		// 作业量小于时间片, 运行完马上切换
	{
		Sleep(tmp->mission);
		tmp->mission = 0;
	}
}

void show(PCB *p[], int n)
{
	int i, t;		// 周转时间
	float avg;		// 平均周转
	int sum = 0;

	printf("\n进程名\t开始\t结束\t服务\t周转\t平均周转  (时间单位 ms)\n");
	for (i=0; i<n; i++)
	{
		t = p[i]->end_time - p[i]->start_time;
		avg = (float)t / (p[i]->service_time);
		sum += p[i]->end_time;
		printf(" %s\t%d\t%d\t%d\t%d\t%.2f\n", 
			p[i]->name, p[i]->start_time, p[i]->end_time, 
			p[i]->service_time, t, avg);
	}
	printf("\n每个进程平均耗时 %0.2f\n\n", (float)sum / n);
}

int main(void)
{
	PCB *p[10];
	int i, m, t;
	char name[2] = {'A'-1, '\0'};
	int flag;
	
	initialize(&q);
	srand(time(NULL));
	for (i=0; i<10; i++)	// 随机创建 10 个进程入队
	{
		name[0]++;
		m = rand() % 1000 + 500;
		p[i] = createprocess(name, i, m);
	}
	while (1)		// 不停顿地处理进程队列
	{				// 遍历队列元素
		flag = 1;
		for (i=q.front; i!=q.rear; i=(i+1)%SIZE)
		{
			if (q.base[i]->mission > 0)		// 有作业才运行
			{
				printf("切换到进程 %s 运行...\n", q.base[i]->name);
				t = clock();
				run(q.base[i]);			// 运行
				q.base[i]->service_time += clock() - t;
				q.base[i]->end_time += clock();
				flag = 0;
			}
		}
		if (flag) break;		// 全部进程都运行完了
	}
	show(p, 10);
	for (i=0; i<10; i++) free(p[i]);	// 销毁进程
	
	return 0;
}

// 以下是队列相关函数, 可以放入头文件中, 也可用 STL 的队列
void initialize(queue *q)
{q->front = q->rear = 0;}
void enqueue(queue *q, PCB *data)
{
	q->base[q->rear] = data;
	q->rear = (q->rear + 1) % SIZE;
}
void dequeue(queue *q)
{q->front = (q->front + 1) % SIZE;}
PCB *getfirst(queue *q)
{
	if (q->front != q->rear) return q->base[q->front];
	else return NULL;
}