#include <stdio.h>
#include <windows.h>

// 互斥量实验:
// 互斥量是信号量的简化版, 最多只能运行一个程序运行
// 可以通过互斥名跨进程访问

HANDLE mutex = NULL;	// 互斥量
int share[10];			// 共享资源


// 1 号线程向共享区写 0
DWORD WINAPI func1(void *id)
{
	int i;

	// 等待 mutex 信号
	WaitForSingleObject(mutex, INFINITE);
	printf("\n %d 号线程在访问共享区\n");
	for (i=0; i<10; i++) share[i] = 0;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	ReleaseMutex(mutex);		// 释放 mutex 

	return 0;
}

// 2 号线程向共享区写 1
DWORD WINAPI func2(void *id)
{
	int i;

	WaitForSingleObject(mutex, INFINITE);
	printf("\n %d 号线程在访问共享区\n");
	for (i=0; i<10; i++) share[i] = 1;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	ReleaseMutex(mutex);

	return 0;
}

int main(void)
{
	HANDLE t1, t2;

	mutex = CreateMutex(NULL, false, NULL);
	// 参数说明 CreateMutex(安全属性, 属于谁, 互斥量的名字)
	// NULL 安全属性
	// FALSE 表示这个互斥量谁都不属于, 也就是现在谁都可以占用他
	// FALSE 表示该互斥量处于激发态, 相当于信号量的 1 值
	// NULL 表示这个互斥量没有名字, 因为不用跨进程访问, 取不取名字无所谓
	if (mutex == NULL) return 0;

	t1 = CreateThread(NULL, 0, func1, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);

	Sleep(400);		// main 线程休息一下
	CloseHandle(mutex);
	CloseHandle(t1);
	CloseHandle(t2);

	return 0;
}
