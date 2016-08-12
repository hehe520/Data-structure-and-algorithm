typedef int elemtype;
typedef int status;

#define OK     1
#define ERROR  0
#define SIZE   50    // 原大小
#define INC    10    // 增量

typedef struct
{
	elemtype *elem;
	int length;
	int size;
}sqlist;


// main_inter.cpp 中
extern sqlist list;				// list 全局变量声明
void   display();				// 主界面
void   wait();					// 等待操作
int    getnum(char str[]);		// 获得用户输入的操作
void   back();					// 回主界面
void   exe_createlist();
void   exe_input();				// 依次输入数据
void   exe_destorylist();
void   exe_insert();
void   exe_update();
void   exe_remove();
void   exe_showlist();
void   exe_clearlist();
void   exe_delall();			// 删除全部元素
void   exe_getelem();
void   exe_listlen();


// list_inter.cpp 中
void   init(sqlist *p);				// 初始化表
status createlist(sqlist *p);		// 创建顺序表
status destorylist(sqlist *p);		// 销毁线性表
status insert(sqlist *p, int pos, elemtype *data);	// 插入数据
status update(sqlist *p, int pos, elemtype *data);	// 修改数据
status remove(sqlist *p, int pos);					// 删除数据
status showlist(sqlist *p);							// 显示表中所有元素
status clearlist(sqlist *p);						// 重置表中元素 写0
status getelem(sqlist *p, int pos, elemtype *back);	// 访问第N个元素
status listlen(sqlist *p, int *back);				// 获得表长

