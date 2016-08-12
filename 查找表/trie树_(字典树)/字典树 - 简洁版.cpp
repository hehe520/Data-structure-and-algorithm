#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

// trie树 - 简洁版, 不保存叶子结点信息, 搜索路径即是字符串

typedef char keytype;
typedef int status;
#define ERROR 0
#define OK 1
#define MAX 26		// 只实验小写字母, 所以是 26

// 分支, 叶子, 分支和叶子
typedef enum {BRANCH, LEAF, L_B} kind;

typedef struct node		// trie 树储存
{
	kind tag;			// 标志
	char data;			// 存放一位字符
	node *child[MAX];	// 孩子地址
	int child_num;		// 孩子数
	node *parent;		// 双亲地址
}trie_tree, node;

typedef struct		// 搜索信息记录类型
{
	char tag;		// 成功标志
	node *add;
	int pos;
}result;

void create(trie_tree **tree)    // 创建 trie 树
{
	int i;

	*tree = (trie_tree *)malloc(sizeof(node));
	// 头结点是分支, 并且不存放任何数据
	(*tree)->tag = BRANCH;
	(*tree)->parent = NULL;
	(*tree)->child_num = 0;
	for (i=0; i<MAX; i++)
		(*tree)->child[i] = NULL;
}

void destory(trie_tree **tree)   // 销毁 trie 树
{
	int i;

	for (i=0; i<MAX; i++)		// 类似二叉树的释放
		if ((*tree)->child[i])
			destory( &((*tree)->child[i]) );
	free(*tree);
}

node *new_node(char data, kind k, node *p)	// 创建一个分支结点
{
	node *n;
	int i;

	n = (node *)malloc(sizeof(node));
	n->parent = p;
	n->tag = k;
	n->data = data;
	n->child_num = 0;
	for (i=0; i<MAX; i++) n->child[i] = NULL;
	return n;
}

// 在树上查找一个结点
result search(trie_tree *tree, keytype *data)
{
	result re;
	int cur;

	re.pos = 0;   re.add = tree;
	if (tree)
	{
		while (data[re.pos] != '\0')
		{
			cur = data[re.pos] - 'a';
			if (tree->child[cur])
				tree = tree->child[cur];
			else break;
			re.add = tree;		// 记录当前地址
			re.pos++;
		}
		if (data[re.pos] == '\0' && tree->tag != BRANCH)
			re.tag = 1;
		else re.tag = 0;
	}
	return re;
}

// 插入一个不存在的结点
status insert(trie_tree **tree, keytype *data)
{
	result re;
	int cur;
	kind k;

	re = search(*tree, data);
	if (re.tag) return ERROR;	// 找到了

	// 没找到 data, 把剩下的元素插入到树中
	while (data[re.pos] != '\0')
	{
		cur = data[re.pos] - 'a';
		if (re.add->tag == LEAF) re.add->tag = L_B;

		if (!(re.add->child[cur]))
		{	// 还没有这个结点, 创建并赋值 data[n]
			if (data[re.pos + 1] != '\0') k = BRANCH;
			else k = LEAF;
			re.add->child[cur] = new_node(data[re.pos], k, re.add);
		}
		re.add->child_num++;
		re.add = re.add->child[cur];	// 向孩子走一步
		re.pos++;
	}
	if (re.add->tag == BRANCH) re.add->tag = L_B;

	return OK;
}

// 删除一个结点
status remove(trie_tree **tree, keytype *data)
{
	result re;
	int i;
	node *del;

	re = search(*tree, data);
	if (!re.tag) return ERROR;	// 没找到

	if (re.add->tag == L_B) re.add->tag = BRANCH;
	else if (re.add->tag == LEAF)
	{
		del = re.add;
		while (re.add->parent)	// 释放 re.add
		{
			re.add = re.add->parent;
			for (i=0; i<MAX; i++)
			{
				if (re.add->child[i])
				{
					if (re.add->child[i] == del)
					{
						re.add->child[i] = NULL;
						re.add->child_num--;
					}
					else if (re.add->child[i]->child_num == 0 
						&& re.add->child[i]->tag == BRANCH)
					{
						free(re.add->child[i]);
						re.add->child[i] = NULL;
					}
					// 释放没有孩子的结点
				}
			}	
		}
		free(del);
	}
	return OK;
}

// 递归深搜, 输出所有结点
void show(trie_tree *tree)
{
	int i;
	static char stack[60];	// 记录字符串
	static int top = 0;

	for (i=0; i<MAX; i++)
	{
		if (tree->child[i])
		{
			stack[top++] = tree->child[i]->data;
			if (tree->child[i]->tag == LEAF 
				|| tree->child[i]->tag == L_B)
			{
				stack[top] = '\0';
				printf("%s\n", stack);
			}
			show(tree->child[i]);
			top--;		// 回溯的时候, 栈指针也要回来一步
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
	destory(&tree);
	return 0;
}
