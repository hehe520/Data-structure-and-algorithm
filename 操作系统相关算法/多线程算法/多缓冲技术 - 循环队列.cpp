
// 多缓冲, 决解高速设备与低速设备之间的数据传递问题
// 缓冲区数据结构: 循环队列

// 实验1 :
// 一条线程 A 高速地向缓冲区循环输入 0 - 9 十个数字
// 一条线程 B 缓慢地从缓冲区取数据

// 实验2 :
// 一条线程 A 缓慢地输入 0 - 9 
// 一条线程 B 高速地读取数据

// 附带 TerminateThread(A, 0), ExitThread(0) 用法

#include <stdio.h>
#include <windows.h>

#define SIZE 50
int queue[SIZE];	// 循环队列缓冲区
int front = 0;		// 队头
int rear = 0;		// 队尾
// 缓冲区虽然是临界资源, 但这里不需要保护
// 因为输入线程修改 rear, 读取线程修改 front

// 屏幕是临界资源, 需要保护, 防止 printf 正在输出的时候被打断, 于是产生乱码
CRITICAL_SECTION mutex;

// 高速地向缓冲区放入 0 - 9
DWORD WINAPI high_put(void *id)
{
	int data = 0;
	while (1)
	{
		while ((rear + 1) % SIZE == front);
		// 如果缓冲区满了, 循环等待
		// 实际中, I/O通道可能切换去其它设备进行I/O
		queue[rear] = data;
		rear = (rear + 1) % SIZE;
		data = (data + 1) % 10;		// 循环放 0 - 9 数字
	}
	return 0;
}

// 缓慢地读取数据
DWORD WINAPI low_get(void *id)
{
	while (1)
	{
		Sleep(200);		// 拖慢速度
		while (front == rear);
		EnterCriticalSection(&mutex);		// 保护 printf
		printf("%d, ", queue[front]);
		if (queue[front] == 9) putchar('\n');
		LeaveCriticalSection(&mutex);
		front = (front + 1) % SIZE;
	}
	return 0;
}

// 缓慢地输入 0 - 9
DWORD WINAPI low_put(void *id)
{
	int data = 0;
	while (1)
	{
		Sleep(250);		// 减缓速度
		while ((rear + 1) % SIZE == front);
		queue[rear] = data;
		rear = (rear + 1) % SIZE;
		data = (data + 1) % 10;		// 循环放 0 - 9 数字
	}
	return 0;
}

// 高速地读取
DWORD WINAPI high_get(void *id)
{
	while (1)
	{
		while (front == rear);			// 空了
		EnterCriticalSection(&mutex);	// 保护 printf
		printf("%d, ", queue[front]);
		if (queue[front] == 9) putchar('\n');
		LeaveCriticalSection(&mutex);
		front = (front + 1) % SIZE;
//		ExitThread(0);			// 线程自行退出
	}
	return 0;
}

int main(void)
{
	HANDLE A;		// 高速线程
	HANDLE B;		// 低速线程

	InitializeCriticalSection(&mutex);		// 初始化临界区
	front = rear = 0;
	printf("实验一:\n");
	A = CreateThread(NULL, 0, high_put, NULL, 0, NULL);
	B = CreateThread(NULL, 0, low_get, NULL, 0, NULL);
	Sleep(10000);			// 运行 10 秒
	TerminateThread(A, 0);		// 由主线程杀死 A 线程
	TerminateThread(B, 0);		// 释放 A 线程占用的缓冲区资源(queue)
	CloseHandle(A);
	CloseHandle(B);
	DeleteCriticalSection(&mutex);		// 释放临界区

	// 实验一用了 TerminateThread() 来杀死线程, 释放资源
	// 但是有可能在临界区被锁上的时候, 线程被杀死, 于是临界区永运地被锁了
	// 下面线程要进入临界区的时候, 产生死锁!
	// 这里重新初始化临界区, 让下面可以正常运行

	// TerminateThread() 的危险性
	// 例一: 用 new, delete 等操作内存时, 系统会有一把锁, 来管理内存申请
	// 如果 A 线程用了 new 申请内存, 于是锁被锁上, 还没来得及解锁
	// B 线程用 TerminateThread(A, 0) 把 A 杀死
	// 于是下面的程序无法再用 new, delete 管理内存, 死锁产生

	// 例二: A 线程中申请了对象, 被 TerminateThread() 强制结束的时候
	// 申请的对象还来不及释放, 造成内存泄漏

	InitializeCriticalSection(&mutex);		// 再次初始化临界区
	front = rear = 0;
	printf("\n\n实验二:\n");
	A = CreateThread(NULL, 0, low_put, NULL, 0, NULL);
	B = CreateThread(NULL, 0, high_get, NULL, 0, NULL);
	Sleep(10000);			// 运行 10 秒
	TerminateThread(A, 0);
	TerminateThread(B, 0);
	CloseHandle(A);
	CloseHandle(B);
	DeleteCriticalSection(&mutex);

	return 0;
}