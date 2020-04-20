#include "globals.h"
#include "util.h"
#include "scanner.h"

static int indentno=0;
#define INDENT indentno+=2
#define UNINDENT indentno-=2

void printToken(TokenType token,const char* tokenString)    //输出记号类型和串值
{
    switch(token)
    {
    case ELSE:
    case IF:
    case INT:
    case RETURN:
    case VOID:
    case WHILE:
        fprintf(listing,"reserved word:%s\n",tokenString);
        break;
    case PLUS:
        fprintf(listing,"+\n");
        break;
    case MINUS:
        fprintf(listing,"-\n");
        break;
    case TIMES:
        fprintf(listing,"*\n");
        break;
    case OVER:
        fprintf(listing,"/\n");
        break;
    case LT:
        fprintf(listing,"<\n");
        break;
    case LEQ:
        fprintf(listing,"<=\n");
        break;
    case GT:
        fprintf(listing,">\n");
        break;
    case GEQ:
        fprintf(listing,">=\n");
        break;
    case EQ:
        fprintf(listing,"==\n");
        break;
    case NEQ:
        fprintf(listing,"!=\n");
        break;
    case ASSIGN:
        fprintf(listing,"=\n");
        break;
    case SEMI:
        fprintf(listing,";\n");
        break;
    case COMMA:
        fprintf(listing,",\n");
        break;
    case LPAREN:
        fprintf(listing,"(\n");
        break;
    case RPAREN:
        fprintf(listing,")\n");
        break;
    case LMBRACKET:
        fprintf(listing,"[\n");
        break;
    case RMBRACKET:
        fprintf(listing,"]\n");
        break;
    case LBBRACKET:
        fprintf(listing,"{\n");
        break;
    case RBBRACKET:
        fprintf(listing,"}\n");
        break;
    case NUM:
        fprintf(listing,"NUM, val= %s\n",tokenString);
        break;
    case ID:
        fprintf(listing, "ID, name= %s\n",tokenString);
        break;
    case ERROR:
        fprintf(listing,"ERROR: %s\n",tokenString);
        break;
    case ENDFILE:
        fprintf(listing," EOF\n");
        break;
    default:
        fprintf(listing,"Unknown token: %d\n",token);

    }
}

char * copyString(char *s)
{
    int n;
    char* t;
    if(s==NULL)
        return NULL;
    n=strlen(s)+1;
    t=malloc(n);
    if(t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else
        strcpy(t,s);
    return t;
}

TreeNode* newNode(NodeKind kind)
{
    TreeNode* t=(TreeNode*)malloc(sizeof(TreeNode));
    int i;
    if(t==NULL)
        fprintf(listing,"Out of memory error at line %d\n",lineno);
    else
    {
        for(i=0;i<MAXCHILDREN;i++)
        {
            t->child[i]=NULL;
        }
        t->sibling=NULL;
        t->nodekind=kind;
        t->lineno=lineno;
        if(kind==OpK||kind==IntK||kind==IdK)
            t->type=Integer;
        if(kind==IdK)      //构建IdK结点时把标识符的名字存在属性里
            t->attr.name=copyString(tokenString);
        //if(kind==OpK)
          //  t->attr.op=;  //先将操作符存在name里
        if(kind==ConstK)
            t->attr.val=atoi(tokenString); //把字符串类型的整数转化为整数

    }
    return t;
}



static void printSpaces()
{
    int i;
    for(i=0;i<indentno;i++)
        fprintf(listing," ");
}

void printTree(TreeNode* tree)
{
    int i;
    INDENT;
    while(tree !=NULL)
    {
        printSpaces();
        switch(tree->nodekind)
        {
        case IdK:
            fprintf(listing,"Id:%s\n",tree->attr.name);
            break;
        case VoidK:
            fprintf(listing,"Void\n");
            break;
        case IntK:
            fprintf(listing,"Int\n");
            break;
        case ConstK:
            fprintf(listing,"const int=%d\n",tree->attr.val);
            break;
        case Var_DeclK:
            fprintf(listing,"Var_Decl\n");
            break;
        case Array_DeclK:
            fprintf(listing,"Array_Decl\n");
            break;
        case FunK:
            fprintf(listing,"Function\n");
            break;
        case ParamsK:
            fprintf(listing,"Params\n");
            break;
        case Int_ParamK:
            fprintf(listing,"Int_Param\n");
            break;
        case Array_ParamK:
            fprintf(listing,"Array_Param\n");
        case CompK:
            fprintf(listing,"Compound Stmt\n");
            break;
        case Selection_StmtK:
            fprintf(listing,"If Stmt\n");
            break;
        case Iteration_StmtK:
            fprintf(listing,"While Stmt\n");
            break;
        case Return_StmtK:
            fprintf(listing,"Return Stmt\n");
            break;
        case AssignK:
            //fprintf(listing,"Assign to %s\n",tree->attr.name);
            if(tree->child[0]->nodekind==IdK)
            {
                fprintf(listing,"Assign Stmt，Assign to Id %s\n",tree->child[0]->attr.name);
            }
            else if(tree->child[0]->nodekind==Array_ElemK)
            {
                if((tree->child[0]->child[0]->nodekind==IdK)&&(tree->child[0]->child[1]->nodekind==ConstK))
                {
                    fprintf(listing,"Assign Stmt，Assign to Array Element %s[%d]\n",tree->child[0]->child[0]->attr.name,tree->child[0]->child[1]->attr.val);
                }

            }
            else
            {
                fprintf(listing,"Assign Stmt\n");
            }
            break;
        case CallK:
            fprintf(listing,"Call Stmt\n");
            break;
        case OpK:
            fprintf(listing,"Op :");
            printToken(tree->attr.op,"\0");
            break;
        case Array_ElemK:
            fprintf(listing,"Array Element\n");
            break;
        case ArgsK:
            fprintf(listing,"ArgsK\n");
            break;
        default:
            fprintf(listing,"UnknownK: Unknown kind\n");
            break;
        }
        for(i=0;i<MAXCHILDREN;i++)
        {
            printTree(tree->child[i]);
        }
        tree=tree->sibling;
    }
    UNINDENT;
}
