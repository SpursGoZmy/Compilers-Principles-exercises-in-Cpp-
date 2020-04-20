#include "globals.h"
#include "scanner.h"
#include "util.h"
#include "parse.h"

static TokenType token;

static TreeNode* declaration_list();   //声明列表
static TreeNode* declaration();      //声明：分为变量声明和函数声明
static TreeNode* params();       //函数的参数列表
static TreeNode* param();       //每一个参数
static TreeNode* compound_stmt();    //函数的复合语句
static TreeNode* local_declaration();    //函数内部的局部变量声明
static TreeNode* statement_list();       //语句列表
static TreeNode* statement();           //单独的语句
static TreeNode* expression_stmt();        //表达式语句
static TreeNode* selection_stmt();        //if语句
static TreeNode* iteration_stmt();        //while语句
static TreeNode* return_stmt();       //返回语句
static TreeNode* expression();        //表达式，可以是赋值语句或简单表达式
static TreeNode* simple_expression(TreeNode* k);        //简单表达式
static TreeNode* var();                          //变量
static TreeNode* additive_expression(TreeNode* k);    //可加表达式
static TreeNode* term(TreeNode* k);            //乘积
static TreeNode* factor(TreeNode* k);         //因子
static TreeNode* call(TreeNode* k);           //函数调用
static TreeNode* args();         //实参列表


static void syntaxError(char* message)
{
    fprintf(listing,"\n>>> ");
    fprintf(listing,"Syntax error at line %d: %s",lineno,message);
    //Error=true;

}
static void match(TokenType expected)
{
    if(token==expected) token=getToken();
    else
    {
        syntaxError("unexpected token ->");
        printToken(token,tokenString);
        fprintf(listing,"    ");
    }
}

TreeNode* declaration_list()
{
    TreeNode* t=declaration();
    TreeNode* p=t;
    while(token!=INT&&token!=VOID&&token!=ENDFILE)      //如果一开始语法分析出现错误，就跳过错误部分直到是int或void
    {
        syntaxError(" ");
        token=getToken();
        if(token==ENDFILE)
            {
                fprintf(listing,"end of File\n");
                break;
            }
    }
    while(token==INT||token==VOID)     //遇到int或void，可以正常开始语法分析，因为声明总是以int或void开始
    {
        TreeNode* q=declaration();
        if(q!=NULL)
        {
            if(t==NULL)     //如果之前第一次语法分析出错，那么就把新找到的declaration结点作为第一个结点
            {
                t=p=q;
            }
            else
            {
                p->sibling=q;  //正常工作，把新找到的declaration结点作为上一个结点的兄弟
                p=q;
            }
        }
    }
    match(ENDFILE);
    return t;
}

TreeNode* declaration()
{
    TreeNode* t=NULL;    //t为最后要返回的子树根节点
    TreeNode* p=NULL;
    TreeNode* q=NULL;
    TreeNode* s=NULL;

    if(token==INT)    //先根据第一个token是int还是void构造一个新结点
    {
        p=newNode(IntK);
        match(INT);
    }
    else if(token==VOID)
    {
        p=newNode(VoidK);
        match(VOID);
    }
    else
    {
        syntaxError("declaration中类型匹配错误");
    }

    if((p!=NULL)&&(token==ID))  //前面没有出错，而且紧跟着第二个token为ID
    {
        q=newNode(IdK);
        //q->attr.name=tokenString;
        match(ID);

        if(token==LPAREN)    //下一个token为 ( ,说明该声明为函数声明
        {
            t=newNode(FunK);
            t->child[0]=p;
            t->child[1]=q;
            match(LPAREN);
            t->child[2]=params();
            match(RPAREN);
            t->child[3]=compound_stmt();
        }
        else if(token==LMBRACKET)     //下一个token为 [ ,说明该声明为数组声明
        {
            t=newNode(Var_DeclK);
            TreeNode* m=newNode(Array_DeclK);
            match(LMBRACKET);
            match(NUM);
            s=newNode(ConstK);
            m->child[0]=q;   //数组结点的第一个孩子为数组名Id
            m->child[1]=s;    //数组结点的第二个孩子为数组大小Num
            t->child[0]=p;        //变量声明结点的第一个孩子为变量类型p
            t->child[1]=m;        //变量声明结点的第二个孩子为数组结点m
            match(RMBRACKET);
            match(SEMI);
        }
        else if(token==SEMI)      //下一个token为 ；,说明为普通变量声明
        {
            t=newNode(Var_DeclK);
            t->child[0]=p;     //第一个孩子为变量类型,应该为int
            t->child[1]=q;     //第二个孩子为变量名字，应该为Id
            match(SEMI);
        }
        else
        {
            syntaxError("类型匹配错误，位于declaration中，ID后面跟随错误token");
        }
    }
    else
    {
        syntaxError("类型匹配错误，位于declaration中，int或void后面没有跟随Id");
    }
    return t;
}

