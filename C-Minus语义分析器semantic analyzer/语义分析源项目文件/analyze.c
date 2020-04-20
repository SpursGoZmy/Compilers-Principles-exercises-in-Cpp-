#include "globals.h"
#include "symtab.h"
#include "parse.h"
#include "analyze.h"

static int location=0;    //�����Ĵ洢λ��
//�������﷨�����б����ĺ���������Ϊ��ǿ�����ı���������ͨ�����ú���ָ��������ǰ�������������ʱ�Ĵ�������
//���罫preProc��Ϊһ���պ������Զ��﷨�����к���������ο��α�259ҳ�����ǹ�����ű�ʱû������
static void traverse(TreeNode *t,void (*preProc) (TreeNode * ),void (* postProc) (TreeNode *))
{
    if (t!=NULL)
    {
        preProc(t);
        for (int i = 0; i < MAXCHILDREN; ++i)
        {
            traverse( t->child[i], preProc, postProc);
        }
        postProc(t);
        traverse( t->sibling, preProc, postProc );
    }
}


static void nullProc(TreeNode * t)  //���traverseʹ�õĿպ���
{
    if(t==NULL)
        return;
    else
        return;
}



 void insertNode(TreeNode * t)   //�����﷨���Ľڵ㲻ͬ���в�ͬ�Ĳ���
{

    if (t==NULL) return;
    switch(t->nodekind)
    {
    case Int_ParamK:
        if(t->child[1]!=NULL)
            st_insert(t->child[1]->attr.name,t->scope,INT,t->child[1]->lineno,location++,false);
        break;
    case Array_ParamK:
        if(t->child[1]!=NULL)
            st_insert(t->child[1]->attr.name,t->scope,INT,t->child[1]->lineno,location++,true);
        break;
    case Array_DeclK:
        if(t->child[1]!=NULL)
            st_insert(t->child[1]->attr.name,t->scope,INT,t->child[1]->lineno,location++,true);
        break;
    case Var_DeclK:
        if(t->child[1]!=NULL)
            st_insert(t->child[1]->attr.name,t->scope,INT,t->child[1]->lineno,location++,false);
        break;
    case IdK:
        {
        int num;

        num=Scope_List->current_num;
        for(int i=num-1;i>=0;i--)
        {
            int memloc=st_lookup(t->attr.name,Scope_List->scope_names[i]);
            if(memloc!=-1)
            {
                st_insert(t->attr.name,Scope_List->scope_names[i],INT,t->lineno,memloc,false);
                break;
            }
        }
        break;
        }
    default:
        break;
    }
}

static void traverse_for_build_symtab(TreeNode * t)   //���ڷ��ű�����﷨������
{
    if(t==NULL) return;

    switch(t->nodekind)
    {
    case Int_ParamK:
    case Array_ParamK:
    case Var_DeclK:
    case Array_DeclK:
        insertNode(t);
        traverse_for_build_symtab(t->sibling);
        break;
    default:
        insertNode(t);
        for(int i=0;i<MAXCHILDREN;i++)
        {
            traverse_for_build_symtab(t->child[i]);
        }
        traverse_for_build_symtab(t->sibling);
        break;
    }
}
void buildSymtab(TreeNode* t)   //���������ṩ�Ľӿ�
{
    traverse_for_build_symtab(t);
}


