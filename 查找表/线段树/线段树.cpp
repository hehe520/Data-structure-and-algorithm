
// 线段树的基本操作
// create()		构造线段树
// destory()	销毁线段树
// insert()		插入区间
// remove()		删除区间
// query()		查询结点覆盖次数
// sum()		查询区间所有和
// max()		查询区间最大值


// 举个简单的例子, 广州到珠海的轻轨售票系统
// 该系统区间是 [广州 - 珠海]
// 插入区间, 就是买票, 比如买了一张 [珠海 - 中山] 的票
// 删除区间, 就是退票, 前提是要买了这张票, 否则退票失败
// 查询区间最大值, 哪个站售的票最多
// 查询区间所有和, 从哪到哪一共卖了几张票
// 查询结点覆盖次数, 比如查询中山站, 此时列车上有几张票
// 票数不能超过列车总座位数, 座位数减出售票数就是剩余票数

// 算法性能分析
// 出售了 n 张票, 有 m 个车站要查询剩余票量
// 普通做法是判断 n 张车票, 有几张覆盖了第 k 个车站, 复杂度 O(m*n)
// 结点数为 m 的线段树, 深度不超过 log m, 一次查询走 log m 次
// n 张票查询, 所以复杂度是 O(n log m)


#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

typedef int status;
#define OK 1
#define ERROR 0

// 线段树用二叉链表储存, 像堆一样, 用数组也可, 因为是完全二叉树
typedef struct node 
{
	int left, right;
	int num;		// 记录次数
	node *lchild;
	node *rchild;
}node, itree;


// 构造线段树, [left..right]
void create(itree **tree, int left, int right)
{
	int mid;

	if (left > right) return ;
	
	*tree = (node *)malloc(sizeof(node));
	(*tree)->lchild = (*tree)->rchild = NULL;
	(*tree)->left = left;
	(*tree)->right = right;
	(*tree)->num = 0;

	if (left != right)		// 左右相等了, 就是一个结点了
	{
		mid = (left + right) / 2;
		create( &((*tree)->lchild), left, mid);
		create( &((*tree)->rchild), mid+1, right);
	}
}

void destory(itree *tree)	// 销毁线段树
{
	if (tree)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

// 在树上插入一个区间, 及插入次数 num
// 若区间匹配, 则记录数 +1
// 否则继续向分割区间, 直到放到合适的结点上
void insert(itree *tree, int left, int right, int num)
{
	int mid;

	if (tree->left == left && tree->right == right)
		tree->num += num;	// 匹配成功
	else 
	{	
		if (!(tree->lchild && tree->rchild)) return ;
		// 匹配失败, 继续分割
		mid = (tree->left + tree->right) / 2;
		if (right <= mid)   // 该区间全在左边
		{
			insert(tree->lchild, left, right, num);
		}
		else if (left > mid)  // 该区间全在右边
		{
			insert(tree->rchild, left, right, num);
		}
		else
		{	// 左右都有, 要分割 [left, mid], [mid+1, right]
			insert(tree->lchild, left, mid, num);
			insert(tree->rchild, mid+1, right, num);
		}
	}
}

// 删除一个结点的算法和插入的很相似, 可以用一个函数完成
void remove(itree *tree, int left, int right, int num)
{
	int mid;

	// 匹配到成功的区间后, 可以做任何事情
	if (tree->left == left && tree->right == right)
	{
		if (tree->num >= num) tree->num -= num;
		else
		{	// 此时 tree->num < num
			// 这里没写代码, 如果拿售票系统来说
			// 这里就是票数不足的情况, 提示退票失败!
		}
	}
	else 
	{	// 没孩子了
		if (!(tree->lchild && tree->rchild)) return ;
		mid = (tree->left + tree->right) / 2;
		if (right <= mid)
			remove(tree->lchild, left, right, num);
		else if (left > mid)
			remove(tree->rchild, left, right, num);
		else 
		{	// 分割区间
			remove(tree->lchild, left, mid, num);
			remove(tree->rchild, mid+1, right, num);
		}
	}
}

// 查询 n 的覆盖数, 从根往下走, 统计 num 和, O(log n)
int query(itree *tree, int n)
{
	int sum = 0;
	int mid;

	if (!tree) return 0;
	mid = (tree->left + tree->right) / 2;
	if (n <= mid)	// 左孩子中
		sum += query(tree->lchild, n);
	else		// 右孩子中
		sum += query(tree->rchild, n);

	return sum + tree->num;  // 加上当前结点的 num
}

// 统计 [left..right] 所有覆盖次数, 也就是统计票数 
int sum(itree *tree, int left, int right)
{
	int re = 0;
	int i;

	for (i=left; i<=right; i++)
		re += query(tree, i);
	return re;
}

// pos, cnt 返回位置和最大次数
void max(itree *tree, int left, int right, int *pos, int *cnt)
{
	int i;
	int tmp;

	*pos = 0;
	*cnt = -0xFFFFFF;
	for (i=left; i<=right; i++)
	{
		tmp = query(tree, i);
		if (tmp > *cnt) 
		{  *cnt = tmp;   *pos = i;  }
	}
}

void show(itree *root, itree *tree)	// 查看所有结点
{
	if (tree)
	{
		printf("[%d, %d] ", tree->left, tree->right);
		printf("记录数 %d\n", tree->num);
		show(root, tree->lchild);
		show(root, tree->rchild);
	}
}

inline char display(itree *tree)	// 显示
{
	char c;
	int i;

	getchar();
	show(tree, tree);
	for (i=tree->left; i<=tree->right; i++)
		printf("\n %d 的覆盖次数 %d", i, query(tree, i));
	printf("\n\n按 c 退出, 回车继续...\n");
	c = getch();   system("cls");
	return c;
}

int main(void)
{
	itree *tree = NULL;
	int left, right, n;
	int cnt, pos;

	printf("请输入线段树区间: ");
	scanf("%d %d", &left, &right);
	create(&tree, left, right);
	while (1)		// 插入测试, 买票
	{
		printf("请插入区间及插入次数: ");
		scanf("%d %d %d", &left, &right, &n);
		insert(tree, left, right, n);
		if (display(tree) == 'c') break;
	}
	while (1)		// 删除测试, 退票
	{
		printf("请要删除区间及删除次数: ");
		scanf("%d %d %d", &left, &right, &n);
		remove(tree, left, right, n);
		if (display(tree) == 'c') break;
	}
	while (1)		// 查询结点覆盖次数, 出售票数
	{	// 设一趟列车, 有 300 个座位, 那么最大覆盖数就是 300
		// 若覆盖数等于 300, 就提示票已售完!
		printf("请输入要查询的结点: ");
		scanf("%d", &n);
		printf("\n覆盖数: %d\n", query(tree, n));

		printf("\n按 c 退出, 回车继续...\n");
		if (getch() == 'c') {system("cls");   break;}
		system("cls");
	}
	printf("查询区间所有和, 输入一个区间: ");	// 统计票数
	scanf("%d %d", &left, &right);
	printf("\n区间所有和: %d\n", sum(tree, left, right));

	// 查询区间最大值, 最小值也同理,
	printf("\n查询区间最大值, 输入一个区间: ");
	scanf("%d %d", &left, &right);
	max(tree, left, right, &pos, &cnt);
	printf("\n最大覆盖的结点: %d    覆盖数: %d\n\n", pos, cnt);

	destory(tree);
	return 0;
}
