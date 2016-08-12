// 图的邻接表的储存及树的孩子兄弟链表

typedef char elemtype;
typedef char infotype;
typedef int status;
#define ERROR 0
#define OK 1
#define NUM 30		// 最大结点数
#define NO -1		// 没找到

// 四种图的类型
typedef enum {DG, UDG} gkind;

extern int used[NUM];

// ---------孩子兄弟链表----------
typedef struct tree
{
	elemtype data;
	tree *child;		// 首孩子
	tree *brother;		// 兄弟
}tree, cnode;


// 邻接表结点结构  |下标|next|
typedef struct vnode
{
	int adj;			// 可到的顶点
	vnode *nextarc;		// 下一条边
	infotype *info;		// 附加信息
}vnode;

// 邻接表头结构  |顶点|指针域|
typedef struct 
{
	elemtype data;		// 顶点名称
	vnode *first;		// 第一条边
}adjlist;

// 邻接表
typedef struct 
{
	adjlist vex[NUM];	// 头结构
	int vexnum;			// 顶点数
	int arcnum;			// 边数
	gkind kind;
}algraph;

// ------------------邻接表存储, 图的相关操作----------------------
status create(algraph *graph, int kind, elemtype *vexs, int n);
status destory(algraph *graph);			// 销毁图
void input(algraph *graph);				// 批量输入
int find_vex(algraph *graph, elemtype *data);		// 查找结点地址
status insert_arc(algraph *graph, elemtype *st, elemtype *end);
status insert_arc_s(algraph *graph, elemtype *st, elemtype *end);

// -----------------优先生成树的主要函数-------------------
status BFS(algraph *graph, tree **t);			// 广度生成树
status BFS_s(algraph *graph, int i, tree **t);
status DFS(algraph *graph, tree **t);			// 深度生成树
status DFS_s(algraph *graph, int i, tree **t);
status DFS2(algraph *graph, tree **t);			// 深度生成树, 非递归
status DFS2_s(algraph *graph, int i, tree **t);
status DFS_node(algraph *graph, char *back);	// 输出关节点到 back
char *DFS_node_s(algraph *graph, int pos, char *back);   // 找单个连通分支
