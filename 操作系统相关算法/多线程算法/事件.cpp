#include <stdio.h>
#include <windows.h>

// 使用事件来同步线程
// 事件可以同步在不同进程中的线程
// 原语操作
// CreateEvent()			// 创建一个事件
// WaitForSingleObject()	// 等待一个信号
// SetEvent()				// 设置为有信号
// ResetEvent()				// 设置为无信号

// 有信号时, WaitForSingleObject() 才能返回, 并且关闭这个信号

HANDLE mutex;		// 控制共享区访问
int share[10];		// 共享区

DWORD WINAPI func1(void *id)		// func1 写 0
{
	int i;

	// 等待一个信号, 有信号才返回
	WaitForSingleObject(mutex, INFINITE);
	printf("\nfunc1 正在访问共享区...\n");
	for (i=0; i<10; i++) share[i] = 0;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	SetEvent(mutex);
	// 信号被 WaitForSingleObject 拿走了
	// 用 SetEvent 重新使得 mutex 有信号, 相当于 V 操作

	return 0;
}

DWORD WINAPI func2(void *id)		// func2 写 1
{
	int i;
	
	WaitForSingleObject(mutex, INFINITE);
	printf("\nfunc2 正在访问共享区...\n");
	for (i=0; i<10; i++) share[i] = 1;
	for (i=0; i<10; i++) printf("%d, ", share[i]);
	printf("\n");
	SetEvent(mutex);

	return 0;
}

int main(void)
{
	HANDLE t1, t2;

	mutex = CreateEvent(NULL, false, true, NULL);
	// CreateEvent 参数解释在下面

	t1 = CreateThread(NULL, 0, func1, NULL, 0, NULL);
	t2 = CreateThread(NULL, 0, func2, NULL, 0, NULL);

	Sleep(100);
	CloseHandle(t1);
	CloseHandle(t2);
	CloseHandle(mutex);
	return 0;
}

/*  CreateEvent(NULL, true, false, NULL)
第一个 NULL, 是安全属性
第二个 false 
如果是false, 那么必须用 SetEvent() 来将事件恢复到有信号状态
如果是true, 当事件被一个等待线程释放以后，系统自动将事件状态恢复到无信号状态
第三个 true, 表示初始时有信号, 谁都可以占用它, false 即无信号
第四个 NULL, 是事件的名字, 也可以是无名的 NULL, 如果跨进程就要名字来辨别了
*/
