
// 虚拟存储技术中的页面置换算法 - 先进先出 (FIFO)
// 哪个先进来, 哪个页面就先出去
// 假设页面有 10 个, 页框只有 5 个
// 程序随机访问 10 个页面, 若页面不在页框内, 置换页面

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <queue>

int nopage = 0;

struct 
{
	unsigned exist : 1;		// 在不在位
	unsigned visit : 1;		// 访问位
	unsigned num   : 6;		// 页框号
}page[10];			// 10 个页面
int kuang = 5;		//  5 个页框

std::queue<int> q;		// 使用 STL 的队列

int FIFO()		// 先进先出置换
{
	int page_num;	// 要淘汰的页面号

	page_num = q.front();
	q.pop();
	page[page_num].exist = 0;		// 淘汰
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

int main(void)
{
	int p;		// 访问的页面
	int i, times = 20;

	srand(time(NULL));
	for (i=0; i<10; i++) page[i].exist = 0;
	for (i=0; i<times; i++)
	{
		p = rand() % 10;
		printf("访问 页面 %d, ", p);
		visit(p);
		printf("\n");
	}
	printf("\n访问次数 %d, 缺页次数 %d\n", times, nopage);
	return 0;
}
