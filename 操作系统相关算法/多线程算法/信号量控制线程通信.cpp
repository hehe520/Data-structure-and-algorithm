
// 线程 A 往缓冲区放数据, 缓冲区最多只能放一个数据
// 线程 B 去缓冲区取数据, 再 A 放了之后才能取
// A 放了数据后, B 才能取
// 首先 A 放数据, 然后通知 B 去取, 同时 A 线程阻塞, 等待 B 的信号
// B 尝试取数据, 取完后, 唤醒 A, 同时 B 阻塞

#include <stdio.h>
#include <windows.h>

int share;		// 缓冲区
HANDLE put;		// 放数据的信号量
HANDLE get;		// 取数据的信号量

// 放数据, 循环放 0 - 9 十个数字
DWORD WINAPI producter(void *id)
{
	int data = 0;

	while (1)
	{
		WaitForSingleObject(put, INFINITE);		// 尝试放数据
		share = data;
		data = (data + 1) % 10;
		ReleaseSemaphore(get, 1, NULL);			// 通知 B 取数据
	}
	return 0;
}

DWORD WINAPI customer(void *id)		// 取数据
{
	while (1)
	{
		WaitForSingleObject(get, INFINITE);		// 尝试取数据
		printf("%d, ", share);
		if (share == 9) printf("\n");
		ReleaseSemaphore(put, 1, NULL);		// 取完, 通知 A 继续放
	}
	return 0;
}

int main(void)
{
	HANDLE a, b;

	put = CreateSemaphore(NULL, 1, 2, NULL);
	get = CreateSemaphore(NULL, 0, 2, NULL);
	a = CreateThread(NULL, 0, producter, NULL, 0, NULL);
	b = CreateThread(NULL, 0, customer, NULL, 0, NULL);

	Sleep(50);

	CloseHandle(a);
	CloseHandle(b);
	CloseHandle(put);
	CloseHandle(get);
	return 0;
}