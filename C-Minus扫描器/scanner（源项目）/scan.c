#include "globals.h"
#include "scanner.h"
#include "util.h"
#define BUFLEN 256   //定义每次读取的一行代码长度
//DFA状态
typedef enum
{
    START,INASSIGN,INEQ,INLE,INGE,INNEQ,ENTERCOMMENT,LEAVECOMMENT,INCOMMENT,INNUM,INID,DONE
}StateType;

//保留字结构体定义和赋值
static struct
{
    char* str;
    TokenType token;
}reservedWords[MAXRESERVED]
={{"else",ELSE},{"if",IF},{"int",INT},{"return",RETURN},{"void",VOID},{"while",WHILE}};


char tokenString[MAXTOKENLEN+1];

static char lineBuf[BUFLEN];   //存储当前行
static int linepos=0;    //当前行所读的字符位置
static int bufsize=0;     //当前行字符串长度
static int EOF_Flag =false;

static int getNextChar()    //读取下一个字符
{
    if(linepos>=bufsize)
    {
        lineno++;
        if(fgets(lineBuf,BUFLEN-1,source))
        {
            if(EchoSource)
                fprintf(listing,"%4d: %s",lineno,lineBuf);
            bufsize=strlen(lineBuf);
            linepos=0;
            return lineBuf[linepos++];
        }
        else
        {
            EOF_Flag=true;
            return EOF;
        }
    }
    else
    {
        return lineBuf[linepos++];
    }
}

static void ungetNextChar()     //考虑先行问题，反填字符
{
    if(!EOF_Flag)
        linepos--;
}

static TokenType reservedLookup(char*s)   //查询记号是否是保留字
{
    int i;
    for(i=0;i<MAXRESERVED;i++)
        if(!strcmp(s,reservedWords[i].str))
        return reservedWords[i].token;
    return ID;
}

TokenType getToken()    //扫描程序主要函数，得到一个记号
{
    int tokenStringIndex=0;
    TokenType currentToken;
    StateType state=START;
    int save;
    while(state!=DONE)       //利用双重case语句实现DFA
    {
        int c=getNextChar();
        save=true;
        switch(state)
        {
        case START:
            if(isdigit(c))
                state=INNUM;
            else if(isalpha(c))
                state=INID;
            else if(c=='=')
                state=INASSIGN;
            else if(c=='<')
                state=INLE;
            else if(c=='>')
                state=INGE;
            else if(c=='!')
                state=INNEQ;
            else if((c==' ')||(c=='\t')||(c=='\n'))
                save=false;
            else if(c=='/')
                state=ENTERCOMMENT;
            else
            {
                state=DONE;
                switch(c)
                {
                case EOF:
                    save=false;
                    currentToken=ENDFILE;
                    break;
                case '+':
                    currentToken=PLUS;
                    break;
                case '-':
                    currentToken=MINUS;
                    break;
                case '*':
                    currentToken=TIMES;
                    break;
                case ';':
                    currentToken=SEMI;
                    break;
                case ',':
                    currentToken=COMMA;
                    break;
                case '(':
                    currentToken=LPAREN;
                    break;
                case ')':
                    currentToken=RPAREN;
                    break;
                case '[':
                    currentToken=LMBRACKET;
                    break;
                case ']':
                    currentToken=RMBRACKET;
                    break;
                case '{':
                    currentToken=LBBRACKET;
                    break;
                case '}':
                    currentToken=RBBRACKET;
                    break;
                default:
                    currentToken=ERROR;
                    break;
                }
            }
            break;
        case INASSIGN:
            state=DONE;
            if(c=='=')
            {
                currentToken=EQ;
            }
            else
            {
                ungetNextChar();
                save=false;
                currentToken=ASSIGN;
            }
            break;
        case INLE:
            state=DONE;
            if(c=='=')
            {
                currentToken=LEQ;
            }
            else
            {
                ungetNextChar();
                save=false;
                currentToken=LT;
            }
            break;
        case INGE:
            state=DONE;
            if(c=='=')
            {
                currentToken=GEQ;
            }
            else
            {
                ungetNextChar();
                save=false;
                currentToken=GT;
            }
            break;
        case INNEQ:
            state=DONE;
            if(c=='=')
            {
                currentToken=NEQ;
            }
            else
            {
                ungetNextChar();
                save=false;
                currentToken=ERROR;
            }
            break;
        case ENTERCOMMENT:
            if(c=='*')
            {
                state=INCOMMENT;
                save=false;
                tokenString[0]=' ';
            }
            else if(c==EOF)
            {
                state=DONE;
                currentToken =ENDFILE;
                save=false;
            }
            else
            {
                state=DONE;
                ungetNextChar();
                save=false;
                currentToken=OVER;
            }
            break;
        case INCOMMENT:
            save=false;
            if(c=='*')
            {
                state=LEAVECOMMENT;
            }
            else if(c==EOF)
            {
                state=DONE;
                currentToken=ENDFILE;
            }
            else
            {
                state=INCOMMENT;
            }
            break;
        case LEAVECOMMENT:
            if(c=='*')
            {
                state=LEAVECOMMENT;
            }
            else if(c=='/')
            {
                state=START;
            }
            else if(c==EOF)
            {
                state=DONE;
                currentToken=ENDFILE;
            }
            else
            {
                state=INCOMMENT;
            }
            break;
        case INNUM:
            if(!isdigit(c))
            {
                ungetNextChar();
                save=false;
                state=DONE;
                currentToken=NUM;
            }
            break;
        case INID:
            if(!isalpha(c))
            {
                ungetNextChar();
                save=false;
                state=DONE;
                currentToken=ID;
            }
            break;
        case DONE:
        default:
            fprintf(listing,"scanner bug: state=%d\n",state);
            state=DONE;
            currentToken=ERROR;
            break;


        }//大switch语句结束
        if((save)&&(tokenStringIndex<=MAXTOKENLEN))
        {
            tokenString[tokenStringIndex++]=(char)c;
        }
        if(state==DONE)
        {
            tokenString[tokenStringIndex]='\0';
            if(currentToken==ID)
                currentToken=reservedLookup(tokenString);
        }
    }

    if(TraceScan)
    {
        fprintf(listing,"\t%d ",lineno);
        printToken(currentToken,tokenString);
    }
    return currentToken;
}
