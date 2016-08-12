#include <stdio.h>
#include <windows.h>

// 信号量解决读者, 写者问题
// 共享区允许多个读者同时读取数据
// 但只运行一个写者线程写数据
// 有读者在的时候写者不能写入

HANDLE mutex;		// 控制各个读者访问临界区
HANDLE m2;			// 控制写者访问临界区
int num = 0;		// 当前正在临界区的读者数量
int share = 0;		// 临界区资源
CRITICAL_SECTION cs;

inline void read_data(int i, int data)
{
	// 如果输出到屏幕的 printf 没有被保护, 那么可能随时被打断
	// 于是输出到屏幕可能出现乱码, 
	EnterCriticalSection(&cs);
	printf(" %d 号读者读取了 %d\n", i, data);
	LeaveCriticalSection(&cs);
}

// 读写不断读取临界区资源
DWORD WINAPI reader(void *id)
{
	int i = *(int *)id;
	while (1)
	{
		// 尝试进入临界区, 如果是第一个进入, 锁定写者
		WaitForSingleObject(mutex, INFINITE);
		num++;
		if (num == 1) WaitForSingleObject(m2, INFINITE);
		// 第一个读者, 锁定 m2, 写者不能写
		ReleaseSemaphore(mutex, 1, NULL);
		// 此时写者不能写了, 开始读取数据
		read_data(i, share);

		// 尝试离开临界区, 如果是最后一个离开, 解锁写者
		WaitForSingleObject(mutex, INFINITE);
		num--;
		if (num == 0) ReleaseSemaphore(m2, 1, NULL);
		ReleaseSemaphore(mutex, 1, NULL);
		Sleep(5);
	}
	return 0;
}

// 写者不断改写临界区资源
DWORD WINAPI writer(void *id)
{
	int data = 1;

	while (1)
	{
		WaitForSingleObject(m2, INFINITE);
		share = data++;			// 写入 data
		printf("写者写入了 %d\n", data-1);
		ReleaseSemaphore(m2, 1, NULL);
	}
	return 0;
}

int main(void)
{
	HANDLE r[10];		// 10 个读者
	HANDLE w;			// 一个写者
	int i;
	int id[] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 9};
	
	// 初始化各个临界区变量, 信号量
	InitializeCriticalSection(&cs);
	mutex = CreateSemaphore(NULL, 1, 10, NULL);
	m2 = CreateSemaphore(NULL, 1, 10, NULL);

	// 开始运行
	w = CreateThread(NULL, 0, writer, NULL, 0, NULL);
	for (i=0; i<10; i++)
		r[i] = CreateThread(NULL, 0, reader, id + i, 0, NULL);

	Sleep(100);		// main 休息一下
	// 释放句柄资源
	CloseHandle(w);
	for (i=0; i<10; i++) CloseHandle(r[i]);
	CloseHandle(mutex);
	CloseHandle(m2);
	DeleteCriticalSection(&cs);

	return 0;
}
