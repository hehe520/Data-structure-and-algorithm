#include <stdio.h>
#include <windows.h>

// 用信号量实现线程同步
// 与互斥量的区别: 信号量可以同时允许不超过 n 个线程运行
// 互斥只能是一个线程运行

// 主要操作:
// CreateSemaphore()			创建一个信号量句柄
// CloseHandle()				释放一个信号量句柄
// WaitForSingleObject()		相当于 P 操作, 尝试运行
// ReleaseSemaphore()			相当于 V 操作, 退出共享区

HANDLE mutex;		// 信号量
int share[10];		// 共享资源, 任何线程都可以访问

DWORD WINAPI func1(void *id)		// 该函数往 share 中写 0
{
	int i;

	// WaitForSingleObject() 相当于 P 操作, 尝试访问
	// INFINITE 表示等待无限长的时间, 单位是毫秒, 写数字也可
	// 若超过这个时间还没信号, 线程就退出, 放弃等待
	// 即共享区的没有被访问到
	WaitForSingleObject(mutex, INFINITE);
	printf("\nfunc1 在访问共享区: \n");
	for (i=0; i<10; i++) share[i] = 0;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	ReleaseSemaphore(mutex, 1, NULL);
	// ReleaseSemaphore() 相当于 V 操作, 
	// 1 表示将信号量加 1, 同时唤醒一个在等待的线程

	return 0;
}

DWORD WINAPI func2(void *id)		// 该函数往 share 中写 1
{
	int i;

	WaitForSingleObject(mutex, INFINITE);		// P 操作
	printf("\nfunc2 在访问共享区: \n");
	for (i=0; i<10; i++) share[i] = 1;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	ReleaseSemaphore(mutex, 1, NULL);		// V 操作

	return 0;
}

int main(void)
{
	HANDLE t1, t2;

	// 创建一个信号量,安全值 NULL 初始值 1, 最大线程值 1, 信号量名: NULL
	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	if (mutex == NULL) return 0;	// 创建失败

	t1 = CreateThread(NULL, 0, func1, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);
	Sleep(100);

	CloseHandle(mutex);		// 释放资源
	CloseHandle(t1);
	CloseHandle(t2);
	return 0;
}
