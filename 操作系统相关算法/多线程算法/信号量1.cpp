#include <stdio.h>
#include <windows.h>

// 此次实验内容: 
// 用信号量控制, 最大允许多个线程访问共享区资源
// 这就是与互斥量的区别, 互斥量只能是一个访问

HANDLE mutex;		// 信号量
int share = 1234;	// 共享资源, 任何线程都可以访问

DWORD WINAPI func1(void *id)		// 该函数去拿共享区的资源
{
	WaitForSingleObject(mutex, INFINITE);	// P 操作
	printf("\n%d 号线程在访问共享区: %d\n", *(int *)id, share);
	ReleaseSemaphore(mutex, 1, NULL);	// V 操作 

	return 0;
}

int main(void)
{
	HANDLE t[4];
	int i;

	// 创建一个信号量, 初始值 2, 最大值 4, 同时允许两个线程访问
	mutex = CreateSemaphore(NULL, 2, 4, NULL);
	if (mutex == NULL) return 0;	// 创建失败

	for (i=0; i<4; i++)
		t[i] = CreateThread(NULL, 0, func1, (void *)(&i), 0, NULL);
	Sleep(200);

	CloseHandle(mutex);		// 释放资源
	for (i=0; i<4; i++) CloseHandle(t[i]);

	return 0;
}
