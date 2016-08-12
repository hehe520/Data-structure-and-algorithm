
// 虚拟存储技术中的页面置换算法 - 第二次机会 (SC)
// 哪个页面先进来, 哪个页面不一定先出去, 还要参考访问位, 
// 如果被访问了, 将访问位置为 0, 给予一次机会
// 假设页面有 10 个, 页框只有 5 个
// 程序随机访问 10 个页面, 若页面不在页框内, 调用置换算法

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>
#include <queue>

struct 
{
	unsigned exist : 1;		// 在不在位
	unsigned visit : 1;		// 访问位
	unsigned num   : 6;		// 页框号
}page[10];			// 10 个页面
int kuang = 5;		//  5 个页框

std::queue<int> q;		// 使用 STL 的队列
int nopage = 0;			// 缺页次数
HANDLE mutex;			// 控制页表访问

int FIFO()		// 先进先出置换
{
	int page_num;	// 要淘汰的页面号

	while (1)
	{
		page_num = q.front();
		q.pop();
		if (page[page_num].visit == 1)		// 被访问过
		{
			page[page_num].visit = 0;		// 访问位置为 0
			q.push(page_num);			// 给予二次机会, 加回队尾
		}
		else { page[page_num].exist = 0;  break;}	// 淘汰
	}
	printf("淘汰页面 %d, ", page_num);

	return page[page_num].num;		// 返回该页面占用的页框号
}

void visit(int p)		// 尝试访问 p 号页面
{
	if (page[p].exist == 0)
	{
		printf("未命中, ");
		nopage++;
		// 不在页框中, 引发缺页中断
		if (kuang) page[p].num = kuang--;	// 页框有空余
		else page[p].num = FIFO();		// 页框满了, 需要淘汰一个
		q.push(p);
	}
	else printf("命中  , ");
	page[p].exist = 1;
	page[p].visit = 1;
}

// 每隔一个时钟滴答, 清空所有访问位
DWORD WINAPI clear(void *id)
{
	int i, size, t;
	while (1)
	{
		Sleep(3);		// 每隔 3 ms, 清空一次访问位
		WaitForSingleObject(mutex, INFINITE);
		printf("访问位定期清零被唤醒...\n");
		size = q.size();
		for (i=0; i<size; i++)
		{
			t = q.front();
			q.pop();
			page[t].visit = 0;
			q.push(t);
		}
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
	for (i=0; i<10; i++) page[i].exist = 0;
	t = CreateThread(NULL, 0, clear, NULL, 0, NULL);
	for (i=0; i<times; i++)
	{
		p = rand() % 10;
		WaitForSingleObject(mutex, INFINITE);
		printf("访问 页面 %d, ", p);
		visit(p);
		printf("\n");
		ReleaseSemaphore(mutex, 1, NULL);
	}
	SuspendThread(t);
	printf("\n访问次数 %d, 缺页次数 %d\n", times, nopage);
	CloseHandle(t);
	CloseHandle(mutex);
	return 0;
}
