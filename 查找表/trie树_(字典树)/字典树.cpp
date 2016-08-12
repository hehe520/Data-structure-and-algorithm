
// trie 树, 数字查找树的一种, 又称字典树
// 主要操作, 插入, 查找, 删除, 
// trie 树结构比较复杂, 相关算法不便文字描述, 详见书上

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>
#include <string.h>

typedef char keytype;
typedef int status;
#define SIZE 100
#define ERROR 0
#define OK 1
#define MAX 27		// 结点的度, 0 号单元保留
// 度设为 27 是因为, 这里只实验 26 个小写英文字母
// 如果要实验大写字母, 可以将 27 改更大, 实验数字也同理

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
result search(trie_tree *tree, keytype *data, node **stack)
{
	result tmp;
	int i = 0;
	int top = 0;

	tmp.tag = 0;   tmp.pos = 0;
	while (data[tmp.pos] != '\0')
	{	
		stack[top++] = tree;
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
	tmp.add = stack[top - 1];
	stack[top] = NULL;		// 类似字符串封口
	tmp.pos--;
	return tmp;
}

// 在树上插入一个结点, 算法见 245 页, 图 9.21
status insert(trie_tree **tree, keytype *data)
{
	result r;
	int i, j;
	keytype buf[70];
	node *stack[SIZE];

	r = search(*tree, data, stack);
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

// 在树中删除一个元素
status remove(trie_tree **tree, keytype *data)
{
	node *stack[SIZE];
	node *cur;
	node *del;
	int top, i;
	result r;

	r = search(*tree, data, stack);
	if (!r.tag) return ERROR;		// 没找到元素

	for (top=0; stack[top]!=NULL; top++);	// 获得栈顶指针
	while (top != 0)
	{
		cur = stack[--top];
		i = data[top] - 'a' + 1;
		// 分支的结点放在, 下一个结点 0 号单元
		if (cur->ptr.add[i]->tag == LEAF)
		{
			free(cur->ptr.add[i]);
			cur->ptr.add[i] = NULL;
			cur->ptr.num--;
		}
		else	// 孩子结点, 不是叶子, 但是度为 0 的也要释放掉
		{		// 度为 1 的, 可能是 0 号单元占用了
			if (cur->ptr.add[i]->ptr.num == 0)
			{
				free(cur->ptr.add[i]);
				cur->ptr.add[i] = NULL;
				cur->ptr.num--;
			}
			else if (cur->ptr.add[i]->ptr.num == 1)
			{
				if (cur->ptr.add[i]->ptr.add[0])	// 0 号占用着
				{
					// 把 0 号的叶子提上来
					del = cur->ptr.add[i];
					cur->ptr.add[i] = cur->ptr.add[i]->ptr.add[0];
					free(del);
				}
			}
			else	// 直接释放 0 号
			{
				if (cur->ptr.add[i]->ptr.add[0])
				{
					free(cur->ptr.add[i]->ptr.add[0]);
					cur->ptr.add[i]->ptr.add[0] = NULL;
					cur->ptr.add[i]->ptr.num--;
				}
			}
		}
	}
	return OK;
}

void show(trie_tree *tree)		// 输出全部树结点
{
	int i;
	for (i=0; i<MAX; i++)
	{
		if (tree->ptr.add[i])	// 是叶子直接输出, 否则继续递归
		{
			if (tree->ptr.add[i]->tag == LEAF)
				printf("%s\n", tree->ptr.add[i]->data);
			else show(tree->ptr.add[i]);
		}
	}
}

char display(trie_tree *tree)	// 显示
{
	char c;

	printf("\n\n树中全部元素: \n");
	show(tree);

	printf("\n按 c 退出, 按回车继续...");
	getchar();
	c = getch();
	system("cls");

	return c;
}

int main(void)
{
	trie_tree *tree;
	char data[50];

	create(&tree);
	while (1)		// 插入测试
	{
		printf("请输入要查找的元素: ");
		scanf("%s", &data);
		if (insert(&tree, data))		// 插入
			printf("\n该元素不存在, 已插入!");
		else printf("\n已找到!");

		if (display(tree) == 'c') break;
	}
	while (1)		// 删除测试
	{
		printf("请输入要删除的元素: ");
		scanf("%s", &data);
		if (remove(&tree, data))		// 删除
			printf("\n成功删除!");
		else printf("\n这个元素不存在!");

		if (display(tree) == 'c') break;
	}
	destory(tree);
	return 0;
}
