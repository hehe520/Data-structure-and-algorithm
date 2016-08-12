
// 多路查找树 - B-tree
// 主要操作: 查找 search(),  插入 insert(),  删除 remove()

#include <stdio.h>
#include <stdlib.h>
#include <conio.h>

#define OK     1
#define ERROR  0
#define TRUE   1
#define FALSE  0
typedef int status;
typedef int keytype;
typedef char FILE_ADD;	// 磁盘文件地址类型

#define M 3		// 定义一棵 3 阶 B 树

// B 树储存结构
typedef struct btree
{
	int num;				// 兄弟数量
	btree *child[M+1];		// 孩子指针
	btree *parent;			// 双亲指针
	keytype key[M+1];		// 关键字
	FILE_ADD *addr[M+1];	// 磁盘上文件的地址
}btree, node;

typedef struct		// 搜索结果记录类型
{
	char tag;		// 成功标志
	btree *add;		// 结点地址
	int pos;		// 第 pos 号元素
}result;


// 查找一个元素 data
result search(btree *tree, keytype data)
{
	result tmp;
	int i;

	tmp.tag = FALSE;   tmp.add = NULL;
	while (tree && !tmp.tag)		// 还有树, 还没找到时
	{
		// 寻找 pos, 此部分要将磁盘上的文件读取到内存中
		// 也就是打开 tree->addr[] 记录的文件地址
		// 然后在内存中查找, 该算法没有体现, 直接查找关键字
		// 查找一个 pos, 使得 key[pos] <= data < key[pos+1]
		tmp.add = tree;    tmp.pos = -1;
		if (tree->num == 1)
		{
			if (data <= tree->key[0]) tmp.pos = 0;
			else tmp.pos = 1;
		}
		else 
		{
			if (data <= tree->key[0])	// 判断首
			{
				if (data == tree->key[0]) tmp.pos = 0;
				else tmp.pos = 0;
			}
			else if (data >= tree->key[tree->num - 1])	// 判断尾
			{
				if (data == tree->key[tree->num - 1]) tmp.pos = tree->num-1;
				else tmp.pos = tree->num;
			}
			else 
				for (i=0; i<tree->num; i++)		// 那么该元素肯定在中间
				{
					if (tree->key[i] <= data 
						&& data < tree->key[i + 1])
					{
						tmp.pos = i + 1;   break;
					}
				}
		}
		if (tmp.pos == -1) break;	// 没找到
		else 
		{
			if (tmp.pos < tree->num 
				&& tree->key[tmp.pos] == data) tmp.tag = TRUE;
			else 
			{	// 向子树走一步, 并且没找到
				tree = tree->child[tmp.pos];
				tmp.tag = FALSE;
			}
		}
	}
	return tmp;		// 查找失败返回双亲
}

// 在 t 结点上插入 data, 插入后关键字仍有序
// 从后面往前插, 类似插入排序的方法
void in(btree *t, keytype data)
{
	int i;
	for (i=t->num; i>0; i--)
	{
		if (data < t->key[i-1])	// 不断向前移动
		{
			t->key[i] = t->key[i-1];
			t->child[i+1] = t->child[i];
			t->child[i] = t->child[i-1];
			t->addr[i+1] = t->addr[i];
			t->addr[i] = t->addr[i-1];
		}
		else	// 插在 i 位置
		{
			t->key[i] = data;
			t->child[i+1] = NULL;
			break;
		}
	}
	if (i == 0)		// 插在第一位
	{
		t->key[0] = data;
		t->child[0] = NULL;
	}
	t->num++;
}

// 分裂 tree 结点, 其中 [mid+1..num-1] 分裂到 n 中
void split(btree *tree, btree *n, int mid)
{
	int i, j;

	for (j=0, i=mid+1; i<tree->num; i++, j++)
	{
		// 同时要移动孩子
		in(n, tree->key[i]);
		n->child[j] = tree->child[i];
		if (tree->child[i])
			tree->child[i]->parent = n;
	}
	n->child[j] = tree->child[i];
	if (tree->child[i])
		tree->child[i]->parent = n;
	tree->num = mid;			// 删除分裂出去的元素
}

btree *new_node()	// 建立并初始化一个结点
{
	btree *n;
	int i;

	n = (btree *)malloc(sizeof(btree));
	for (i=0; i<=M; i++)
	{
		n->addr[i] = NULL;
		n->child[i] = NULL;
		n->key[i] = NULL;
	}
	n->parent = NULL;
	n->num = 0;
	return n;
}

inline int ceil(int n)		// 求 n/2 的上限
{
	if (n % 2)
		return n / 2;
	else return n / 2 - 1;
}

