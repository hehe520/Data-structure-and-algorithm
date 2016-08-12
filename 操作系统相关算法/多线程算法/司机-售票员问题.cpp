#include <stdio.h>
#include <windows.h>

// 信号量解决司机 - 售票员问题
// 售票员想开门, 必须等司机的停车信号, stop_car
// 司机想开车, 必须等售票员关门信号, close_door
// 初始: stop_car = 0, close_door = 0

HANDLE stop_car;
HANDLE close_door;

DWORD WINAPI driver(void *id)		// 司机
{
	while (1)
	{
		printf("---> 司机: 停车\n");
		ReleaseSemaphore(stop_car, 1, NULL);		// 发出停车信号
		WaitForSingleObject(close_door, INFINITE);	// 等待关门信号
		printf("司机: 开车\n");
	}
	return 0;
}

DWORD WINAPI seller(void *id)		// 售票员
{
	while (1)
	{
		WaitForSingleObject(stop_car, INFINITE);	// 等待停车信号
		printf("售票员: 开门\n");
		printf("售票员: 售票\n");
		printf("售票员: 关门\n");
		ReleaseSemaphore(close_door, 1, NULL);		// 发出关门信号
	}
	return 0;
}

int main(void)
{
	HANDLE d, s;

	stop_car = CreateSemaphore(NULL, 0, 2, NULL);
	close_door = CreateSemaphore(NULL, 0, 2, NULL);
	d = CreateThread(NULL, 0, driver, NULL, 0, NULL);
	s = CreateThread(NULL, 0, seller, NULL, 0, NULL);

	Sleep(1000);
	CloseHandle(stop_car);
	CloseHandle(close_door);
	CloseHandle(d);
	CloseHandle(s);

	return 0;
}