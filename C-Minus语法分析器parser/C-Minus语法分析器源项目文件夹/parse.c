#include "globals.h"
#include "scanner.h"
#include "util.h"
#include "parse.h"

static TokenType token;

static TreeNode* declaration_list();   //�����б�
static TreeNode* declaration();      //��������Ϊ���������ͺ�������
static TreeNode* params();       //�����Ĳ����б�
static TreeNode* param();       //ÿһ������
static TreeNode* compound_stmt();    //�����ĸ������
static TreeNode* local_declaration();    //�����ڲ��ľֲ���������
static TreeNode* statement_list();       //����б�
static TreeNode* statement();           //���������
static TreeNode* expression_stmt();        //���ʽ���
static TreeNode* selection_stmt();        //if���
static TreeNode* iteration_stmt();        //while���
static TreeNode* return_stmt();       //�������
static TreeNode* expression();        //���ʽ�������Ǹ�ֵ����򵥱��ʽ
static TreeNode* simple_expression(TreeNode* k);        //�򵥱��ʽ
static TreeNode* var();                          //����
static TreeNode* additive_expression(TreeNode* k);    //�ɼӱ��ʽ
static TreeNode* term(TreeNode* k);            //�˻�
static TreeNode* factor(TreeNode* k);         //����
static TreeNode* call(TreeNode* k);           //��������
static TreeNode* args();         //ʵ���б�


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
    while(token!=INT&&token!=VOID&&token!=ENDFILE)      //���һ��ʼ�﷨�������ִ��󣬾��������󲿷�ֱ����int��void
    {
        syntaxError(" ");
        token=getToken();
        if(token==ENDFILE)
            {
                fprintf(listing,"end of File\n");
                break;
            }
    }
    while(token==INT||token==VOID)     //����int��void������������ʼ�﷨��������Ϊ����������int��void��ʼ
    {
        TreeNode* q=declaration();
        if(q!=NULL)
        {
            if(t==NULL)     //���֮ǰ��һ���﷨����������ô�Ͱ����ҵ���declaration�����Ϊ��һ�����
            {
                t=p=q;
            }
            else
            {
                p->sibling=q;  //���������������ҵ���declaration�����Ϊ��һ�������ֵ�
                p=q;
            }
        }
    }
    match(ENDFILE);
    return t;
}

TreeNode* declaration()
{
    TreeNode* t=NULL;    //tΪ���Ҫ���ص��������ڵ�
    TreeNode* p=NULL;
    TreeNode* q=NULL;
    TreeNode* s=NULL;

    if(token==INT)    //�ȸ��ݵ�һ��token��int����void����һ���½��
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
        syntaxError("declaration������ƥ�����");
    }

    if((p!=NULL)&&(token==ID))  //ǰ��û�г������ҽ����ŵڶ���tokenΪID
    {
        q=newNode(IdK);
        //q->attr.name=tokenString;
        match(ID);

        if(token==LPAREN)    //��һ��tokenΪ ( ,˵��������Ϊ��������
        {
            t=newNode(FunK);
            t->child[0]=p;
            t->child[1]=q;
            match(LPAREN);
            t->child[2]=params();
            match(RPAREN);
            t->child[3]=compound_stmt();
        }
        else if(token==LMBRACKET)     //��һ��tokenΪ [ ,˵��������Ϊ��������
        {
            t=newNode(Var_DeclK);
            TreeNode* m=newNode(Array_DeclK);
            match(LMBRACKET);
            match(NUM);
            s=newNode(ConstK);
            m->child[0]=q;   //������ĵ�һ������Ϊ������Id
            m->child[1]=s;    //������ĵڶ�������Ϊ�����СNum
            t->child[0]=p;        //�����������ĵ�һ������Ϊ��������p
            t->child[1]=m;        //�����������ĵڶ�������Ϊ������m
            match(RMBRACKET);
            match(SEMI);
        }
        else if(token==SEMI)      //��һ��tokenΪ ��,˵��Ϊ��ͨ��������
        {
            t=newNode(Var_DeclK);
            t->child[0]=p;     //��һ������Ϊ��������,Ӧ��Ϊint
            t->child[1]=q;     //�ڶ�������Ϊ�������֣�Ӧ��ΪId
            match(SEMI);
        }
        else
        {
            syntaxError("����ƥ�����λ��declaration�У�ID����������token");
        }
    }
    else
    {
        syntaxError("����ƥ�����λ��declaration�У�int��void����û�и���Id");
    }
    return t;
}

