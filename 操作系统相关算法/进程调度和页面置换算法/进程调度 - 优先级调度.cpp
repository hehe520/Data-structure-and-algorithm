#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// 交互式系统中的调度,优先级调度 (抢占式调度)
// main 函数只处理优先级最高的队列中的进程, 处理完后, 降低其级数
// adjust 函数就把等待过久的进程提高一个优先级
// 例如每等待 80 ms 就把所有进程提高一个级数
// 即队列外部用优先级调度, 而内部用轮转调度

// 由于 main 与 adjust 都要访问队列, 于是队列成了临界区
// 同时访问会出现问题, 例如 main 要入队, 还没来得及把 rear 加一
// 切换到 adjust, 它也要入队, 于是把 main 的给覆盖掉了
// 用信号量 mutex 控制访问

#define TS 80		// 时间片 (time slice) 80 ms
HANDLE mutex;

typedef struct		// PCB 进程控制块
{
	char name[30];		// 进程名
	unsigned int pid;	// 进程 pid
	unsigned int p;		// 优先级
	long int start_time;
	long int end_time;
	long int service_time;
	long int mission;		// 作业量, 假设一个作业要 1 毫秒
}PCB;

#define JI 3		// 多级队列的级数
#define SIZE 20	
typedef struct
{
	PCB *base[SIZE];
	int rear;
	int front;
}queue;
queue q[JI];	// 多级队列, q[0] 优先级最高
void initialize(queue *q);
void enqueue(queue *q, PCB *data);
void dequeue(queue *q);
PCB *getfirst(queue *q);
int empty(queue *q);


// 创建进程并入队
PCB *createprocess(char *name, int i, int m)
{
	PCB *p;

	p = (PCB *)malloc(sizeof(PCB));
	p->mission = m;
	p->p = rand() % JI;		// 随机产生一个优先级
	strcpy(p->name, name);
	p->pid = i;
	p->start_time = clock();
	p->end_time = 0;
	p->service_time = 0;
	// 按照优先级入队, q[0], q[1], q[2]
	enqueue(q + p->p, p);

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

// 每个 50 ms, 提高等待过久的队列的优先级 
DWORD WINAPI adjust(void *id)
{
	int i;

	Sleep(50);
	while (1)		// 不断的搬运
	{
		WaitForSingleObject(mutex, INFINITE);
		for (i=1; i<JI; i++)
		{
			while (!empty(q + i))
			{
				// 把 i 级的元素搬到 0 级
//				printf("正在搬运 %d 到 %d 级队列...\n", i, i-1);
				enqueue(q + i - 1, getfirst(q + i));
				dequeue(q + i);
			}
		}
		ReleaseSemaphore(mutex, 1, NULL);
	}
	return 0;
}

int main(void)
{
	PCB *p[10];
	PCB *tmp;
	int i, m, t;
	char name[2] = {'A'-1, '\0'};
	int flag;
	HANDLE ad;
	
	mutex = CreateSemaphore(NULL, 1, 2, "queue");
	for (i=0; i<JI; i++)
		initialize(q + i);
	srand(time(NULL));
	for (i=0; i<10; i++)	// 随机创建 10 个进程入队
	{
		name[0]++;
		m = rand() % 1300 + 200;
		p[i] = createprocess(name, i, m);
	}
	ad = CreateThread(NULL, 0, adjust, NULL, 0, NULL);

	while (1)		// 不停顿地处理进程队列 q[0]
	{				// 遍历队列元素
		WaitForSingleObject(mutex, INFINITE);	// 尝试进入临界区
		while (!empty(q + 0))
		{
			tmp = getfirst(q + 0);
			dequeue(q + 0);
			if (tmp->mission > 0)		// 有作业才运行
			{
				printf("切换到进程 %s 运行...\n", tmp->name);
				t = clock();
				run(tmp);			// 运行
				tmp->service_time += clock() - t;
				tmp->end_time += clock();
				enqueue(q + JI - 1, tmp);	// 处理完了, 降级
			}
		}
		ReleaseSemaphore(mutex, 1, NULL);
		flag = 1;
		for (i=0; i<JI; i++)
			if (!empty(q + i)) flag = 0;
		if (flag) break;
	}
	show(p, 10);
	for (i=0; i<10; i++) free(p[i]);	// 销毁进程
	CloseHandle(mutex);
	CloseHandle(ad);
	
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
int empty(queue *q)
{return !(q->rear - q->front);}