TreeNode* params()
{
    TreeNode* t=newNode(ParamsK);
    TreeNode* p=NULL;
    TreeNode* q=NULL;
    //TreeNode* s=NULL;

    if(token==VOID)     //函数参数为void
    {
        p=newNode(VoidK);
        match(VOID);
        if(token!=RPAREN)
            {
                syntaxError("函数参数VOID后面没有跟右括号");
            }
        if(t!=NULL)
            t->child[0]=p;
    }
    else if(token==INT)   //函数参数为一个个参数param()组成
    {
        p=param();
        t->child[0]=p;
        while(token==COMMA)
        {

            match(COMMA);
            q=param();
            if(q==NULL)
                printf("nullllll\n");
            if(p!=NULL)
            {
                p->sibling=q;
            }
            p=q;
            if(token==RPAREN)
                break;
        }

    }
    else
    {
        syntaxError("函数参数类型错误\n");
    }
    return t;
}

TreeNode* param()       //构建单个Int参数或者数组参数的结点
{
    TreeNode* t=NULL;
    TreeNode* p=NULL;
    TreeNode* q=NULL;
    match(INT);
    p=newNode(IntK);
    q=newNode(IdK);
    match(ID);
    if(token==LMBRACKET)   //说明为数组参数，如 int a[]
    {
        match(LMBRACKET);
        match(RMBRACKET);
        t=newNode(Array_ParamK);
        t->child[0]=p;
        t->child[1]=q;
    }
    else if(token==COMMA)   //说明为普通Int参数
    {
        t=newNode(Int_ParamK);
        t->child[0]=p;
        t->child[1]=q;
    }
    else if(token==RPAREN)
    {
        t=newNode(Int_ParamK);
        t->child[0]=p;
        t->child[1]=q;
    }
    else
    {
        syntaxError("参数分隔格式错误");
    }
    return t;
}

TreeNode* compound_stmt()
{
    TreeNode* t=newNode(CompK);
    match(LBBRACKET);
    t->child[0]=local_declaration();
    t->child[1]=statement_list();
    match(RBBRACKET);
    return t;
}

