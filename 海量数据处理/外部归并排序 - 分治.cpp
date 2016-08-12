
// 对 2 亿个数字进行排序, 约 10 G 的文件, 每个数字 int 能表示
// 算法流程
// 将 10 G 的文件散列到 300 个文件中, 每个文件大约 35 MB
// 对 35 MB 的小文件内部排序, 或者分发到多台计算机中, 并行处理 MapReduce
// 最后使用最小堆, 进行 300 路归并排序, 合成大文件
// 再写一个算法判断 2 亿个数字是否有序

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>
#include <queue>

#define FILE_NUM  300				// 哈希文件数
#define HASH(a)   (a % FILE_NUM)

int num = 6000000;						// 2 亿个数字, 手动改
char path[20] = "c:\\data.dat";			// 待排文件
char result[20] = "c:\\result.dat";		// 排序后文件
char tmpdir[100] = "c:\\hashfile";		// 临时目录

// 随机生成 2 亿个数字
int write_file(void)
{
	FILE *out = NULL;
	int i;

	printf("\n正在生成 %d 个数字...\n\n", num);
	out = fopen(path, "wt");
	if (out == NULL) return 0;

	unsigned int s, e;
	e = s = clock();
	for (i=0; i<num; i++)
	{
		e = clock();
		if (e - s > 1000)		// 计算进度
		{
			printf("\r处理进度 %0.2f %%\t", (i * 100.0) / num);
			s = e;
		}
		fprintf(out, "%d\n", 
			(rand() % 31623) * (rand() % 31623));
	}
	fclose(out);
	return 1;
}

// 对 2 亿个数字进行哈希, 分散到子文件中
// 入口参数: path, tmpdir
int map(void)
{
	FILE *in = NULL;
	FILE *tmp[FILE_NUM + 5];
	char hashfile[512];		// 哈希文件地址
	int data, add;
	int i;

	printf("\r正在哈希 %s\n\n", path);
	in = fopen(path, "rt");
	if (in == NULL) return 0;
	for (i=0; i<FILE_NUM; i++) tmp[i] = NULL;

	// 开始哈希, 核心代码要尽可能的加速
	unsigned int s, e;
	e = s = clock();
	i = 0;
	while (fscanf(in, "%d", &data) != EOF)
	{
		add = HASH(data);
		if (tmp[add] == NULL)
		{
			sprintf(hashfile, "%s\\hash_%d.~tmp", tmpdir, add);
			tmp[add] = fopen(hashfile, "a");
		}
		fprintf(tmp[add], "%d\n", data);

		i++;
		e = clock();			// 计算进度
		if (e - s > 1000)
		{
			printf("\r处理进度 %0.2f %%\t", (i * 100.0) / num);
			s = e;
		}
	}
	for (i=0; i<FILE_NUM; i++)
		if (tmp[i]) fclose(tmp[i]);
	fclose(in);

	return 1;
}

// 对 300 个文件逐个排序, 采用堆排序 STL 的优先队列
void calc(void)
{
	int fileexist(char *path);		// 判断文件存在
	std::priority_queue<int> q;		// 堆排序
	char hashfile[512];
	FILE *fp = NULL;
	int i, data;

	// 逐个处理 300 个文件, 或者将这些文件发送到其它计算机中并行处理
	for (i=0; i<FILE_NUM; i++)
	{
		sprintf(hashfile, "%s\\hash_%d.~tmp", tmpdir, i);
		if (fileexist(hashfile))
		{
			printf("\r正在排序 hash_%d.~tmp\t", i);

			// 小文件从磁盘加入内存中
			fp = fopen(hashfile, "rt");
			while (fscanf(fp, "%d", &data) != EOF)
			{
				q.push(data);
				// 优先队列默认是大顶堆, 即降序排序
				// 要升序需要重载 () 运算符
			}
			fclose(fp);

			// 排序后再从内存写回磁盘
			fp = fopen(hashfile, "wt");		// 覆盖模式写
			while (!q.empty())
			{
				fprintf(fp, "%d\n", q.top());
				q.pop();
			}
			fclose(fp);
		}
	}
}