// 向 B 树中插入一个不存在的结点
// 该算法已经不能用文字描述了, 详见<数据结构> 242 页
status insert(btree **tree, keytype data)
{
	result tmp;
	btree *n, *r;
	char flag = FALSE;		// 指示是否分裂完成
	int mid;
	
	tmp = search(*tree, data);	// 先找到这个结点
	if (!tmp.add)
	{
		*tree = new_node();		// 建立根结点
		in(*tree, data);   return OK;
	}
	else 
	{	
		if (tmp.tag != TRUE)	// 没找到该该结点
		{
			in(tmp.add, data);		// 找到合适的位置插入
			while (tmp.add && !flag)		// 没分裂完, 要继续执行
			{
				if (tmp.add->num > M-1)
				{
					// 如果超过 m-1 要分裂结点
					// 如果是分裂根结点, 要申请新空间
					if (tmp.add->parent)
					{
						// [0..mid-1], [mid], [mid+1..num-1]
						// 把 key[mid] 插入到双亲中
						mid = ceil(tmp.add->num);
						in(tmp.add->parent, tmp.add->key[mid]);
						
						// [mid+1..num-1] 分裂出去
						n = new_node();
						split(tmp.add, n, mid);		// 分裂
						
						if (tmp.add->parent->key[0] == tmp.add->key[mid])
						{
							// 插在了第一个位置, 重新连接指针
							tmp.add->parent->child[0] = tmp.add->parent->child[1];
							tmp.add->parent->child[1] = n;
						}
						else	// 插在了最后一位
						{
							tmp.add->parent->child[tmp.add->parent->num] = n;
						}
						n->parent = tmp.add->parent;
					}
					else	// 根结点分裂
					{
						r = new_node();		// 建立新根
						mid = ceil(tmp.add->num);
						in(r, tmp.add->key[mid]);
						n = new_node();		// 建立新兄弟结点
						
						split(tmp.add, n, mid);
						n->parent = tmp.add->parent = r;
						r->child[0] = tmp.add;		// 重接孩子
						r->child[1] = n;
						*tree = r;		// 重设新根
					}
					tmp.add = tmp.add->parent;	// 看看双亲要不要分裂
					tmp.tag = FALSE;
				}
				else flag = TRUE;
			} // end while
		}
		else return ERROR;
	}
	return OK;
}

void del_pos(btree *tree, int pos)	// 类似顺序表删除
{
	int i;

	for (i=pos; i<tree->num; i++)
	{
		tree->addr[i] = tree->addr[i+1];
		tree->addr[i+1] = NULL;
		tree->child[i] = tree->child[i+1];
		tree->child[i+1] = NULL;
		tree->key[i] = tree->key[i+1];
	}
	tree->child[i] = tree->child[i+1];
	tree->num--;
}

int find_pos(btree *f, btree *child)	// 寻找位序
{
	int i;
	for (i=0; i<=f->num; i++)
	{
		if (f->child[i]->key[0] == child->key[0])
			break;
	}
	return i;
}

void del_child(btree *cur)
{
	int i;

	for (i=0; i<cur->num; i++)
	{
		if (cur->child[i])
		{
			free(cur->child[i]);
			cur->child[i] = NULL;
		}
	}
}

void left(btree *cur, int pos)		// B 树左旋
{
	cur->child[pos]->key[0] = cur->key[pos];
	cur->key[pos] = cur->child[pos+1]->key[0];
	del_pos(cur->child[pos+1], 0);
}

void right(btree *cur, int pos)		// B 树右旋
{
	cur->child[pos]->key[0] = cur->key[pos-1];
	cur->key[pos-1] = cur->child[pos-1]->key[1];
	del_pos(cur->child[pos-1], 1);
}

// 在 cur 中释放 child[del], 再进行相应的合并
void combine_p(btree *cur, int del)
{
	int i;

	free(cur->child[del]);
	cur->child[del] = NULL;
	if (del == 0)		// 向右拿
	{
		in(cur->child[1], cur->key[0]);
		for (i=del; i<cur->num; i++)
		{
			cur->child[i] = cur->child[i+1];
			cur->key[i] = cur->key[i+1];
		}
	}
	else	// 向左拿
	{
		in(cur->child[del-1], cur->key[del-1]);
		if (del == 1) 
		{
			cur->key[0] = cur->key[1];
			cur->child[1] = cur->child[2];
		}
	}
	cur->num--;
}

