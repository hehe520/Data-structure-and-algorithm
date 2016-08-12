
// 输出某个目录下的全部文件和文件夹
// 详细的注释见 "文件目录遍历 - 当前目录.cpp"

#include <stdio.h>
#include <windows.h>
#include <string.h>

int file_num = 0;		// 文件数
int dir_num = 0;		// 目录数


// 递归深搜, 深度遍历目录, f 是递归的层数
void DFS(char *_path, int f=0)
{
	WIN32_FIND_DATA file_infor;		// 文件信息记录结构
	HANDLE in;			// 文件句柄
	int i;
	char path[2048];		// 当前目录名
	char next_path[2048];	// 下一层目录名

	strcpy(path, _path);
	strcat(path, "\\*");
	in = FindFirstFile(path, &file_infor);
	while (in != INVALID_HANDLE_VALUE)
	{
		if (file_infor.cFileName[0] != '.')		// .是父目录  ..是当前目录
		{
			for (i=0; i<f; i++) putchar('\t');	// 突出层次
			printf("%s", file_infor.cFileName);
			if (file_infor.dwFileAttributes == FILE_ATTRIBUTE_DIRECTORY)
			{	
				// 如果直接在 path 上修改, 回溯时要恢复现场
				dir_num++;
				printf("\t<目录>\n");
				strcpy(next_path, _path);		// 加上下一层目录名
				strcat(next_path, "\\");
				strcat(next_path, file_infor.cFileName);
				DFS(next_path, f + 1);		// 如果是目录, 递归搜索下一层
			}
			else
			{
				file_num++;		// 记录文件数
				putchar('\n');
			}
		}
		// 回来时, 继续搜索下一个文件, 搜索失败返回 0, false
		if (FindNextFile(in, &file_infor) == false) break;
	}
	FindClose(in);
}

int main(void)
{
	DFS("C:\\test", 0);			// 实验目录 c:\test
	printf("\n文件数 %d   目录数 %d\n", file_num, dir_num);

	return 0;
}