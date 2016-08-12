#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

// 批处理系统中的调度, 非抢占式调度
// 最短作业优先, 先把任务量排序, 每次拿最短的作业

// PCB 进程控制块
typedef struct 
{
	char name[30];		// 进程名
	unsigned int pid;
	long int start_time;
	long int end_time;
	long int service_time;
	long int mission;		// 作业量, 假设一个作业要 1 毫秒
}PCB;

// 创建进程
PCB *createprocess(char *name, int i, int m)
{
	PCB *p;

	p = (PCB *)malloc(sizeof(PCB));
	p->mission = m;
	strcpy(p->name, name);
	p->pid = i;
	p->start_time = 0;
	// 最短作业要求作业同时到达

	return p;
}

void sort(PCB **p, int n);		// 排序

void run(PCB *t)	// 运行中
{
	Sleep(t->mission);
	t->mission = 0;
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
	
	srand(time(NULL));
	for (i=0; i<10; i++)	// 随机创建 10 个进程入队
	{
		name[0]++;
		m = rand() % 1000 + 500;
		p[i] = createprocess(name, i, m);
		Sleep(1);
	}
	sort(p, 10);		// 从小到大按照作业量排序
	for (i=0; i<10; i++)
	{
		printf("进程 %s 运行中...\n", p[i]->name);
		t = clock();		// 开始处理时间
		run(p[i]);
		p[i]->service_time = clock() - t;
		p[i]->end_time = clock();
	}
	show(p, 10);
	for (i=0; i<10; i++) free(p[i]);	// 销毁进程
	
	return 0;
}

void sort(PCB **p, int n)
{
	int i, j;
	int flag;
	PCB *tmp;

	for (i=0; i<n; i++)
	{
		flag = 1;
		for (j=0; j<n-1; j++)
		{
			if (p[j]->mission > p[j+1]->mission)
			{
				tmp = p[j];
				p[j] = p[j+1];
				p[j+1] = tmp;
				flag = 0;
			}
		}
		if (flag) break;
	}
}