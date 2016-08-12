typedef int status;

#define OK 1
#define ERROR 0

// ²Ù×÷·ûÕ»
typedef struct cnode 
{
	char data;
	cnode *next;
}cnode;

typedef struct opstack
{
	cnode *base;
	cnode *top;
	int lenth;
}opstack;

status opcreate(opstack *p);
status opdestory(opstack *p);
status oppush(opstack *p, char data);
status oppop(opstack *p, char *back);
char opgettop(opstack *p);


// Êı×ÖÕ»
typedef struct nnode 
{
	float data;
	nnode *next;
}nnode;

typedef struct numstack
{
	nnode *base;
	nnode *top;
	int lenth;
}numstack;

status numcreate(numstack *p);
status numdestory(numstack *p);
status numpush(numstack *p, float data);
status numpop(numstack *p, float *back);
float numgettop(numstack *p);