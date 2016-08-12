// 哈夫曼树及编码 (haffman)

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

typedef short int status;
#define NO -1		// NO 表示没有

// 哈夫曼树储存结构
// 对于严格的二叉树采用顺序储存
typedef struct
{
	int wight;		// 权值
	int lchild;		// 左孩子 0
	int rchild;		// 右孩子 1
	int parent;		// 双亲
}htree;		// 动态分配空间


// 初始化各个成员
inline void init_member(htree **tree, int i)
{
	(*tree)[i].parent = NO;
	(*tree)[i].lchild = NO;
	(*tree)[i].rchild = NO;
}

// 输入模块, 包括权值
void input(char *str, int *w)
{
	int len, i;

	// 输入汉字也是可以的, 因为一个汉字占两个字节
	// 编码的时候, 两个字符的编码表示一个汉字
	scanf("%s", str);
	len = strlen(str);
	for (i=0; i<len; i++)
	{
		// 权值越大的编码越短
		printf("请输入 %c 的权值: ", str[i]);
		scanf("%d", w + i);
	}
	fflush(stdin);
	system("cls");
}

// 销毁哈夫曼树及码表
void destory(htree *tree, char **table, int n)
{
	int i;

	if (tree) free(tree);
	for (i=0; i<n; i++)		// 类似二维数组的释放
		if (table[i]) free(table[i]);
	free(table);
}

// 递归法, 求树的带权路径长度, size 树的大小, flood 层数
// 如果是叶子, 返回路径长度
// 如果不是, 求左右孩子的长度
int WPL(htree *tree, int root, int flood)
{
	int len = 0;

	if (tree[root].lchild == NO 
		&& tree[root].rchild == NO)
			return flood * tree[root].wight;

	len += WPL(tree, tree[root].lchild, flood + 1);
	len += WPL(tree, tree[root].rchild, flood + 1);

	return len;
}

// 把 buf 转码到 result 中, 借助哈夫曼表和字符
// 首先在 str 中找到 *buf 的位序 pos
// 将 table 中第 pos 个字符串复制到 result 中
status encode(char **table, char *str, char *buf, char *result)
{
	int cur = 0;
	int pos;
	char *s = str;

	while (*buf != '\0')
	{
		// 在 str 中找当前字符
		pos = 0;
		while (s[pos] != '\0')
		{
			if (s[pos] == *buf) break;
			pos++;
		}
		if (s[pos] == '\0') return 0;	// 没找到字符

		// 将 table[pos] 复制到 result 中
		strcpy(result + cur, table[pos]);
		while (result[cur] != '\0') cur++;   // 推后 cur
		buf++;
	}
	return 1;
}

// 将密文 result 通过哈夫曼树 tree(长度 n) 还原成明文 buf
status decode(htree *tree, char *str, char *result, char *buf)
{
	int n;
	int cur = 0;
	int pos = 0;
	int root;
	
	// n 个字符有 (2n-1) 个结点, 下标少 1
	n = strlen(str);   root = 2 * n - 2;

	for (pos=0; result[pos] != '\0'; pos++)
	{
		if (result[pos] == '0')
			root = tree[root].lchild;	// 往左走
		else if (result[pos] == '1')
			root = tree[root].rchild;	// 往右走
		else return 0;   // 密文错误

		// 如果左右孩子同时为 0 说明到了叶子结点
		if (tree[root].lchild == NO 
			&& tree[root].rchild == NO)
		{
			buf[cur++] = str[root];
			root = 2 * n - 2;	// 重新从根开始走
		}
	}
	buf[cur] = '\0';
	return 1;
}

// --------------哈夫曼编码主要函数----------------
// 选择一个权值最小, 且没有访问过的结点
// 如果双亲为 NO 刚好是没有访问过的
inline int select(htree **tree, int n)
{
	int i;
	int wight = 0xFFFFFFF;
	int min;

	for (i=0; i<n; i++)
		if ((*tree)[i].parent == NO 
			&& (*tree)[i].wight < wight)
			{
				wight = (*tree)[i].wight;	// 记录最小的权值
				min = i;		// 以及它的地址
			}
	(*tree)[min].parent = -2;		// 防止下次再选到同样的
	return min;
}

// 从根到叶子, 获得哈夫曼编码, 无栈非递归
// 算法:
// 对每个结点, 设置flag[i] 状态标记
// 0 表示要访问左孩子, 1 表示要访问右孩子, 2 表示都访问过了
status create_table2(htree **tree, char ***table, int n, int r)
{
	char *buf;
	int cur;
	short int *flag;	// 访问状态标记数组

	*table = (char **)malloc(n * sizeof(char *));	// 分配指针数组
	buf = (char *)malloc((n+3) * sizeof(char));
	flag = (short int *)malloc((r+1) * sizeof(short int));
	if (!((*table) && buf && flag)) return 0;

	for (cur=0; cur<=r; cur++) flag[cur] = 0;
	cur = 0;   r--;		// r 是大小, 比下标多 1
	while (r != NO)
	{
		if (flag[r] == 0)   // 0 表示左孩子没访问
		{
			flag[r] = 1;
			if ((*tree)[r].lchild != NO)
			{
				r = (*tree)[r].lchild;
				buf[cur++] = '0';
			}
			else if ((*tree)[r].rchild == NO)
			{
				buf[cur] = '\0';
				(*table)[r] = (char *)malloc((cur+1) * sizeof(char));
				strcpy((*table)[r], buf);
			}
		}
		else if (flag[r] == 1)  // 1 表示右孩子没访问
		{
			flag[r] = 2;
			if ((*tree)[r].rchild != NO)
			{
				r = (*tree)[r].rchild;
				buf[cur++] = '1';
			}
		}
		else	// 2 表示左右孩子都访问了, 退回去
		{
			r = (*tree)[r].parent;
			cur--;
		}
	}
	free(buf);   free(flag);
	return 1;
}

