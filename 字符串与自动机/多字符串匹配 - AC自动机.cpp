//
// 基于字典树的多模式匹配算法之 AC 自动机
//
// 算法流程:
// 1. 根据多个模式串构造一棵字典树
// 2. 连接字典树的失败指针, 这棵树即是 AC 自动机
// 3. 将主串在自动机上进行匹配, 并记录结果
//
// 简单来说就是, 在一篇文章中找很多个单词
// 多次使用 KMP 算法也可以, 但是太慢了
// 该算法类似 KMP, 可以这样想, KMP 在线性的结构上建立自动机
// 而该算法在树形结构上建立自动机
// 下次遇到在网状结构上建立自动机的算法也同理
// 
// 举个例子, 有以下模式串 aash, she, he
// 失败指针如下, 其作用类似 kmp 的 next[] 数组, 其结构类似线索二叉树
// a  a  s  h 
//       |  |
//       s  h  e 
//          |  |
//	        h  e     将这个结构转为字典树的即是自动机


#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
	node *parent;		// 双亲地址
	node *fail;			// 失败指针
	int num;			// 该串出现的次数
}trie_tree, node;

typedef struct		// 搜索记录记录类型
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
	(*tree)->fail = NULL;
	(*tree)->num = 0;
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
	n->parent = p;   n->tag = k;      n->data = data;
	n->num = 0;      n->fail = NULL;
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
		re.add = re.add->child[cur];	// 向孩子走一步
		re.pos++;
	}
	if (re.add->tag == BRANCH) re.add->tag = L_B;
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
			if (tree->child[i]->tag != BRANCH)
			{
				stack[top] = '\0';
				printf("%s\t", stack);
				if (tree->child[i]->num)
					printf("出现 %d 次\n", tree->child[i]->num);
				else printf("未找到!\n");
			}
			show(tree->child[i]);
			top--;		// 回溯的时候, 栈指针也要回来一步
		}
	}
}

void input(char *re[], int n)	// 输入模块
{
	int i, len;
	char buf[100];

	fflush(stdin);
	for (i=0; i<n; i++)
	{
		scanf("%s", buf);   getchar();
		len = strlen(buf);
		re[i] = (char *)malloc((len+2) * sizeof(char));
		strcpy(re[i], buf);
	}
}

// 进行层次遍历, 对每个结点建立失败指针
// 方法: 对于一个结点 k, 沿着 k 双亲的失败指针走
// 因为双亲相同的结点才不用再匹配双亲了
// 若发现双亲的孩子中也有 k 结点, 则失败指针指向他
// 表示匹配失败后, 从这里继续匹配
// 直到失败指针走完了, 把最后一个孩子结点, 指到根上
// 表示要重新从根开始匹配
void create_fail_point(trie_tree *tree)
{
	node *queue[100];		// 队列层次遍历
	int front = 0;
	int rear = 0;
	node *tmp;
	node *cur;		// 当前失败指针
	int i;

	tree->fail = NULL;		// 设根没有失败指针
	queue[rear] = tree;		// 根入队
	rear = (rear + 1) % 100;
	while (front != rear)
	{
		tmp = queue[front];
		front = (front + 1) % 100;
		for (i=0; i<MAX; i++)		// 遍历孩子, 入队
		{
			if (tmp->child[i])
			{
				queue[rear] = tmp->child[i];	// 先入队记录
				rear = (rear + 1) % 100;
				cur = tmp;			// 取得双亲, 修改失败指针
				if (cur == tree) tmp->child[i]->fail = tree;
				else 
				{	// cur 是失败指针指向的结点
					while (cur->fail)
					{
						cur = cur->fail;
						if (cur->child[i]) 
						{
							tmp->child[i]->fail = cur->child[i];
							break; 
						}
						// 只连接一条即可
					}
					if (tmp->child[i]->fail == NULL)
						tmp->child[i]->fail = tree;
				}
			}
		} // end for
	} // end while
}

// 用主串进行模式匹配, 并记录 num 成员
void find(trie_tree *tree, char *str)
{
	int i = 0;
	int pos;
	node *cur = tree;
	node *tmp;

	while (str[i] != '\0')
	{
		pos = str[i] - 'a';
		// 没有这个孩子就往失败指针走, 直到走到根
		while (cur != tree && !(cur->child[pos])) cur = cur->fail;
		// 有这个孩子就往下走一步
		if (cur->child[pos]) cur = cur->child[pos];
		else cur = tree;		// 没有这个孩子, 从根重新匹配
		tmp = cur;
		while (tmp != tree)		// 去到失败的地方, 匹配相同的
		{
			if (tmp->tag != BRANCH) tmp->num++;   // 走到了叶子
			tmp = tmp->fail;
		}
		i++;
	}
}

int main(void)
{
	trie_tree *tree;
	char *re[50];
	char main_str[1000];	 // 主串根据实际需要可以更大
	int n;

	create(&tree);
	printf("输入 n, 及 n 个模式串: ");
	scanf("%d", &n);
	input(re, n);
	printf("\n输入主串: ");
	scanf("%s", main_str);   getchar();
	while (n--)  insert(&tree, re[n]);   // 建立字典树
	create_fail_point(tree);		// 建立失败指针
	find(tree, main_str);			// 进行匹配
	printf("\n");
	show(tree);					// 输出结果

	destory(&tree);
	return 0;
}
