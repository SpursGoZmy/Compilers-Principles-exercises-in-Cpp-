#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include "symtab.h"

#define SIZE 211   //������ű�Ĵ�С��Ҳ����ģ����ʱ�ı������������鱾ѡ��һ������������ѡ��¼B��Tiny���ű�Ĵ�С��211�����α�397ҳ
#define SHIFT 4 //��������˻����ӣ��������2��4�η�=16���ο��α�229ҳ

static int hash(char * key)     //��ϣ���������������������Ӧ��hashֵ���ο��α�230ҳ
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
	int lineno;	// ��¼��Դ�ļ�����һ�������˱���
	struct LineListRec* next;
} * LineList;

typedef struct BucketListRec {	// BucketListRec  ��ϣ���е����ġ�Ͱ��,��ϣ��ʵ���Ͼ���һ���������
	struct BucketListRec* next;	// the next hash
	LineList*   lines;	// reference line  Ͱ��Ӧ��������ű�����lineno
	char * name;
	char *	scope;              //����������������Tiny��ͬ
	TokenType	type;
	int	memloc;		// memory location,�����ڹ�ϣ���е�λ��
	int	isArr;   //��ʶ�Ƿ�������
}* BucketList;

static BucketList hashTable[SIZE];  //��ϣ��


void  st_insert(char *name,char *scope, TokenType type,int lineno,int memloc, int isArr)   //�ڷ��ű������һ���µ�id
{
    int h=hash(name);
    BucketList l=hashTable[h];
    while((l!= NULL)&&((strcmp(name,l->name))!=0 || (strcmp(scope,l->scope))!= 0))   //�ڶ�Ӧ��Ͱ��������ұ���
        l=l->next;
    if(l==NULL)   //û���ڷ��ű����ҵ�
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

		l->next=hashTable[h];   //���µ�����뵽"Ͱ"�����������ͻ���ο��α�228ҳ
        hashTable[h]=l;
    }
    else    //�ڷ��ű����ҵ��˶�Ӧ�ı�����ֻ������һ�������õ��кš�
    {
        LineList t=l->lines;
        while(t->next!=NULL) t=t->next;
        t->next =(LineList) malloc(sizeof(struct LineListRec));
        t->next->lineno=lineno;
        t->next->next=NULL;
    }
}


TokenType search_type(char *name,char* scope)   //����id�����ͣ��������Լ��
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

int st_lookup(char* name,char* scope)   //�ڷ��ű�����ұ������ڴ�Ĵ洢λ�ã����ڴ�������
{
    int h=hash(name);
    BucketList l= hashTable[h];
    while((l!= NULL)&&((strcmp(name,l->name))!=0 || (strcmp(scope,l->scope))!= 0))   //�ڶ�Ӧ��Ͱ��������ұ���
        l=l->next;
    if(l==NULL)
        return -1;
    else
        return l->memloc;
}

int search_arr(char *name,char* scope)  //��ѯ�Ƿ�������
{
    int h=hash(name);
    BucketList l=hashTable[h];
    while((l!= NULL)&&((strcmp(name,l->name))!=0 || (strcmp(scope,l->scope))!= 0))   //�ڶ�Ӧ��Ͱ��������ұ���
        l=l->next;
    if(l==NULL)
        return false;
    else
        return true;
}

void printSymTab(FILE* listing)    //��ӡ���ű�
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