TreeNode* params()
{
    TreeNode* t=newNode(ParamsK);
    TreeNode* p=NULL;
    TreeNode* q=NULL;
    //TreeNode* s=NULL;

    if(token==VOID)     //��������Ϊvoid
    {
        p=newNode(VoidK);
        match(VOID);
        if(token!=RPAREN)
            {
                syntaxError("��������VOID����û�и�������");
            }
        if(t!=NULL)
            t->child[0]=p;
    }
    else if(token==INT)   //��������Ϊһ��������param()���
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
        syntaxError("�����������ʹ���\n");
    }
    return t;
}

TreeNode* param()       //��������Int����������������Ľ��
{
    TreeNode* t=NULL;
    TreeNode* p=NULL;
    TreeNode* q=NULL;
    match(INT);
    p=newNode(IntK);
    q=newNode(IdK);
    match(ID);
    if(token==LMBRACKET)   //˵��Ϊ����������� int a[]
    {
        match(LMBRACKET);
        match(RMBRACKET);
        t=newNode(Array_ParamK);
        t->child[0]=p;
        t->child[1]=q;
    }
    else if(token==COMMA)   //˵��Ϊ��ͨInt����
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
        syntaxError("�����ָ���ʽ����");
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

    if(token==INT)   //Ҫע����������ֻ����int ������int a��int a[]
        {
            q1=newNode(IntK);
            match(INT);
        }
    if(token==ID)
        {
            q2=newNode(IdK);
            match(ID);
        }
    if(token==LMBRACKET)   //�������
    {
        match(LMBRACKET);
        q3=newNode(ConstK);  //���鳤��
        match(NUM);
        match(RMBRACKET);
        p=newNode(Array_DeclK);
        p->child[0]=q1;
        p->child[1]=q2;
        p->child[2]=q3;
        t=p;
        match(SEMI);

    }
    else if(token==SEMI)   //��ͨ���ͱ���
    {
        match(SEMI);
        p=newNode(Var_DeclK);
        p->child[0]=q1;
        p->child[1]=q2;
        t=p;
    }
    else
    {
        syntaxError("��һ���ֲ�����������������\n");
    }

    while(token==INT)
    {
        if(token==INT)   //Ҫע����������ֻ����int ������int a��int a[]
        {
            q1=newNode(IntK);
            match(INT);
        }
        if(token==ID)
        {
            q2=newNode(IdK);
            match(ID);
        }
        if(token==LMBRACKET)   //�������
        {
            match(LMBRACKET);
            q3=newNode(ConstK);  //���鳤��
            match(NUM);
            match(RMBRACKET);
            q=newNode(Array_DeclK);
            q->child[0]=q1;
            q->child[1]=q2;
            q->child[2]=q3;
            if(p!=NULL)
                p->sibling=q;         //�����ֵܽ��
            p=q;
            match(SEMI);

        }
        else if(token==SEMI)   //��ͨ���ͱ���
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
            syntaxError("�ֲ�����������������\n");
        }

    }
    return t;
}

TreeNode* statement_list()
{
    TreeNode* t=statement();
    TreeNode* p=t;
    //�������п��ܵ�statement�Ŀ�ͷ���ж��Ƿ���statement
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

TreeNode* statement()     //��һ���ж�statement������
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
        syntaxError("statementʶ�����");
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
    if(t==NULL)   //����var()��ͷ��˵��ֻ��Ϊsimple_expression
    {
        t=simple_expression(t);
    }
    else     //��var��ͷ�������Ǹ�ֵ��䣬Ҳ������simple_expression��var��call
    {
        TreeNode* p=NULL;
        if(token==ASSIGN)   //��ֵ���
        {
            p=newNode(AssignK);
            match(ASSIGN);
            p->child[0]=t;
            p->child[1]=expression();
            return p;
        }
        else   //˵����simple_expression�е�var��call
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
    //ͨ����һ��token�Ƿ�Ϊrelop���ж�simple_expreession������
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
        if(token==LPAREN&&k->nodekind!=Array_ElemK)   //˵����������call
        {
            t=call(k);
        }
        else    //������ͨ���ͱ��������������
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
            if(token==LPAREN&&p->nodekind!=Array_ElemK)   //��������
            {
                t=call(p);
            }
            else   //��ͨ���ͱ������������
            {
                t=p;
            }
            break;
        case NUM:
            t=newNode(ConstK);
            match(NUM);
            break;
        default:
            syntaxError("factor�д���\n");
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
