#include "globals.h"
#include "symtab.h"
#include "parse.h"
#include "analyze.h"

static int location=0;    //变量的存储位置
//用来对语法树进行遍历的函数，这是为了强调树的遍历技术，通过设置函数指针来调用前序遍历或后序遍历时的处理函数。
//比如将preProc设为一个空函数可以对语法树进行后序遍历，参考课本259页，但是构造符号表时没有用上
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


static void nullProc(TreeNode * t)  //配合traverse使用的空函数
{
    if(t==NULL)
        return;
    else
        return;
}



 void insertNode(TreeNode * t)   //根据语法树的节点不同进行不同的操作
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

static void traverse_for_build_symtab(TreeNode * t)   //用于符号表构造的语法树遍历
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
void buildSymtab(TreeNode* t)   //给主函数提供的接口
{
    traverse_for_build_symtab(t);
}


