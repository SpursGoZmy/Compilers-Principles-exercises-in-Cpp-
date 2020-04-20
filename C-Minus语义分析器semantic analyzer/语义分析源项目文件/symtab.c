#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "symtab.h"

#define SIZE 211   //定义符号表的大小，也就是模运算时的被除数，根据书本选择一个素数，这里选择附录B中Tiny符号表的大小：211，见课本397页
#define SHIFT 4 //用来计算乘积因子，这里就是2的4次方=16，参考课本229页

static int hash(char * key)     //哈希函数，用来计算变量名对应的hash值，参考课本230页
{
    int temp=0;
    int i=0;
    while(key[i]!='\0')
    {
        temp=((temp<< SHIFT)+key[i])% SIZE;
        ++i;
    }
    return temp;
}

typedef struct LineListRec {	// LineListRec
	int lineno;	// 记录在源文件的哪一行引用了变量
	struct LineListRec* next;
} * LineList;

typedef struct BucketListRec {	// BucketListRec  哈希表中单个的“桶”,哈希表实际上就是一个入口数组
	struct BucketListRec* next;	// the next hash
	LineList*   lines;	// reference line  桶对应的链表，存放变量的lineno
	char * name;
	char *	scope;              //变量的作用域，这点和Tiny不同
	TokenType	type;
	int	memloc;		// memory location,变量在哈希表中的位置
	int	isArr;   //标识是否是数组
}* BucketList;

static BucketList hashTable[SIZE];  //哈希表


void  st_insert(char *name,char *scope, TokenType type,int lineno,int memloc, int isArr)   //在符号表里插入一个新的id
{
    int h=hash(name);
    BucketList l=hashTable[h];
    while((l!= NULL)&&((strcmp(name,l->name))!=0 || (strcmp(scope,l->scope))!= 0))   //在对应的桶链表里查找变量
        l=l->next;
    if(l==NULL)   //没有在符号表里找到
    {
        l=(BucketList)malloc(sizeof(struct BucketListRec));
        l->name=name;
		l->scope = scope;
		l->type = type;
		l->memloc = memloc;
		l->isArr = isArr;
		l->lines=(LineList) malloc(sizeof(struct LineListRec));
		LineList p=l->lines;
		p->lineno=lineno;
		p->next=NULL;
//		l->lines->lineno=lineno;
//		l->lines->next=NULL;

		l->next=hashTable[h];   //把新的项插入到"桶"表中来解决冲突，参考课本228页
        hashTable[h]=l;
    }
    else    //在符号表里找到了对应的变量，只需增添一个被引用的行号。
    {
        LineList t=l->lines;
        while(t->next!=NULL) t=t->next;
        t->next =(LineList) malloc(sizeof(struct LineListRec));
        t->next->lineno=lineno;
        t->next->next=NULL;
    }
}


TokenType search_type(char *name,char* scope)   //查找id的类型，用于属性检查
{
    int h=hash(name);
    BucketList l=hashTable[h];
    while((l!= NULL)&&((strcmp(name,l->name))!=0 || (strcmp(scope,l->scope))!= 0))
        l=l->next;
    if(l==NULL)
        return ERROR;
    else
        return l->type;
}

int st_lookup(char* name,char* scope)   //在符号表里查找变量在内存的存储位置，用于代码生成
{
    int h=hash(name);
    BucketList l= hashTable[h];
    while((l!= NULL)&&((strcmp(name,l->name))!=0 || (strcmp(scope,l->scope))!= 0))   //在对应的桶链表里查找变量
        l=l->next;
    if(l==NULL)
        return -1;
    else
        return l->memloc;
}

int search_arr(char *name,char* scope)  //查询是否是数组
{
    int h=hash(name);
    BucketList l=hashTable[h];
    while((l!= NULL)&&((strcmp(name,l->name))!=0 || (strcmp(scope,l->scope))!= 0))   //在对应的桶链表里查找变量
        l=l->next;
    if(l==NULL)
        return false;
    else
        return true;
}

void printSymTab(FILE* listing)    //打印符号表
{
    fprintf(listing,"---------------------------------------------------------------------------------------\n");
    fprintf(listing," symbol table of program\n");
    fprintf(listing,"Variable Name  Scope             Type    Location    Line Numbers\n");
    fprintf(listing,"------------   ------           ------    ------     -----------    \n");
    for(int i=0;i< SIZE;i++)
    {
        if(hashTable[i]!=NULL)
        {
            BucketList l=hashTable[i];
            while(l!=NULL)
            {
                LineList t=l->lines;
                fprintf(listing,"%-14s  ",l->name);
                fprintf(listing,"%-7s  ",l->scope);
                switch (l->type)
                {
						case INT:
							fprintf(listing, "%12s", "int");
							break;
						case VOID:
							fprintf(listing, "%12s", "void");
							break;
						default:
							fprintf(listing,"%12s","unknown type");
							break;
                }
                fprintf(listing, "%8d\t", l->memloc);
				while(t!=NULL)
                {
                    fprintf(listing,"%4d",t->lineno);
                    t=t->next;
                }
                fprintf(listing,"\n");
                l=l->next;
            }
        }
    }
}


