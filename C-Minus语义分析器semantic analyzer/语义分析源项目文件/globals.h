#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAXRESERVED 6
#define Max_scope_num 200
#define true 1
#define false 0
typedef enum
{
    //book-keeping tokens
    ENDFILE,ERROR,
    //保留字
    ELSE,IF,INT,RETURN,VOID,WHILE,
    //专用符号
    PLUS,MINUS,TIMES,OVER,LT,LEQ,GT,GEQ,EQ,NEQ,ASSIGN,SEMI,COMMA,LPAREN,RPAREN,
    LMBRACKET,RMBRACKET,LBBRACKET,RBBRACKET,
    //一般数字和标识符
    NUM,ID

}TokenType;
extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno;   //代码的行号
extern int EchoSource;
extern int TraceScan;
extern int TraceParse;
typedef enum
{     //共20种语法树结点类型
    IntK,IdK,VoidK,ConstK,Var_DeclK,Array_DeclK,FunK,ParamsK,Int_ParamK,Array_ParamK,CompK,
    Selection_StmtK,Iteration_StmtK,Return_StmtK,AssignK,OpK,Array_ElemK,
    CallK,ArgsK,UnkonwnK
}NodeKind;

typedef enum
{
    Void,Integer
}ExpType;

#define MAXCHILDREN 4   //语法树结点最多有4个孩子，当语法树结点为fun-declaration(FunK)时才需要4个孩子

typedef struct treeNode
{
    struct treeNode* child[MAXCHILDREN];
    struct treeNode* sibling;
    int lineno;   //行号
    NodeKind nodekind;   //结点类型
    union
    {
        TokenType op;  //存储Op语法树结点的Op类型
        int val;       //存储ConstK结点的数值大小
        char* name;    //存储Id结点的Id名字
    }attr;
    char *scope;                    //作用域属性
    ExpType type;
}TreeNode;



typedef struct scope_list
{
    char *scope_names[50];
    int current_num;
}All_scopes;

extern All_scopes* Scope_List;

#endif // GLOBALS_H_INCLUDED
