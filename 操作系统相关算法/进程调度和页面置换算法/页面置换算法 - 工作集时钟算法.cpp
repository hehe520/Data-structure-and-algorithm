
// 虚拟存储技术中的页面置换算法 - 工作集时钟算法
// 由于工作集算法需要扫面全部的页面, 比较费时
// 于是在工作集算法上改进, 不扫描全部页面, 
// 只扫描在内存中的页面, 即页框, 所有页框形成一个循环队列
// 预先随机装入 5 个页面, 以形成循环队列 (静态的)

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
		unsigned modify: 1;		// 修改位
		unsigned num   : 5;		// 页框号
	}bit;
}page[10];			// 10 个页面 
int page_num[5];	// 5 个页面
int top = 0;		// 指针

int nopage = 0;		// 缺页次数
const int max_time = 50;		// 最大生存时间 50 ms

HANDLE mutex;		// 控制 count[] 访问

// 遍历所有在内存中的页面, 如果被访问过的, 访问位置为 0
// 如果没访问过, 则看其有没有超过最大生存时间, 超过了就淘汰
// 如果没超过, 更新其时间, 防止被淘汰, 
// 如果没有找到合适的页面就随机淘汰一个
int working()
{
	int min = 0XFFFFF;
	int pos = -1;
	int start;

	start = top;		// 记录指针当前的位置
	while ((top + 1) % 5 != start)		// 转一圈
	{
		if (page[top].bit.visit == 1)
		{
			page[top].time = clock();
			page[top].bit.visit = 0;
		}
		else if (page[top].bit.modify == 0 
				&& clock() - page[top].time > max_time)		// 没有修改过
		{ pos = top;   break; }
		top = (top + 1) % 5;
	}
	if (pos == -1) pos = top;
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
		page[p].bit.num = working();
	}
	else printf("命中, ");
	page[p].time = clock();		// 记录使用时间
	page[p].bit.exist = 1;
	page[p].bit.visit = 1;
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
	for (i=0; i<10; i++)
	{
		page[i].bit.modify = 0;
		page[i].bit.exist = 0;
		page[i].time = 0;
	}
	for (i=0; i<5; i++)		// 预先装入 5 个页面
	{
		page_num[i] = i;
		page[i].bit.exist = 1;
	}
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