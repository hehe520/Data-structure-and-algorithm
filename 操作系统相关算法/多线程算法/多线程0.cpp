#include <stdio.h>
#include <windows.h>

// 创建 10 个线程并退出

DWORD WINAPI func(void *i)
{
	printf("线程 %d\n", *(int *)i);
	return 0;
}

int main(void)
{
	HANDLE t[10];
	int i;

	for (i=0; i<10; i++)
	{
		t[i] = CreateThread(NULL, 0, func, (void *)(&i), 0, NULL);
		Sleep(20);			// 让线程有时间执行完
	}
	for (i=0; i<10; i++)
		CloseHandle(t[i]);
	return 0;
}