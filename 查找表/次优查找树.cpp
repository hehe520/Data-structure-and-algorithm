
// 次优查找树, 用二叉链表保存

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

typedef char keytype;	// 用 char 代替 keytype
typedef int status;
#define OK 1
#define ERROR 0


// 次优查找树的储存结构
typedef struct sostree
{
	keytype data;
	sostree *lchild;
	sostree *rchild;
}sostree, snode;


// 建树的递归部分, sw[] 保存权值和, low 至 high
status create_tree(sostree **tree, keytype *str, 
				   int *sw, int low, int high)
{
	int dw = sw[high] + sw[low - 1];	// 这也是动态规划的思想
	int i, p;		// p 是平衡因子
	int min, pos;

	// 找最小的平衡因子 pos
	pos = low;							// 防止找不到的情况
	min = abs(sw[high] - sw[low]);		// min = INT_MAX 也是可以的
	for (i=low + 1; i<=high; i++)		// 查找区间 [low+1..high]
	{
		// p 的作用类似天平的平衡因子
		// p 是求左子树 - 右子树的绝对差值
		// 差值最小的 p 就是左右子树权值和差不多一样的情况
		// 只是次优查找树, 不是最优
		p = abs(dw - sw[i] - sw[i-1]);
		if (p < min) { min = p;   pos = i; }
	}
	*tree = (snode *)malloc(sizeof(snode));
	(*tree)->data = str[pos - 1];

	// 划分区间, 左孩子 [low..pos-1], 右孩子 [pos+1..high]
	if (pos == low) (*tree)->lchild = NULL;
	else create_tree( &((*tree)->lchild), str, sw, low, pos - 1 );
	if (pos == high) (*tree)->rchild = NULL;
	else create_tree( &((*tree)->rchild), str, sw, pos + 1, high );

	return OK;
}

// 根据元素和权值, 在 tree 上建立次优查找树
status create(sostree **tree, keytype *str, int *wight)
{
	int len = strlen(str);
	int *sw;	// sw[i + 1] 保存前 i 项和, sw[0] 保留
	int i;

	if (!len) return ERROR;

	sw = (int *)malloc((len + 5) * sizeof(int));
	sw[0] = 0;
	for (i=1; i<=len; i++)		// 求前 n 项权值, 动态规划的思想
		sw[i] = sw[i-1] + wight[i-1];
	
	create_tree(tree, str, sw, 1, len);
	free(sw);
	return OK;
}

// 查找 ch 元素
status search(sostree *tree, keytype ch)
{
	if (!tree) return ERROR;

	if (tree->data == ch) return OK;
	else if (ch < tree->data)
		return search(tree->lchild, ch);
	else 
		return search(tree->rchild, ch);
	return ERROR;
}

void input(keytype *str, int *wight)	// 输入模块, 以 0 0 结束
{
	int i;

	for (i=0; i<100; i++)
	{
		scanf("%c %d", str + i, wight + i);
		getchar();
		if (str[i] == '0' && wight[i] == 0) 
		{
			str[i] = '\0';    wight[i] = -1;	// 结束标记
			break;
		}
	}
	fflush(stdin);
}

void destory(sostree *tree)		// 销毁树
{
	if (tree)
	{
		destory(tree->lchild);
		destory(tree->rchild);
		free(tree);
	}
}

int main(void)
{
	sostree *tree;		// 树根
	keytype str[100];	// 记录结点
	int wight[100];		// 结点对应的权值
	char c;

	printf("构造次优查找树\n请输入元素及权值:\n");
	input(str, wight);
	create(&tree, str, wight);	// 根据结点权值建树

	while (1)
	{
		printf("\n输入查找数据: ");
		scanf("%c", &c);   getchar();
		if (c == '\n') break;
		if (search(tree, c)) printf("找到!\n");
		else printf("错误: 未找到!\n");
	}
	destory(tree);
	return 0;
}
/*		两组测试数据
A 1
B 1
C 2
D 5
E 3
F 4
G 4
H 3
I 5
0 0


A 1
B 30
C 2
D 29
E 3
0 0
*/