// 合并双亲与孩子, 高度降一
void combine_child(btree *f, int del)
{
	if (del == 0) f->key[1] = f->child[1]->key[0];
	else
	{
		f->key[1] = f->key[0];
		f->key[0] = f->child[0]->key[0];
	}
	f->num = 2;
	del_child(f);
}

// 平衡修复情况1: 双亲一个, 叔叔一个
void fixup_1(btree *fcur, btree *u, btree *cur, int i)
{
	if (i == 0)
	{
		fcur->key[1] = u->key[0];
		fcur->child[1] = u->child[0];
		fcur->child[2] = u->child[1];
	}
	else 
	{
		fcur->child[2] = fcur->child[1];
		fcur->key[1] = fcur->key[0];
		fcur->key[0] = u->key[0];
		fcur->child[1] = u->child[1];
		fcur->child[0] = u->child[0];
	}
	fcur->num = 2;
	u->child[0]->parent = u->child[1]->parent = fcur;
	free(u);
}

// 平衡修复情况2: 双亲多个, 叔叔一个
void fixup_2(btree *fcur, btree *u, btree *cur, int i)
{
	if (i == 0)
	{
		in(u, fcur->key[0]);
		u->child[0] = fcur->child[0];
		fcur->child[0]->parent = u;
		del_pos(fcur, 0);
	}
	else if (i == 1)
	{
		in(u, fcur->key[0]);
		u->child[2] = fcur->child[1];
		fcur->child[1]->parent = u;
		fcur->key[0] = fcur->key[1];
		fcur->child[1] = fcur->child[2];
		fcur->child[2] = NULL;
		fcur->num--;
	}
	else
	{
		in(u, fcur->key[1]);
		u->child[2] = fcur->child[2];
		fcur->child[2]->parent = u;
		fcur->child[2] = NULL;
		fcur->num--;
	}
}

// 平衡修复情况3: 双亲一个, 叔叔多个
void fixup_3(btree *fcur, btree *u, btree *cur, int i)
{
	node *n = new_node();

	n->num = 1;
	n->parent = fcur;
	if (i == 0)
	{
		n->key[0] = fcur->key[0];
		fcur->key[0] = u->key[0];
		n->child[0] = fcur->child[0];
		n->child[1] = u->child[0];
		fcur->child[0] = u->child[0]->parent = n;
		u->child[0] = NULL;
		fcur->child[0] = n;
		del_pos(u, 0);
	}
	else 
	{
		n->key[0] = fcur->key[0];
		fcur->key[0] = u->key[1];
		n->child[0] = u->child[2];
		u->child[2]->parent = n;
		n->child[1] = fcur->child[1];
		fcur->child[1]->parent = n;
		u->child[2] = NULL;
		fcur->child[1] = n;
		n->parent = fcur;
		u->num--;
	}
}

// 平衡修复情况4: 双亲多个, 叔叔多个
void fixup_4(btree *fcur, btree *u, btree *cur, int i)
{
	node *n = new_node();

	n->num = 1;
	n->parent = fcur;
	if (i == 0)
	{
		n->key[0] = fcur->key[0];	// 调试通过
		fcur->key[0] = u->key[0];
		n->child[0] = fcur->child[0];
		n->child[1] = u->child[0];
		fcur->child[0]->parent = u->child[0]->parent = n;
		u->child[0] = NULL;
		fcur->child[0] = n;
		del_pos(u, 0);
	}
	else if (i == 1)
	{
		n->key[0] = fcur->key[0];	// 调试通过
		fcur->key[0] = u->key[1];
		n->child[0] = u->child[2];
		n->child[1] = fcur->child[1];
		u->child[2]->parent = fcur->child[1]->parent = n;
		u->child[2] = NULL;
		fcur->child[1] = n;
		u->num--;
	}
	else if (i == 2)
	{
		n->key[0] = fcur->key[1];	// 调试通过
		fcur->key[1] = u->key[1];
		n->child[0] = u->child[2];
		n->child[1] = fcur->child[2];
		u->child[2]->parent = fcur->child[2]->parent = n;
		u->child[2] = NULL;
		fcur->child[2] = n;
		u->num--;
	}
}