TreeNode* local_declaration()
{
    TreeNode* t=NULL;
    TreeNode* q=NULL;
    TreeNode* p=NULL;
    TreeNode* q1=NULL;
    TreeNode* q2=NULL;
    TreeNode* q3=NULL;

    if(token==INT)   //要注意类型声明只能是int ，例如int a或int a[]
        {
            q1=newNode(IntK);
            match(INT);
        }
    if(token==ID)
        {
            q2=newNode(IdK);
            match(ID);
        }
    if(token==LMBRACKET)   //数组变量
    {
        match(LMBRACKET);
        q3=newNode(ConstK);  //数组长度
        match(NUM);
        match(RMBRACKET);
        p=newNode(Array_DeclK);
        p->child[0]=q1;
        p->child[1]=q2;
        p->child[2]=q3;
        t=p;
        match(SEMI);

    }
    else if(token==SEMI)   //普通整型变量
    {
        match(SEMI);
        p=newNode(Var_DeclK);
        p->child[0]=q1;
        p->child[1]=q2;
        t=p;
    }
    else
    {
        syntaxError("第一个局部变量声明出现问题\n");
    }

    while(token==INT)
    {
        if(token==INT)   //要注意类型声明只能是int ，例如int a或int a[]
        {
            q1=newNode(IntK);
            match(INT);
        }
        if(token==ID)
        {
            q2=newNode(IdK);
            match(ID);
        }
        if(token==LMBRACKET)   //数组变量
        {
            match(LMBRACKET);
            q3=newNode(ConstK);  //数组长度
            match(NUM);
            match(RMBRACKET);
            q=newNode(Array_DeclK);
            q->child[0]=q1;
            q->child[1]=q2;
            q->child[2]=q3;
            if(p!=NULL)
                p->sibling=q;         //连接兄弟结点
            p=q;
            match(SEMI);

        }
        else if(token==SEMI)   //普通整型变量
        {
            match(SEMI);
            q=newNode(Var_DeclK);
            q->child[0]=q1;
            q->child[1]=q2;
            if(p!=NULL)
                p->sibling=q;
            p=q;

        }
        else
        {
            syntaxError("局部变量声明出现问题\n");
        }

    }
    return t;
}

TreeNode* statement_list()
{
    TreeNode* t=statement();
    TreeNode* p=t;
    //根据所有可能的statement的开头来判断是否是statement
    while(token==IF||token==LBBRACKET||token==ID||token==WHILE||token==RETURN||token==SEMI||token==LPAREN||token==NUM)
    {
        TreeNode* q=statement();
        if(q!=NULL)
        {
            if(t==NULL)
            {
                t=p=q;
            }
            else
            {
                p->sibling=q;
                p=q;
            }
        }
    }
    return t;
}

TreeNode* statement()     //进一步判断statement的类型
{
    TreeNode* t=NULL;
    switch(token)
    {
    case IF:
        t=selection_stmt();
        break;
    case WHILE:
        t=iteration_stmt();
        break;
    case RETURN:
        t=return_stmt();
        break;
    case LBBRACKET:
        t=compound_stmt();
        break;
    case ID:
    case SEMI:
    case LPAREN:
    case NUM:
        t=expression_stmt();
        break;
    default:
        syntaxError("statement识别错误");
        token=getToken();
        break;
    }
    return t;
}

TreeNode* selection_stmt()
{
    TreeNode* t=newNode(Selection_StmtK);
    match(IF);
    match(LPAREN);
    if(t!=NULL)
    {
        t->child[0]=expression();
    }
    match(RPAREN);
    t->child[1]=statement();
    if(token==ELSE)
    {
        match(ELSE);
        if(t!=NULL)
        {
            t->child[2]=statement();
        }
    }
    return t;
}

TreeNode* expression_stmt()
{
    TreeNode* t=NULL;
    if(token==SEMI)
    {
        match(SEMI);
        return t;
    }
    else
    {
        t=expression();
        match(SEMI);

    }
    return t;
}

TreeNode* iteration_stmt()
{
    TreeNode* t=newNode(Iteration_StmtK);
    match(WHILE);
    match(LPAREN);
    if(t!=NULL)
    {
        t->child[0]=expression();
    }
    match(RPAREN);
    if(t!=NULL)
    {
        t->child[1]=statement();
    }
    return t;
}

TreeNode* return_stmt()
{
    TreeNode* t=newNode(Return_StmtK);
    match(RETURN);
    if(token==SEMI)
    {
        match(SEMI);
        return t;
    }
    else
    {
        if(t!=NULL)
        {
            t->child[0]=expression();
        }
    }
    match(SEMI);
    return t;
}

TreeNode* expression()
{
    TreeNode* t=var();
    if(t==NULL)   //不以var()开头，说明只能为simple_expression
    {
        t=simple_expression(t);
    }
    else     //以var开头，可能是赋值语句，也可能是simple_expression的var或call
    {
        TreeNode* p=NULL;
        if(token==ASSIGN)   //赋值语句
        {
            p=newNode(AssignK);
            match(ASSIGN);
            p->child[0]=t;
            p->child[1]=expression();
            return p;
        }
        else   //说明是simple_expression中的var或call
        {
            t=simple_expression(t);
        }
    }
    return t;
}

