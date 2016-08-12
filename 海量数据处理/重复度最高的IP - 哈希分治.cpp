
// 在海量数据中找出出现次数最高的前 10 个 IP
// 算法思想, 分治法
// IP 记录文件大小 10 GB, 约 10 亿个 IP, 内存限制 64 MB
// 将 IP 记录文件中的 IP 对 500 取模, 散列到 500 个小文件中
// 相同的 IP 会被散列到同一个文件中
// 平均每个小文件 20 MB, 在 64 MB 之内
// 再在每个小文件中用二叉树统计每个 IP 的次数

// 遍历二叉树返回最大的 IP 和出现次数
// 因为 IP 有 2^32 个, 要保证能产生重复的 IP 需要生成 2^32 个 IP
// 约要 64 GB 的文件, 为了保证 10 GB 的文件中也能产生重复 IP 
// 将 IP 段设置为 0.0.0.0 - 100.100.100.100
// fopen() 最多只能打开 507 个文件

// 后记: 将主记录文件分解成 500 个小文件后 (分)
// 可以将这些小文件通过网络发送给其他计算机计算 (治)
// 再将结果返回给主程序, 即类似 google 的 MapReduce 工作原理

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <io.h>			// access()
#include <string.h>

const int num = 1000000;		// 10 亿, 手动修改 num

unsigned int iton(char *ip);		// 点分记法的 IP 地址转为整型
void ntoi(unsigned int num, char *ip);
int  fileexist(char *path);			// 判断文件存在
void fclose_all(FILE **t);			// 关闭所有文件
int  random_write(char *path);		// 随机生成 6.7 亿个 IP, 约 10 GB
// 统计 hashfile 中次数最多的 IP
void count(char *hashfile, unsigned int *data, unsigned int *num);
void sort(unsigned int *max, unsigned int *ip, int n);		// 排序
inline unsigned int hash(unsigned int ip)				// 哈希函数
{ return (ip % 500); }

typedef struct node		// 二叉树结点
{
	unsigned int ip;	// IP
	unsigned int n;		// 出现次数
	node *left;
	node *right;
}node;

int main(void)
{
	FILE *in = NULL;
	FILE *tmpfile[505];
	char *path = "c:\\ip_data.dat";
	char hashfile[50];
	char buf[20];
	unsigned int add, data, n;
	unsigned int ip[10], max[10];	// 记录前十的 IP
	unsigned int t1, t2, s, e;		// 记录时间
	int i, j, len, now;			// IP 个数

	printf("正在生成数据 %s\n\n", path);
	if (!random_write(path)) return 0;	// 随机生成 IP 记录文件

	// 判断文件夹是否存在, access() == 0 存在
	if (access("c:\\hashfile", 0) == 0) 
		system("rmdir /s /q c:\\hashfile");
	system("mkdir c:\\hashfile");		// 建立工作目录
	system("attrib +h c:\\hashfile");	// 隐藏目录

	in = fopen(path, "rt");			// 打开 IP 记录文件
	if (in == NULL) return 0;
	for (i=0; i<505; i++) tmpfile[i] = NULL;

	// 将 6.7 亿个 IP 散列到 505 个小文件中
	printf("\r正在哈希 %s\n\n", path);
	e = s = t1 = clock();
	now = 0;
	while (fscanf(in, "%s", buf) != EOF)
	{
		data = iton(buf);		// IP 数字化
		add = hash(data);		// 计算哈希地址, 文件地址
		sprintf(hashfile, "c:\\hashfile\\hash_%u.~tmp", add);
		if (tmpfile[add] == NULL)
			tmpfile[add] = fopen(hashfile, "a");
		sprintf(buf, "%u\n", data);
		len = strlen(buf);
		// 将该 IP 写入到文件中, 反复读写磁盘慢
		fwrite(buf, len, 1, tmpfile[add]);
		now++;
		e = clock();
		if (e - s > 1000)		// 计算进度
		{
			printf("\r处理进度 %0.2f %%\t", (now * 100.0) / num);
			s = e;
		}
	}
	fclose(in);
	fclose_all(tmpfile);
	remove(path);

	// 在每个小文件中统计重复度最高的 IP, 可以用多台计算机处理
	for (i=0; i<10; i++) max[i] = 0;
	for (i=0; i<500; i++)
	{
		sprintf(hashfile, "c:\\hashfile\\hash_%d.~tmp", i);
		if (fileexist(hashfile))
		{
			printf("\r正在处理 hash_%d.~tmp\t", i);
			// 统计该小文件中的次数最多的 IP
			count(hashfile, &data, &n);
			// 因为只有 10 个元素, 用选择排序的思想记录最大的 10 个
			// 如果元素过多, 可以用插入排序的思想找或者用堆
			unsigned int min = 0xFFFFFFFF, pos;
			for (j=0; j<10; j++)
			{
				if (max[j] < min)
				{
					min = max[j];
					pos = j;
				}
			}
			if (n > min)
			{
				max[pos] = n;
				ip[pos] = data;
			}
		}
	}
	t2 = clock();
	sort(max, ip, 10);

	FILE *log = NULL;		// 同时在 C:\ip_result.txt 记录结果
	log = fopen("C:\\ip_result.txt", "wt");
	fprintf(log, "\n访问次数最高的前 10 个 IP:\n\n");
	fprintf(log, "     %-15s%s\n", "IP", "访问次数");
	printf("\n\n访问次数最高的前 10 个 IP:\n\n");
	printf("     %-15s%s\n", "IP", "访问次数");
	for (i=0; i<10; i++)
	{
		ntoi(ip[i], buf);		// 解码
		printf("   %-20s%u\n", buf, max[i]);
		fprintf(log, "   %-20s%u\n", buf, max[i]);
	}
	fprintf(log, "\n--- 用时 %0.3f 秒\n", (t2 - t1) / 1000.0);
	printf("\n--- 用时 %0.3f 秒\n\n", (t2 - t1) / 1000.0);
	fclose(log);
	system("rmdir /s /q c:\\hashfile");

	return 0;
}

