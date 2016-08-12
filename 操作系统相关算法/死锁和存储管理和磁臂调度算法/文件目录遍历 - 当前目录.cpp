
// 遍历某个目录下的所有文件
// 涉及到的 windows API 及宏
// FindFirstFile()			// 获得第一个文件
// FindnextFile()			// 获得下一个文件
// FindClose()				// 释放句柄
// GetLastError()			// 获得最后的错误信息
// INVALID_HANDLE_VALUE		// 无效句柄
// ERROR_NO_MORE_FILES		// 没有更多的文件
// FILE_ATTRIBUTE_DIRECTORY	// 是目录

// 实验目录 c:\test, 在里面放一些文件及目录

#include <stdio.h>
#include <windows.h>

int main(void)
{
	WIN32_FIND_DATA filedata;	// 文件信息, WIN32_FIND_DATA 声明见下
	HANDLE in;		// 文件句柄

	// filedata 内容由操作系统填写
	in = FindFirstFile("c:\\test\\*.*", &filedata);
	while (in != INVALID_HANDLE_VALUE)		// 如果不是无效句柄
	{
		printf("%s", filedata.cFileName);
		if (filedata.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			printf("\t<目录>");			// 判断是不是目录
		putchar('\n');
		if (FindNextFile(in, &filedata) == false) break;
//		if (GetLastError() == ERROR_NO_MORE_FILES) break;		// 该句也可
	}
	FindClose(in);			// 释放资源

	return 0;
}
/*
typedef struct _WIN32_FIND_DATA
{ 
　　 DWORD dwFileAttributes;			// 文件属性 
　　 FILETIME ftCreationTime;			// 文件创建时间 
　　 FILETIME ftLastAccessTime;			// 文件最后一次访问时间 
　　 FILETIME ftLastWriteTime;			// 文件最后一次修改时间 
　　 DWORD nFileSizeHigh;				// 文件长度高32位 
　　 DWORD nFileSizeLow;				// 文件长度低32位 
　　 DWORD dwReserved0;					// 系统保留 
　　 DWORD dwReserved1;					// 系统保留 
　　 TCHAR cFileName[ MAX_PATH ];		// 长文件名 
　　 TCHAR cAlternateFileName[ 14 ];	// 8.3格式文件名 
}WIN32_FIND_DATA, *PWIN32_FIND_DATA; 

dwFileAttributes (文件属性) 取值如下
FILE_ATTRIBUTE_ARCHIVE
FILE_ATTRIBUTE_COMPRESSED
FILE_ATTRIBUTE_DIRECTORY		// 目录, 判断是目录还是文件
FILE_ATTRIBUTE_HIDDEN			// 隐藏
FILE_ATTRIBUTE_NORMAL			// 正常
FILE_ATTRIBUTE_OFFLINE
FILE_ATTRIBUTE_READONLY			// 只读
FILE_ATTRIBUTE_SYSTEM			// 系统文件夹
FILE_ATTRIBUTE_TEMPORARY		// 临时文件夹
*/