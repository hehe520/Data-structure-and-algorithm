#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// 批处理系统中的调度, 非抢占式调度
// 最高响应比优先, 用进程的等待时间除以服务时间, 得到响应比
// 每次处理一个响应比最高的进程

// PCB 进程控制块
typedef struct 
{
	char name[30];		// 进程名
	unsigned pid;		// 进程 pid
	long int start_time;
	long int end_time;
	long int service_time;
	long int mission;		// 任务量, 假设一个任务要 1 毫秒
}PCB;

// 创建进程并入队
PCB *createprocess(char *name, int i, int m)
{
	PCB *p;

	p = (PCB *)malloc(sizeof(PCB));
	p->mission = m;
	strcpy(p->name, name);
	p->pid = i;
	p->start_time = clock();

	return p;
}

void run(PCB *tmp)	// 运行中
{
	Sleep(tmp->mission);
	tmp->mission = 0;
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
	int i, j, m, t;
	char name[2] = {'A'-1, '\0'};
	PCB *tmp;
	int pos = 0;
	float xiangying, max;	// 响应比
	long int wait_time;
	
	srand(time(NULL));
	for (i=0; i<10; i++)	// 随机创建 10 个进程入队
	{
		name[0]++;
		m = rand() % 1000 + 500;
		p[i] = createprocess(name, i, m);
		Sleep(1);
	}
	for (i=0; i<10; i++)		// 处理进程
	{
		// 遍历队列中的元素, 找出一个最高响应比的进程
		max = -1;
		for (j=0; j<10; j++)
		{
			if (p[j]->mission != 0)
			{	// 计算响应比, = 等待时间 / 服务时间
				wait_time = clock() - p[j]->start_time;
				xiangying = (float)wait_time / p[j]->service_time;
				if (xiangying > max) { max = xiangying;  pos = j; }
			}
		}
		tmp = p[pos];
		printf("进程 %s 运行中...\n", tmp->name);
		t = clock();		// 开始处理时间
		run(tmp);
		tmp->service_time = clock() - t;
		tmp->end_time = clock();
	}
	show(p, 10);
	for (i=0; i<10; i++) free(p[i]);	// 销毁进程
	
	return 0;
}
