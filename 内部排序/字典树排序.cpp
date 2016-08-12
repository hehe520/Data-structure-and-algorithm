#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// 利用 trie 树进行排序,  对字符串进行排序

typedef char keytype;
typedef int status;
#define ERROR 0
#define OK 1
#define MAX 27		// 结点的度, 0 号单元保留

typedef enum {LEAF, BRANCH} kind;	// 标志叶子与分支

// trie 树的储存结构
typedef struct node
{
	kind tag;			// 标志
	union 
	{
		keytype data[MAX];	// 关键字
		struct 
		{
			node *add[MAX];		// 孩子地址
			int num;			// 孩子数量
		}ptr;
	};
}node, trie_tree;

typedef struct		// 搜索结果记录类型
{
	int tag;		// 成功标志
	node *add;		// 找到的结点地址
	int pos;		// 最后匹配位置
}result;

void create(trie_tree **tree)	// 创建 trie 树
{
	int i;

	*tree = (trie_tree *)malloc(sizeof(trie_tree));
	(*tree)->tag = BRANCH;		// 分支
	for (i=0; i<MAX; i++) (*tree)->ptr.add[i] = NULL;
	(*tree)->ptr.num = 0;
}

void destory(trie_tree *tree)	// 销毁 trie 树
{
	int i;
	for (i=0; i<MAX; i++)
	{
		// 如果是叶子, 直接释放, 不是叶子, 递归释放
		if (tree->ptr.add[i])
		{
			if (tree->ptr.add[i]->tag == LEAF)
				free(tree->ptr.add[i]);
			else destory(tree->ptr.add[i]);
		}
	}
	free(tree);
}

node *new_node(kind k)		// 创建一个新结点, 并初始化
{
	node *n;
	int i;

	n = (node *)malloc(sizeof(node));
	for (i=0; i<MAX; i++)
		n->ptr.add[i] = NULL;
	n->tag = k;
	n->ptr.num = 0;
	
	return n;
}

// 在树上查找一个结点, 返回搜索结果, stack 记录搜索路径
// 插入的时候不需要 stack 信息, 但是删除的时候就要了
result search(trie_tree *tree, keytype *data)
{
	result tmp;
	int i = 0;
	int top = 0;

	tmp.tag = 0;   tmp.pos = 0;
	while (data[tmp.pos] != '\0')
	{	
		tmp.add = tree;
		i = data[tmp.pos++] - 'a' + 1;	// 关键字地址
		// 树不为空, 当前是分支结点, 且有孩子
		if (tree && tree->tag==BRANCH && tree->ptr.add[i])
			tree = tree->ptr.add[i];
		else break;
	}
	// 如果有树, 有树的孩子, 且孩子是叶子, 和 data 相等, 那就找到了
	// 如果不是叶子, 就看下一个分支的 0 号单元
	if (tree)
	{
		if (tree->tag == LEAF)
		{
			if (!strcmp(tree->data, data))
				tmp.tag = 1;
		}
		else 
		{
			if (tree->ptr.add[0] && 
				!strcmp(tree->ptr.add[0]->data, data))
				tmp.tag = 1;
		}
	}
	tmp.pos--;
	return tmp;
}

// 在树上插入一个结点, 算法见 245 页, 图 9.21
status insert(trie_tree **tree, keytype *data)
{
	result r;
	int i, j;
	keytype buf[70];

	r = search(*tree, data);
	if (r.tag) return ERROR;	// 存在了

	tree = &r.add;
	while (data[r.pos] != '\0')		// 建立分支
	{
		i = data[r.pos] - 'a' + 1;	// 匹配失败的最后位置
		if ((*tree))			// 看看失败后的原因
		{
			if ((*tree)->tag == LEAF)
			{
				// 如果是叶子, 则改造成分支, 叶子再接到分支 0 位置
				strcpy(buf, (*tree)->data);		// buf记录叶子
				(*tree)->tag = BRANCH;
				(*tree)->ptr.num = 2;
				for (j=0; j<MAX; j++) (*tree)->ptr.add[i] = NULL;
				// 因为是叶子改造过来的, 共用体union结点要初始化
				// 重接叶子
				(*tree)->ptr.add[0] = new_node(LEAF);
				strcpy((*tree)->ptr.add[0]->data, buf);
				tree = &((*tree)->ptr.add[i]);		// 向下走一步
			}
			else 
			{
				(*tree)->ptr.num++;
				tree = &((*tree)->ptr.add[i]);	// 是分支, 直接往下走
			}
		}
		else	// 创建结点
		{
			*tree = new_node(BRANCH);
			(*tree)->ptr.num++;
			tree = &((*tree)->ptr.add[i]);
		}
		r.pos++;
	}
	// 最后再把叶子结点建好
	if (*tree) tree = &((*tree)->ptr.add[0]);
	*tree = new_node(LEAF);
	strcpy((*tree)->data, data);

	return OK;
}

// 先序遍历树, 同时把叶子的值写回 data
void tra(trie_tree *tree, char (*data)[50])
{
	int i;
	static int num = 0;

	for (i=0; i<MAX; i++)
	{
		if (tree->ptr.add[i])	// 是叶子直接输出, 否则继续递归
		{
			if (tree->ptr.add[i]->tag == LEAF)
				strcpy(data[num++], tree->ptr.add[i]->data);
			else tra(tree->ptr.add[i], data);
		}
	}
}

// 字典树排序, 把所有结点插入树中, 进行先序遍历
void trie_sort(char (*data)[50], int n)
{
	trie_tree *tree;
	int i;

	create(&tree);
	for (i=0; i<n; i++) insert(&tree, data[i]);
	tra(tree, data);	// 把树中元素写回 data
	destory(tree);
}

int main(void)
{
	int i, n;
	char data[50][50];

	printf("请输入 n, 及 n 个字符串: ");
	scanf("%d", &n);
	for (i=0; i<n; i++) scanf("%s", data[i]);

	trie_sort(data, n);
	printf("\n排序后: \n");
	for (i=0; i<n; i++) printf("%s\n", data[i]);
	printf("\n");
	return 0;
}