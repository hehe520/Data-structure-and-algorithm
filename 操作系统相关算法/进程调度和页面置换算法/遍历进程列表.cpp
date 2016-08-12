
// 进程不断变化, 所以只能获取某一时刻的快照
// CreateToolhelp32Snapshot() 用来获取进程列表的快照
// 并且保存到一个句柄 snap 中
// Process32First() 从 snap 中获得第一个进程信息
// 并把进程的信息放入结构体 info 中
// Process32Next() 获得 snap 中的下一个结构体, 获取失败返回 NULL

#include <stdio.h>
#include <windows.h>
#include <tlhelp32.h>

int main(void)
{
	// 保存单个进程信息的结构体
	PROCESSENTRY32 info;
	HANDLE snap;		// 进程快照的句柄
	int flag;
	
	info.dwSize = sizeof(info);
	snap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS,0);
	flag = Process32First(snap, &info);
	printf("%-38s%-15s%-15s\n\n", "进程名", "进程ID", "线程数");
	while (flag)
	{
		printf("%-38s%-15u%-15u\n", 
			info.szExeFile, info.th32ProcessID, info.cntThreads);
		flag = Process32Next(snap, &info);
	}
	CloseHandle(snap);
	return 0;
}
