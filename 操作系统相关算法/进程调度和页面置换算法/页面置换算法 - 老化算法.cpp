
// 虚拟存储技术中的页面置换算法 - 老化算法
// 利用二进制的思想来计算页面的使用程度
// 假设页面有 10 个, 页框只有 5 个
// 程序随机访问 10 个页面, 若页面不在页框内, 调用置换算法

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <windows.h>

struct 
{
	unsigned exist : 1;		// 在不在位
	unsigned visit : 1;		// 访问位
	unsigned num   : 6;		// 页框号
}page[10];			// 10 个页面
int kuang = 5;		//  5 个页框
int nopage = 0;		// 缺页次数

unsigned short int count[10];
// 老化算法中需要 8 个位来表示页面的老化程度
// C 语言中 unsigned short int 最小, 占 2 个字节 16 个位, 后面 8 个浪费了
// char 虽然占 1 个字节, 但是它是按照有符号数解释的

HANDLE mutex;		// 控制 count[] 访问

int old()		// 淘汰一个页面
{
	int min = 0XFFFFF;
	int i, pos;

	for (i=0; i<10; i++)
	{
		if (page[i].exist == 1 && count[i] < min)
		{
			min = count[i];
			pos = i;
		}
	}
	printf("淘汰页面 %d, ", pos);
	page[pos].visit = page[pos].exist = 0;
	count[pos] = 0;
	return page[pos].num;
}

void visit(int p)		// 访问
{
	if (page[p].exist == 0)
	{
		printf("未命中, ");
		// 不在内存中, 产生缺页中断
		nopage++;
		if (kuang) page[p].num = kuang--;
		else page[p].num = old();
	}
	else printf("命中, ");
	page[p].exist = 1;
	page[p].visit = 1;
	// 位运算, 第八位置成 1, 80 H = 1000 0000 B, 再或一下
//	count[p] = count[p] | 0x80;			// C 语言写法
	__asm
	{
		MOV EBX, p
		ADD EBX, EBX			// 双字节, 所以地址翻倍
		MOV AX, [count+EBX]
		OR  AL, 80H				// 或一下
		MOV [count+EBX], AX
	}
	// 汇编写法, 与 C 语言写法等效
}

// 一个时钟滴答, 将计数器向右移一位, 老化
DWORD WINAPI dida(void *id)
{
//	int i;
	while (1)
	{
		Sleep(50);
		WaitForSingleObject(mutex, INFINITE);
		printf("一个时钟滴答...\n");
//		for (i=0; i<10; i++)
//			count[i] = count[i] >> 1;
		__asm			// 该汇编和上面两句代码等效
		{
			MOV ECX, 10
			MOV EBX, 0
LP:			MOV AX, [count+EBX]
			SHR AX, 1				// 右移一位
			MOV [count+EBX], AX
			ADD EBX, 2
			LOOP LP
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
	for (i=0; i<10; i++) count[i] = 0;
	t = CreateThread(NULL, 0, dida, NULL, 0, NULL);
	for (i=0; i<times; i++)
	{
		p = rand() % 10;
		Sleep(1);			// 减慢一下速度, 不然没机会时钟滴答了,
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