// 在 B 树中删除一个关键字
status remove(btree **tree, keytype data)
{
	result tmp;
	node *cur;		// 双亲
	node *fcur;		// 祖父
	node *u;		// 叔叔
	int i;
	char flag = 0;
	int min = ceil(M);	// 最小结点数
	int lower;
	int del, re;

	tmp = search(*tree, data);
	if (!tmp.tag) return ERROR;		// 没找到
	if (tmp.add->child[0] == NULL)	// 如果是叶子
	{
		if (tmp.add->num > min || !(tmp.add->parent))
		{
			del_pos(tmp.add, tmp.pos);
			if (tmp.add->num == 0)
			{
				free(*tree);
				*tree = NULL;	// 删完了
			}
		}
		else	// 看看左右兄弟有没有"多余"的结点
		{
			flag = 0;		// flag 标志有没有多余的结点
			cur = tmp.add->parent;
			i = find_pos(cur, tmp.add);
			switch (i)
			{
			case 0 :	// 向右拿
				if (cur->child[1]->num > min)
				{
					left(cur, 0);   flag = 1;
				}
				break;
			case 1 :	// 先左后右
				if (cur->child[0]->num > min)
				{
					right(cur, 1);   flag = 1;
				}
				else if (cur->child[2] 
					&& cur->child[2]->num > min)
				{
					left(cur, 1);   flag = 1;
				}
				break;
			case 2 :	// 向左拿
				if (cur->child[1]->num > min)
				{
					right(cur, 2);   flag = 1;
				}
				break;
			}
			if (!flag)		// 兄弟没有多余的结点
			{				// 双亲有多余的, 拿一个与自己合并
				if (cur->num > min) combine_p(cur, i);
				else 
				{
					combine_child(cur, i);
					lower = 1;			// 高度降一
					while (lower)		// 循环到根, 进行平衡
					{
						fcur = cur->parent;
						lower = 0;
						if (fcur)
						{
							i =  find_pos(fcur, cur);
							if (i == 0) u = fcur->child[1];
							else u = fcur->child[i-1];
							switch ((fcur->num==1) + (u->num==1) * 2)
							{
							case 0 :		// F 多个, U 多个
								fixup_4(fcur, u, cur, i);   break;
							case 1 :		// F 一个, U 多个
								fixup_3(fcur, u, cur, i);   break;
							case 2 :		// F 多个, U 一个
								fixup_2(fcur, u, cur, i);   break;
							case 3 :		// F 一个, U 一个
								fixup_1(fcur, u, cur, i);
								lower = 1;		// 高度降一 
								cur = fcur;		// 向上走一步, 继续平衡到根
								break;		
							}
						}
					} // end while
				}
			}
		}
	}
	else	// 不是叶子, 用后继替换法, 前驱替换也可
	{
		del = tmp.add->key[tmp.pos];
		cur = tmp.add->child[tmp.pos + 1];
		while (cur->child[0] != NULL) cur = cur->child[0];
		re = cur->key[0];
		remove(tree, cur->key[0]);
		// 此处 remove 会再执行一次搜索, 待优化
		tmp = search(*tree, del);
		tmp.add->key[tmp.pos] = re;
	}
	return OK;
}


void show(btree *tree)		// 按结点元素, 输出 B 树
{
	int i;
	if (tree)
	{
		printf("{");
		for (i=0; i<tree->num; i++)
		{
			printf("%d", tree->key[i]);
			if (i != tree->num - 1) printf(", ");
		}
		printf("}, ");
		if (tree->num == 1) printf("\t\t");
		else printf("\t");
		// 因为树形结构不便于在控制台下观看
		// 所以输出双亲, 看看整个 B 树结构是否正确
		if (tree->parent) 
		{
			printf("parent = {");
			for (i=0; i<tree->parent->num; i++)
			{
				printf("%d", tree->parent->key[i]);
				if (i != tree->parent->num - 1) printf(", ");
			}
			printf("}, ");
		}
		else printf("parent = NULL");
		printf("\n");

		for (i=0; i<=tree->num; i++)	// 递归查看孩子结点
			if (tree->child[i]) show(tree->child[i]);
			else break;
	}
}

char display(btree *tree)	// 显示模块
{
	char c;

	printf("先序遍历:\n");
	show(tree);   printf("\n\n按 c 退出, 按回车继续\n");
	getchar();
	c = getch();
	system("cls");
	return c;
}

int main(void)
{
	btree *tree = NULL;
	keytype data = 0;

	while (1)		// 插入测试
	{
		printf("请输入要查找的元素: ");
		scanf("%d", &data);
		if (insert(&tree, data))		// 插入
			printf("\n该元素不存在, 已插入!\n\n");
		else printf("\n找到!\n\n");
	
		if (display(tree) == 'c') break;
	}
	while (1)		// 删除测试
	{
		printf("请输入要删除的元素: ");
		scanf("%d", &data);
		if (!remove(&tree, data))		// 删除
			printf("\n该元素不存在, 删除失败!\n");
		else printf("\n已删除!\n\n");

		if (display(tree) == 'c') break;
	}
	return 0;
}
