
// 虚拟存储技术中的页面置换算法 - 最近最少使用 (LRU)
// 每个页面一个计数器, 访问一次, 计数器加一
// 缺页中断时, 淘汰访问次数最少的页面
// 假设页面有 10 个, 页框只有 5 个
// 程序随机访问 10 个页面, 若页面不在页框内, 调用置换算法

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

struct 
{
	unsigned exist : 1;		// 在不在位
	unsigned visit : 1;		// 访问位
	unsigned num   : 6;		// 页框号
}page[10];				// 10 个页面
long int count[10];		// 计数器
int kuang = 5;			//  5 个页框

int nopage = 0;			// 缺页次数

int LRU()
{
	int i, pos;
	int min = 0XFFFFF;

	for (i=0; i<10; i++)
	{
		if (page[i].exist == 1)		// 查找在内存中的页面
		{
			if (count[i] < min)
			{
				min = count[i];
				pos = i;
			}
		}
	}
	printf("淘汰页面 %d, ", pos);
	page[pos].exist = 0;		// 淘汰
	count[pos] = 0;				// 计数器清零
	return page[pos].num;
}

void visit(int p)		// 访问页面
{
	if (page[p].exist == 0)		// 不在
	{
		printf("未命中, ");
		nopage++;
		if (kuang) page[p].num = kuang--;
		else page[p].num = LRU();
	}
	else printf("命中  , ");
	page[p].exist = 1;
	page[p].visit = 1;
	count[p]++;		// 计数器
}

int main(void)
{
	int p;		// 访问的页面
	int i, times = 20;

	srand(time(NULL));
	for (i=0; i<10; i++) count[i] = page[i].exist = 0;
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
