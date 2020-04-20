#ifndef GLOBALS_H_INCLUDED
#define GLOBALS_H_INCLUDED

#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<string.h>

#define MAXRESERVED 6
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
#endif // GLOBALS_H_INCLUDED
