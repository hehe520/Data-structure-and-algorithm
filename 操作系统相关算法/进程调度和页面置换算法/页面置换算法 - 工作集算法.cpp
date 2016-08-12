
// 虚拟存储技术中的页面置换算法 - 工作集算法
// 找出一个不在工作集中的页面并淘汰

// 假设页面有 10 个, 页框只有 5 个
// 程序随机访问 10 个页面, 若页面不在页框内, 置换页面

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

struct
{
	long int time;			// 时间
	struct 
	{
		unsigned exist : 1;		// 在不在位
		unsigned visit : 1;		// 访问位
		unsigned num   : 6;		// 页框号
	}bit;
}page[10];			// 10 个页面
int kuang = 5;		//  5 个页框
int nopage = 0;		// 缺页次数
const int max_time = 50;		// 最大生存时间 50 ms

HANDLE mutex;		// 控制 page[] 访问

// 遍历全部页面, 如果一个页面被访问了, 更新其访问时间
// 否则如果大于生存时间, 淘汰之
// 否则更新其时间, 防止被淘汰
// 如果没有找到淘汰的页面, 说明全部页面都在工作集中
// 那就随机淘汰一个, 最好淘汰一个没有修改的
int working()		// 淘汰一个页面
{
	int min = 0XFFFFF;
	int i, pos = -1;

	for (i=0; i<10; i++)
	{
		if (page[i].bit.visit == 1) page[i].time = clock();
		else
		{
			if (clock() - page[i].time > max_time)
			{ pos = i;   break; }
			else page[i].time = clock();
		}
	}
	if (pos == -1)	// 没有找到合适的页面, 说明全部页面都属于工作集
	{
		for (i=0; i<10; i++)		// 随机淘汰一个
			if (page[i].bit.exist == 1) 
			{ pos = i;   break;}
	}
	printf("淘汰页面 %d, ", pos);
	page[pos].bit.visit = page[pos].bit.exist = 0;
	return page[pos].bit.num;
}

void visit(int p)		// 访问
{
	if (page[p].bit.exist == 0)
	{
		printf("未命中, ");
		// 不在内存中, 产生缺页中断
		nopage++;
		if (kuang) page[p].bit.num = kuang--;
		else page[p].bit.num = working();
	}
	else printf("命中, ");
	page[p].bit.exist = 1;
	page[p].bit.visit = 1;
	page[p].time = clock();		// 记录使用时间
}

// 一个时钟滴答, 定时清空访问位
DWORD WINAPI dida(void *id)
{
	int i;
	
	while (1)
	{
		Sleep(5);
		WaitForSingleObject(mutex, INFINITE);
		printf("清空访问位...\n");
		for (i=0; i<10; i++) page[i].bit.visit = 0;
		ReleaseSemaphore(mutex, 1, NULL);
	}
	return 0;
}

int main(void)
{
	int p;		// 访问的页面
	int i, times = 20;
	HANDLE t;

	mutex = CreateSemaphore(NULL, 1, 2, NULL);
	srand(time(NULL));
	for (i=0; i<10; i++) page[i].time = page[i].bit.exist = 0;
	t = CreateThread(NULL, 0, dida, NULL, 0, NULL);
	for (i=0; i<times; i++)
	{
		p = rand() % 10;
		Sleep(1);
		WaitForSingleObject(mutex, INFINITE);
		printf("访问 页面 %d, ", p);
		visit(p);
		printf("\n");
		ReleaseSemaphore(mutex, 1, NULL);
	}
	SuspendThread(t);
	CloseHandle(t);
	CloseHandle(mutex);
	printf("\n访问次数 %d, 缺页次数 %d\n", times, nopage);
	return 0;
}