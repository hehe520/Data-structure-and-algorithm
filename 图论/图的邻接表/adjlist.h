/* -----------------------------
// 图的邻接表的储存及相关操作
//
//
// 邻接表如图: 
// [0][A] -> [1] -> [3] -> [4]
// [1][B] -> [0]
// [2][C]
// [3][D] -> [2]
// [4][E] => [0] -> [1]
------------------------------ */

typedef char elemtype;
typedef char infotype;
typedef int status;
#define ERROR 0
#define OK 1
#define NUM 30		// 最大结点数
#define NO -1		// 没找到

// 四种图的类型
typedef enum {DG, UDG, DN, UDN} gkind;

extern char used[NUM];


// 邻接表结点结构  |下标|next|
typedef struct vnode
{
	int wight;			// 权值
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
void show(algraph *graph);				// 输出邻接表
int find_vex(algraph *graph, elemtype *data);		// 查找结点地址
char *getname(algraph *graph, int addr);			// 查找结点名称
status insert_arc(algraph *graph, elemtype *st, elemtype *end, int wight);
status insert_arc_s(algraph *graph, elemtype *st, elemtype *end, int wight);
status remove_arc(algraph *graph, elemtype *st, elemtype *end);
status remove_arc_s(algraph *graph, elemtype *st, elemtype *end);
status insert_vex(algraph *graph, elemtype *name);
status remove_vex(algraph *graph, elemtype *name);
int first_vex(algraph *graph, elemtype *name);
int next_vex(algraph *graph, elemtype *name, elemtype *pos);

status BFS(algraph *graph, void (*visit)(algraph *graph, int n));	// 广度优先搜索
status BFS_s(algraph *graph, int i, 
			 void (*visit)(algraph *graph, int n));			// 广搜, 打通单个连通分支
status DFS(algraph *graph, void (*visit)(algraph *graph, int n));	// 深度优先搜搜
status DFS_s(algraph *graph, int i, 
			 void (*visit)(algraph *graph, int n));			// 深搜, 打通单个连通分支
status DFS2(algraph *graph, void (*visit)(algraph *graph, int n));	// 深搜, 非递归
status DFS2_s(algraph *graph, int f,						// 非递归打通每个连通分支
			void (*visit)(algraph *graph, int n));			