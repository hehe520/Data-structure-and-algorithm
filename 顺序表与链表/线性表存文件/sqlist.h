typedef int status;
typedef int elemtype;	// 采用整型做试验

#define OK    1
#define ERROR 0
#define SIZE  10	// 初始大小
#define INC   10	// 增量

// 线性表存储结构
typedef struct 
{
	elemtype *base;	// 指向一块自己的地方
	int lenth;		// 当前使用量
	int size;		// 总容量
}sqlist;

// 单链表结点结构
typedef struct node
{
	elemtype data;
	node *next;
}node;

// 单链表头结构
typedef struct head
{
	node *first;
	node *end;
	int lenth;
}head;

// --------main_inter主要函数--------
void display(void);		// 显示界面
void wait(void);		// 等待操作
void exe_writefile();	// 顺序表写文件
void exe_writefile2();	// 链表写文件
void exe_readfile();	// 从文件读取顺序表
void exe_readfile2();	// 从文件读取链表
void back();			// 回主界面
status existfile(const char *path);	// 检查文件存在

// --------list_inter主要函数--------
status createlist(sqlist *p, int size);
status destorylist(sqlist *p);
status insertlist(sqlist *p, int pos, elemtype *data);
status showlist(sqlist *p);

// 顺序表文件读写函数
status writefile(sqlist *p, const char *path);
status readfile(sqlist *p, const char *path);

// --------node_inter主要函数--------
status createnode(head *p);
status destorynode(head *p);
status shownode(head *p);
status append(head *p, elemtype *data);

// 链表文件读写函数
status writefile2(head *p, const char *path);
status readfile2(head *p, const char *path);