TreeNode* simple_expression(TreeNode* k)
{
    TreeNode* t=additive_expression(k);
    k=NULL;  //
    //通过下一个token是否为relop来判断simple_expreession的类型
    if(token==EQ||token==GT||token==GEQ||token==LT||token==LEQ||token==NEQ)
    {
        TreeNode* q=newNode(OpK);
        if(q!=NULL)
        {
            q->attr.op=token;
            q->child[0]=t;
            t=q;
            match(token);
            t->child[1]=additive_expression(k);
            return t;
        }

    }
    return t;
}

TreeNode* additive_expression(TreeNode* k)
{
    TreeNode* t=term(k);
    k=NULL;
    while(token==PLUS||token==MINUS)
    {
        TreeNode* q=newNode(OpK);
        q->attr.op=token;
        q->child[0]=t;
        match(token);
        q->child[1]=term(k);
        t=q;
    }
    return t;
}

TreeNode* term(TreeNode* k)
{
    TreeNode* t=factor(k);
    k=NULL;
    while(token==TIMES||token==OVER)
    {
        TreeNode* q=newNode(OpK);
        q->attr.op=token;
        q->child[0]=t;
        match(token);
        q->child[1]=factor(k);
        t=q;
    }
    return t;
}

TreeNode* factor(TreeNode* k)
{
    TreeNode* t=NULL;
    TreeNode* p=NULL;
    if(k!=NULL)
    {
        if(token==LPAREN&&k->nodekind!=Array_ElemK)   //说明函数调用call
        {
            t=call(k);
        }
        else    //就是普通整型变量或者数组变量
        {
            t=k;
        }
    }
    else
    {
        switch(token)
        {
        case LPAREN:
            match(LPAREN);
            t=expression();
            match(RPAREN);
            break;
        case ID:
            p=var();
            if(token==LPAREN&&p->nodekind!=Array_ElemK)   //函数调用
            {
                t=call(p);
            }
            else   //普通整型变量或数组变量
            {
                t=p;
            }
            break;
        case NUM:
            t=newNode(ConstK);
            match(NUM);
            break;
        default:
            syntaxError("factor中错误\n");
            token=getToken();
            break;
        }
    }
    return t;
}

TreeNode* var()
{
    TreeNode* t=NULL;
    TreeNode* p=NULL;
    TreeNode* q=NULL;
    if(token==ID)
    {
        p=newNode(IdK);
        match(ID);
        if(token==LMBRACKET)
        {
            match(LMBRACKET);
            q=expression();
            match(RMBRACKET);
            t=newNode(Array_ElemK);
            t->child[0]=p;
            t->child[1]=q;
        }
        else
        {
            t=p;
        }
    }
    return t;
}

TreeNode* call(TreeNode* k)
{
    TreeNode* t=newNode(CallK);
    if(k!=NULL)
        t->child[0]=k;
    match(LPAREN);
    if(token==RPAREN)
    {
        match(RPAREN);
        return t;
    }
    else if(k!=NULL&&token!=RPAREN)
    {
        t->child[1]=args();
        match(RPAREN);
    }
    return t;
}

TreeNode* args()
{
    TreeNode* t=newNode(ArgsK);
    TreeNode* s=NULL;
    TreeNode* p=NULL;
    if(token!=RPAREN)
    {
        s=expression();
        p=s;
        while(token==COMMA)
        {
            TreeNode* q;
            match(COMMA);
            q=expression();
            if(q!=NULL)
            {
                if(s==NULL)
                {
                    s=p=q;
                }
                else
                {
                    p->sibling=q;
                    p=q;
                }

            }
        }
    }
    if(s!=NULL)
    {
        t->child[0]=s;
    }
    return t;
}

TreeNode* parse()
{
    TreeNode* t;
    token=getToken();
    t=declaration_list();
    if(token!=ENDFILE)
        syntaxError("Code ends before file\n");
    return t;
}
