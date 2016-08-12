#include <stdio.h>
#include <windows.h>

// 忙等待的互斥, Peterson算法
// enter_gion() 加锁, leave_gion() 解锁

// 忙等待互斥 与 信号量的区别: 
// 信号量解决了忙等待造成 CPU 浪费

// CPU 分给了进程 1 一个时间片, 进程 1 一直在看临界区能不能进入
// 即循环等待, 直到时间片用完, CPU 切换到另一个进程

// 如果是用信号量, CPU 分给进程 1 一个时间片,
// 进程 1 同样去看临界区能不能访问, 
// 如果不能该进程马上被阻塞到阻塞队列里, CPU 马上切换到别的进程
// 即 时间片没有被完全用完, 只是用了看了一下临界区的时间而已

// 即忙等待要消耗完全部时间片, 信号量会被主动调出运行队列

#define NUM 10			// 最大线程数量
int want_to_join;
int interests[NUM];

// 给 pro 号线程加锁
void enter_gion(int pro)
{
	int other = 1 - pro;

	want_to_join = pro;
	interests[pro] = 1;		// 想进入
	while (pro == want_to_join && interests[other] == 1);
	// 如果我想进入, 别人也想进入, 就先等待, 让另一个线程进入
	// 此时 want_to_join 已经被改成我了, 所以另一个线程可以从循环中退出
}

void leave_gion(int pos)
{
	interests[pos] = 0;
}

DWORD WINAPI func1(void *para)
{
	int i;

	enter_gion(0);		// 尝试进入临界区
						// 临界区
	for (i=0; i<50; i++) printf("t1 = %d\n", i);
	leave_gion(0);		// 离开临界区

	return 0;
}

DWORD WINAPI func2(void *para)
{
	int i;

	enter_gion(1);
	for (i=0; i<50; i++) printf("  t2 = %d\n", i);
	leave_gion(1);

	return 0;
}

int main(void)
{
	HANDLE t1, t2;

	t1 = CreateThread(NULL, 0, func1, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);
	Sleep(500);
	CloseHandle(t1);
	CloseHandle(t2);

	// Sleep(500) 是为了让main进程不立即结束, 让线程有时间运行完
	return 0;
}
