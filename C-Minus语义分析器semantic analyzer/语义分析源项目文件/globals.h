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
    //������
    ELSE,IF,INT,RETURN,VOID,WHILE,
    //ר�÷���
    PLUS,MINUS,TIMES,OVER,LT,LEQ,GT,GEQ,EQ,NEQ,ASSIGN,SEMI,COMMA,LPAREN,RPAREN,
    LMBRACKET,RMBRACKET,LBBRACKET,RBBRACKET,
    //һ�����ֺͱ�ʶ��
    NUM,ID

}TokenType;
extern FILE* source;
extern FILE* listing;
extern FILE* code;

extern int lineno;   //������к�
extern int EchoSource;
extern int TraceScan;
extern int TraceParse;
typedef enum
{     //��20���﷨���������
    IntK,IdK,VoidK,ConstK,Var_DeclK,Array_DeclK,FunK,ParamsK,Int_ParamK,Array_ParamK,CompK,
    Selection_StmtK,Iteration_StmtK,Return_StmtK,AssignK,OpK,Array_ElemK,
    CallK,ArgsK,UnkonwnK
}NodeKind;

typedef enum
{
    Void,Integer
}ExpType;

#define MAXCHILDREN 4   //�﷨����������4�����ӣ����﷨�����Ϊfun-declaration(FunK)ʱ����Ҫ4������

typedef struct treeNode
{
    struct treeNode* child[MAXCHILDREN];
    struct treeNode* sibling;
    int lineno;   //�к�
    NodeKind nodekind;   //�������
    union
    {
        TokenType op;  //�洢Op�﷨������Op����
        int val;       //�洢ConstK������ֵ��С
        char* name;    //�洢Id����Id����
    }attr;
    char *scope;                    //����������
    ExpType type;
}TreeNode;



typedef struct scope_list
{
    char *scope_names[50];
    int current_num;
}All_scopes;

extern All_scopes* Scope_List;

#endif // GLOBALS_H_INCLUDED