typedef struct node		// 队列结点
{
	int data;
	int id;			// 哈希文件的编号
	bool operator < (const node &a) const
	{ return data < a.data; }
}node;

// 将 300 个有序文件合并成一个文件, K 路归并排序
int reduce(void)
{
	int fileexist(char *path);
	std::priority_queue<node> q;		// 堆排序
	FILE *file[FILE_NUM + 5];
	FILE *out = NULL;
	char hashfile[512];
	node tmp, p;
	int i, count = 0;

	printf("\r正在合并 %s\n\n", result);
	out = fopen(result, "wt");
	if (out == NULL) return 0;
	for (i=0; i<FILE_NUM; i++) file[i] = NULL;
	for (i=0; i<FILE_NUM; i++)		// 打开全部哈希文件
	{
		sprintf(hashfile, "%s\\hash_%d.~tmp", tmpdir, i);
		if (fileexist(hashfile))
		{
			file[i] = fopen(hashfile, "rt");
			fscanf(file[i], "%d", &tmp.data);
			tmp.id = i;
			q.push(tmp);		// 初始化队列
			count++;			// 计数器
			printf("\r入队进度 %0.2f %%\t", (count * 100.0) / FILE_NUM);
		}
	}
	unsigned int s, e;
	e = s = clock();
	while (!q.empty())		// 开始 K 路归并
	{
		tmp = q.top();
		q.pop();
		// 将堆顶的元素写回磁盘, 再从磁盘中拿一个到内存
		fprintf(out, "%d\n", tmp.data);
		if (fscanf(file[tmp.id], "%d", &p.data) != EOF)
		{
			p.id = tmp.id;
			q.push(p);
			count++;
		}

		e = clock();			// 计算进度
		if (e - s > 1000)
		{
			printf("\r处理进度 %0.2f %%\t", (count * 100.0) / num);
			s = e;
		}
	}
	for (i=0; i<FILE_NUM; i++)
		if (file[i]) fclose(file[i]);
	fclose(out);

	return 1;
}

int check(void)		// 检查是否降序排序
{
	FILE *in = NULL;
	int max  = 0x7FFFFFFF;
	int data;
	int count = 0;

	printf("\r正在检查文件正确性...\n\n");
	in = fopen(result, "rt");
	if (in == NULL) return 0;

	unsigned int s, e;
	e = s = clock();
	while (fscanf(in, "%d", &data) != EOF)
	{
		if (data <= max) max = data;
		else
		{
			fclose(in);
			return 0;
		}
		count++;
		e = clock();			// 计算进度
		if (e - s > 1000)
		{
			printf("\r处理进度 %0.2f %%\t", (count * 100.0) / num);
			s = e;
		}
	}
	fclose(in);
	return 1;
}

// 判断文件存在
int fileexist(char *path)
{
	FILE *fp = NULL;

	fp = fopen(path, "rt");
	if (fp)
	{
		fclose(fp);
		return 1;
	}
	else return 0;
}

int main(void)
{
	char cmd_del[200];		// 删除目录
	char cmd_att[200];		// 设置隐藏
	char cmd_mkdir[200];	// 建立目录

	// 初始化 cmd 命令, 建立工作目录
	sprintf(cmd_del, "rmdir /s /q %s", tmpdir);
	sprintf(cmd_att, "attrib +h %s", tmpdir);
	sprintf(cmd_mkdir, "mkdir %s", tmpdir);
	if (access(path, 0) == 0) system(cmd_del);
	system(cmd_mkdir);		// 建立工作目录
	system(cmd_att);		// 隐藏目录

	// 随机生成 2 亿个数字
	if (!write_file()) return 0;

	map();		// 对 2 亿个数字进行哈希, 即 Map
	calc();		// 对 300 个文件逐个排序
	reduce();	// 最后将 300 个有序文件合并成一个文件, 即 reduce
	if (check()) printf("\r排序正确!\t\t\t\n\n");
	else printf("\r排序错误!\t\t\t\n\n");

	system(cmd_del);	// 删除哈希文件
	remove(path);		// 删除 2 亿数字文件
	remove(result);		// 删除排序后的文件

	return 0;
}