// 根据哈夫曼树 tree, 生成码表到 table
// 其中叶子 n 个, 非叶子 size - n 个
// 采用从叶子走到根的方法获得编码
status create_table(htree **tree, char ***table, int n, int size)
{
	int i;
	int son, p;
	char *buf;		// 记录编码
	int cur;

	// 为码表申请空间
	(*table) = (char **)malloc(n * sizeof(char *));
	if (!(*table)) return 0;

	// n 个字符的最大编码长度不超过 n - 1
	buf = (char *)malloc(n * sizeof(char));
	buf[n - 1] = '\0';

	for (i=0; i<n; i++)		// 遍历每片叶子
	{
		cur = n - 1;
		son = i;
		p = (*tree)[son].parent;
		while (p != NO)
		{
			if ((*tree)[p].lchild == son) 
				buf[--cur] = '0';	// 左孩子为 0
			else 
				buf[--cur] = '1';
			son = p;
			p = (*tree)[p].parent;
		}
		// 完成一个字符的编码, 记录到表中, 下面类似行指针
		(*table)[i] = (char *)malloc((size - cur) * sizeof(char));
		if (!(*table)[i]) return 0;
		strcpy((*table)[i], buf + cur);
	}
	free(buf);
	return 1;
}

// 该函数完成动态生成哈夫曼树, 和生成码表
// 在 tree 上构造哈夫曼树, 编码保存到 table 中, w 是权值
status haffman_code(htree **tree, char ***table, int *w, int n)
{
	int size;
	int i;
	int s1, s2;

	// ------申请空间和初始化各个变量
	size = 2 * n - 1;	// 对于 n 个字符的树, 需要 2n-1 个结点
	*tree = (htree *)malloc(size * sizeof(htree));
	if (!(*tree)) return 0;

	// 树的空间说明  [0..n-1], [n..size-1]
	// 0 到 n-1 是叶子结点的,  n 到 size 是连接叶子结点的
	for (i=0; i<n; i++)		// 初始化叶子[0..n-1]
	{
		(*tree)[i].wight = w[i];	// 初始化权值
		init_member(tree, i);
	}
	for (i=n; i<size; i++)   // 初始化非叶子结点[n..size-1]
	{
		(*tree)[i].wight = NO;
		init_member(tree, i);
	}

	// ------动态构造哈夫曼树 htree
	for (i=n; i<size; i++)	// 非叶子[n..size-1]
	{
		// 在[0..i-1]中选两个 权值最小且没有访问过 的的结点
		s1 = select(tree, i);
		s2 = select(tree, i);

		(*tree)[i].lchild = s1;		// s1, s2 连接起来
		(*tree)[i].rchild = s2;
		(*tree)[s1].parent = (*tree)[s2].parent = i;
		(*tree)[i].wight = (*tree)[s1].wight + (*tree)[s2].wight;
	}
	
	// ------生成哈夫曼码表 到 table, 以下两个函数皆可
	create_table(tree, table, n, size);
//	create_table2(tree, table, n, size);

//  create_table() 是从叶子 到 根求编码
//  create_table2() 是从根 到 叶子求编码
	return 1;
}

int main(void)
{
	char str[60];	// 储存要编码的字符
	int w[60];		// 对应的权值
	htree *tree;	// 哈夫曼树
	char **table;	// 哈夫曼编码表 (二维数组)
	// str[i] 对应 tree[i] 树 和 table[i] 的编码

	int len, i;
	char buf[100];
	char result[200];

	printf("\n--- 哈夫曼编码\n\n--- 请输入要编码的字符: ");
	input(str, w);

	len = strlen(str);
	haffman_code(&tree, &table, w, len);	// 开始哈夫曼编码
	i = WPL(tree, 2 * len - 2, 0);			// 求 WPL
	printf("\n--- 哈夫曼树的 WPL = %d\n", i);
	printf("\n--- 哈夫曼码表如下:\n\n");
	for (i=0; i<len; i++)
		printf(" %c : %s\n", str[i], table[i]);

	printf("\n输入字符串 转换成 哈夫曼编码:");
	scanf("%s", buf);	fflush(stdin);
	if (encode(table, str, buf, result))	// 开始转码
		printf("\n密文如下: %s\n", result);
	else printf("\n明文错误!\n");

	printf("\n输入哈夫曼编码 转换成 字符串:");
	scanf("%s", result);	fflush(stdin);
	if (decode(tree, str, result, buf))		// 开始解码
		printf("\n明文如下: %s\n\n", buf);
	else printf("\n密文错误!\n\n");

	destory(tree, table, len);

	return 0;
}
