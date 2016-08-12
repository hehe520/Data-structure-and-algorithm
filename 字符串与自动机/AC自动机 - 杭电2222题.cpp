
#include <stdio.h>
#include <malloc.h>

#define NUM 26		// 26 个小写字母
#define MAXN 100000

typedef struct node		// 字典树结构
{
	int count;			// 字符串的出现的次数, 可重复
	node *fail;			// 失败指针
	node *next[NUM];	// 孩子指针
}node;

node *q[MAXN];
char str[1000010];		// 主串

// 申请一个新结点, 返回地址
node *new_node()
{
	int i;
	node *tmp = (node *)malloc(sizeof(node));

	tmp->count = 0;
	tmp->fail = NULL;
	for (i=0; i<NUM; i++)
		tmp->next[i] = NULL;
	return tmp;
}

// 将 str 字符串插入字典树中
void insert(node *tree, char *str)
{
	int i;
	int add;
	node *cur = tree;

	for (i=0; str[i]!='\0'; i++)
	{
		add = str[i] - 'a';
		if (cur->next[add] == NULL)
			cur->next[add] = new_node();
		cur = cur->next[add];
	}
	cur->count++;		// 该单词数增加 1
}

// 建立失败指针
void build(node *tree)
{
	int f = 0;		// 队头
	int r = 0;		// 队尾
	node *cur;
	node *ptr;
	int i;

	q[r] = tree;
	r = (r + 1) % MAXN;
	while (f != r)
	{
		cur = q[f];
		f = (f + 1) % MAXN;
		for (i=0; i<NUM; i++)		// 遍历子结点
		{
			if (cur->next[i])
			{
				if (cur == tree) cur->next[i]->fail = tree;
				else
				{
					// 遍历失败指针
					for (ptr=cur->fail; ptr!=NULL; ptr=ptr->fail)
					{
						// 如果有相同前驱结点
						if (ptr->next[i])
						{
							cur->next[i]->fail = ptr->next[i];
							break;
						}
					}
					// 没有找到前驱结点相同的串
					if (ptr == NULL)
						cur->next[i]->fail = tree;
				}
				q[r] = cur->next[i];
				r = (r + 1) % MAXN;
			}
		}
	}
}

// 查找树中有多少个子串出现了, 子串可以重复
int search(node *root, char *str)
{
	int i;
	int add;
	node *cur = root;
	node *ptr;
	int total = 0;

	for (i=0; add=str[i]-'a', str[i]!='\0'; i++)
	{
		// 去到失败指针指向的结点继续匹配
		while (cur != root)
		{
			if (cur->next[add]) break;
			cur = cur->fail;
		}
		ptr = cur = cur->next[add];
		if (cur == NULL) ptr = cur = root;
		while (ptr != NULL)
		{
			if (ptr->count > 0)
			{
				total += ptr->count;
				ptr->count = 0;
			}
			ptr = ptr->fail;
		}
	}
	return total;
}

void destory(node *tree)
{
	int i;

	for (i=0; i<NUM; i++)
		if (tree->next[i])
			destory(tree->next[i]);
	free(tree);
}

int main(void)
{
	int t, n, i;
	char buf[70];
	node *tree;

	scanf("%d", &t);
	while (t--)
	{
		tree = new_node();		// 根结点
		scanf("%d", &n);
		for (i=0; i<n; i++)		// 读入待查找子串
		{
			scanf("%s", buf);
			insert(tree, buf);
		}
		scanf("%s", str);		// 读入主串
		build(tree);			// 建立失败指针
		n = search(tree, str);	// 查找多少个子串出现了
		printf("%d\n", n);
		destory(tree);
	}
	return 0;
}