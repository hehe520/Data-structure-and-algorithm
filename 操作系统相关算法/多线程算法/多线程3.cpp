
// 两个线程修改一个变量的值

#include <stdio.h>
#include <windows.h>

HANDLE mutex;
HANDLE m[3];
int value = 0;

// 多个线程来访问 getvalue, 修改 value 的值
void getvalue(int id)
{
	WaitForSingleObject(mutex, INFINITE);
	printf("fun%d -> %d\n", id, value);
	value++;
	ReleaseSemaphore(mutex, 1, NULL);
}

DWORD WINAPI func1(LPVOID id)
{
	int i;

	for (i=0; i<50; i++) getvalue(1);
	ReleaseSemaphore(m[0], 1, NULL);
	return 0;
}

DWORD WINAPI func2(LPVOID id)
{
	int i;

	for (i=0; i<50; i++) getvalue(2);
	ReleaseSemaphore(m[1], 1, NULL);	// 发信号执行完
	return 0;
}

DWORD WINAPI func3(LPVOID id)
{
	int i;

	for (i=0; i<50; i++) getvalue(3);
	ReleaseSemaphore(m[2], 1, NULL);
	return 0;
}

int main(void)
{
	HANDLE t1, t2, t3;

	m[0] = CreateSemaphore(NULL, 0, 1, NULL);
	m[1] = CreateSemaphore(NULL, 0, 1, NULL);
	m[2] = CreateSemaphore(NULL, 0, 1, NULL);
	mutex = CreateSemaphore(NULL, 1, 5, NULL);
	t1 = CreateThread(NULL, 0, func1, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);
	t3 = CreateThread(NULL, 0, func3, NULL, 0, NULL);

	// 等待三个线程运行完, main 线程再退出
	WaitForMultipleObjects(3, m, true, INFINITE);
	CloseHandle(t1);
	CloseHandle(t2);
	CloseHandle(t3);
	CloseHandle(m[0]);
	CloseHandle(m[1]);
	CloseHandle(m[2]);
	CloseHandle(mutex);

	return 0;
}