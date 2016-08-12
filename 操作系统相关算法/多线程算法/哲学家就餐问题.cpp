#include <stdio.h>
#include <windows.h>

// 哲学家就餐问题
#define MAX 5			// 五个哲学家
#define LEFT  ((i - 1 + MAX) % MAX)
#define RIGHT ((i + 1) % MAX)		// 类似队列
#define EATING   0
#define THINKING 1
#define HUNGRY   2
unsigned int status[MAX];
HANDLE s[MAX];		// 每个哲学家一个信号量
HANDLE mutex;

void test(HANDLE *s, int i)
{
	if (status[i] == HUNGRY 
		&& status[LEFT] != EATING 
		&& status[RIGHT] != EATING)
	{
		printf("%d 号哲学家正在吃...\n", i);
		status[i] = EATING;
		ReleaseSemaphore(*s, 1, NULL);	// 发一个信号, 表示吃到了
	}
}

void take_fork(int i)		// 第 i 号拿叉子
{
	WaitForSingleObject(mutex, INFINITE);
	printf("%d 号尝试拿起叉子\n", i);
	status[i] = HUNGRY;
	test(s + i, i);			// 尝试吃, 未必能吃到
	ReleaseSemaphore(mutex, 1, NULL);

	// 如果吃到了, s[i] 有信号, 可以直接结束, 否则 s[i] 无信号
	// 如果没吃到, 先阻塞, 等待其他人吃完, 来唤醒自己
	WaitForSingleObject(s[i], INFINITE);
}

void put_fork(int i)		// 第 i 号放叉子
{
	WaitForSingleObject(mutex, INFINITE);
	status[i] = THINKING;		// 调整状态
	printf("%d 号吃完, 尝试让邻近的吃\n", i);
	test(s + LEFT, LEFT);		// 给左右吃的信号
	test(s + RIGHT, RIGHT);
	ReleaseSemaphore(mutex, 1, NULL);
}

DWORD WINAPI go(void *id)		// 五个哲学家拿叉子
{
	int i = *(int *)id;

	take_fork(i);
	put_fork(i);
	return 0;
}

int main(void)
{
	HANDLE t[MAX];		// 每个哲学家一个线程
	int i;
	int id[5] = {1, 2, 3, 4, 5};
	
	mutex = CreateSemaphore(NULL, 1, MAX, NULL);
	for (i=0; i<MAX; i++)		// 创建信号量, 初始是 0
	{
		s[i] = CreateSemaphore(NULL, 0, MAX, NULL);
		status[i] = THINKING;
	}
	for (i=0; i<MAX; i++)
		t[i] = CreateThread(NULL, 0, go, id + i, 0, NULL);

	Sleep(500);
	for (i=0; i<MAX; i++) CloseHandle(t[i]);
	for (i=0; i<MAX; i++) CloseHandle(s[i]);
	CloseHandle(mutex);

	return 0;
}
