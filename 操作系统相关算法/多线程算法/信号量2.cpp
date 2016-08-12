#include <stdio.h>
#include <windows.h>

// 此次实验内容: 有一个线程在运行, main 线程必须等他结束才能结束
// 用信号量来通知 main 线程何时结束
// 不再使用 Sleep() 来估计大约什么时候结束

HANDLE mutex;		// 信号量
HANDLE quit;		// main 结束信号

DWORD WINAPI func1(void *id)
{
	int i;

	WaitForSingleObject(mutex, INFINITE);	// P 操作
	printf("线程运行中, 请稍等...\n");
	for (i=0; i<500000000; i++) ;		// 空操作
	ReleaseSemaphore(quit, 1, NULL);	// 给 quit 加一, 告诉main可以退出
	ReleaseSemaphore(mutex, 1, NULL);	// V 操作 

	return 0;
}

int main(void)
{
	HANDLE t;
	int i;

	mutex = CreateSemaphore(NULL, 1, 1, NULL);
	// quit 初始值为 0, 因为 main 要等待其他线程的信号
	quit = CreateSemaphore(NULL, 0, 1, NULL);
	if (mutex == NULL || quit == NULL) return 0;

	t = CreateThread(NULL, 0, func1, NULL, 0, NULL);

	CloseHandle(mutex);		// 释放资源
	CloseHandle(t);

	// 不再使用 Sleep() 来让线程充分执行完
	// main 函数的线程想要结束, 但是必须等上面的线程结束了, 才能结束
	// 线程结束后, 给 quit 发信号(V 操作), 于是 main 的线程可以结束
	WaitForSingleObject(quit, INFINITE);
	CloseHandle(quit);
	return 0;
}
