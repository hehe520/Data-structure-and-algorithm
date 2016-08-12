
// 用信号量解决生产者, 消费者问题
// 生产者尝试进入共享槽放数据, 如果共享槽是满的, 则不让进入
// 所以设 empty 信号量控制空槽的数量, 初值 empty = N

// 同理: 消费者想要进入共享槽拿数据, 如果槽是空的, 也不让进入
// 所以设 full 信号量控制满槽的数量, 初值 full = 0

// 生产者和消费者不能同时访问共享槽, 所以设 mutex 控制共享槽访问

#include <stdio.h>
#include <windows.h>

#define N 10		// 槽的大小
int buffer[N];		// 共享槽
int pro = 0;		// 生产者的下标
int con = 0;		// 消费者的下标

HANDLE mutex;		// 控制共享槽/屏幕访问
HANDLE full;		// 满槽数量
HANDLE empty;		// 空槽数量

DWORD WINAPI producer(void *id)		// 生产者
{
	while (1)
	{
		WaitForSingleObject(empty, INFINITE);	// 看看空槽数量, 尝试减一
		buffer[pro] = rand() % 100;
		WaitForSingleObject(mutex, INFINITE);
		printf("生产者放入 %d\n", buffer[pro]);
		ReleaseSemaphore(mutex, 1, NULL);
		pro = (pro + 1) % N;
		ReleaseSemaphore(full, 1, NULL);	// 满槽数加一
	}
	return 0;
}

DWORD WINAPI consumer(void *id)		// 消费者
{
	while (1)
	{
		WaitForSingleObject(full, INFINITE);	// 看看满槽数量, 尝试减一
		WaitForSingleObject(mutex, INFINITE);
		printf("消费者拿走: %d\n", buffer[con]);
		ReleaseSemaphore(mutex, 1, NULL);
		con = (con + 1) % N;
		ReleaseSemaphore(empty, 1, NULL);	// 空槽数加一
	}
	return 0;
}

int main(void)
{
	HANDLE p, c;	// 两条线程

	// 最大运行线程 2 条
	mutex = CreateSemaphore(NULL, 1, 2, NULL);		// 共享区控制
	empty = CreateSemaphore(NULL, N, 2, NULL);		// 空槽控制
	full = CreateSemaphore(NULL, 0, 2, NULL);		// 满槽控制

	// CREATE_SUSPENDED 创建先不运行
	p = CreateThread(NULL, 0, producer, NULL, 0, NULL);
	c = CreateThread(NULL, 0, consumer, NULL, 0, NULL);

	Sleep(50);			// main 线程睡眠一下
	CloseHandle(p);		// 释放
	CloseHandle(c);
	CloseHandle(mutex);
	CloseHandle(empty);
	CloseHandle(full);
/*
	另外两个线程操作函数
	TerminateThread(handle,0); 
	ExitThread(0);
*/
	return 0;
}
