#include "globals.h"
#include "util.h"
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