void fclose_all(FILE **t)		// 关闭所有文件
{
	int i;

	for (i=0; i<500; i++)
	{
		if (t[i])
		{
			fclose(t[i]);
			t[i] = NULL;
		}
	}
}

// 随机生成 6.7 亿个 IP, 约 10 GB
int random_write(char *path)
{
	FILE *out = NULL;
	int i, j, b;
	char buf[20];
	char *cur;
	unsigned int s, e;

	out = fopen(path, "wt");
	if (out == NULL) return 0;
	srand(time(NULL));
	e = s = clock();
	for (i=0; i<num; i++)
	{
		e = clock();
		if (e - s > 1000)		// 计算进度
		{
			printf("\r处理进度 %0.2f %%\t", (i * 100.0) / num);
			s = e;
		}
		for (j=0; j<20; j++) buf[j] = '\0';
		cur = buf;
		for (j=0; j<4; j++) 
		{
			// 点分记法中应该产生 0-255 的数值
			// 这里产生 0-101 的数值
			b = rand() % 101;
			sprintf(cur, "%d.", b);
			while (*cur != '\0') cur++;
		}
		*(cur - 1) = '\n';
		fwrite(buf, cur-(char *)buf, 1, out);
	}
	fclose(out);		// 记得要关闭
	return 1;
}

// 二叉树的插入
void insert(node **tree, unsigned int ip)
{
	if ((*tree) == NULL)
	{
		// new_node
		(*tree) = (node *)malloc(sizeof(node));
		(*tree)->ip = ip;
		(*tree)->n = 1;
		(*tree)->left = (*tree)->right = NULL;
	}
	else if ((*tree)->ip == ip)
	{
		(*tree)->n++;
		return ;
	}
	else if (ip < (*tree)->ip)		// 左子树
		insert(&((*tree)->left), ip);
	else insert(&((*tree)->right), ip);	// 右子树
}

unsigned int maxn;		// 入口参数
node *max_node;			// 出口参数
void max_n(node *tree)	// 找出树中 n 最大的结点
{
	if (tree)
	{
		if (tree->n > maxn)
		{
			maxn = tree->n;
			max_node = tree;
		}
		max_n(tree->left);
		max_n(tree->right);
	}
}

void destory(node *tree)		// 释放树
{
	if (tree)
	{
		destory(tree->left);
		destory(tree->right);
		free(tree);
	}
}

// 统计 hashfile 中次数最多的 IP
void count(char *hashfile, unsigned int *data, unsigned int *n)
{
	FILE *in = NULL;
	node *tree = NULL;
	unsigned int ip;

	in = fopen(hashfile, "rt");
	while (fscanf(in, "%d", &ip) != EOF)
	{
		insert(&tree, ip);
	}
	fclose(in);
	maxn = 0;
	max_n(tree);	// 结果在 max_node
	*n = max_node->n;
	*data = max_node->ip;
	destory(tree);
}

// 插入排序
void sort(unsigned int *max, unsigned int *ip, int n)
{
	int i, j;
	unsigned int tmpm, tmpi;

	for (i=1; i<n; i++)
	{
		if (max[i-1] < max[i])
		{
			tmpm = max[i];
			tmpi = ip[i];
			for (j=i; j>0; j--)
			{
				if (max[j-1] < tmpm)
				{
					max[j] = max[j-1];
					ip[j]  = ip[j-1];
				}
				else break;
			}
			max[j] = tmpm;
			ip[j]  = tmpi;
		}
	}
}

// 将字符串的 IP 地址转为整型
unsigned int iton(char *ip)
{
	unsigned int r = 0;
	unsigned int t;
	int i, j;

	for (j=i=0; i<4; i++)
	{
		sscanf(ip + j, "%d", &t);
		if (i < 3)
			while (ip[j++] != '.');
		r = r << 8;
		r += t;
	}
	return r;
}

// 将整型 num 转为字符型 IP
void ntoi(unsigned int num, char *ip)
{
	unsigned int b, f;
	int i, cur;

	f = 0x00FFFFFF;
	cur = 0;
	for (i=3; i>=0; i--)
	{
		b = num >> (i * 8);
		num = num & f;
		f = f >> 8;
		sprintf(ip + cur, "%u.", b);
		while (ip[cur] != '\0') cur++;
	}
	ip[cur - 1] = '\0';
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
