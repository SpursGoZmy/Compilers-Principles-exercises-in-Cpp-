#ifndef SYMTAB_H_INCLUDED
#define SYMTAB_H_INCLUDED

#include "globals.h"
void st_insert(char *name,char *scope, TokenType type,int lineno,int memloc,int isArr);  //������ű�

TokenType search_type(char * name,char * scope);    //�ڷ��ű�����ұ���������

int st_lookup(char * name, char * scope);   //�ڷ��ű�����ұ�����λ��

int search_arr(char *name,char *scope);    //ȷ��ĳһ�������Ƿ�������,����0Ϊ�٣�����1Ϊ��

void printSymTab(FILE* listing);    //������ű�

#endif // SYMTAB_H_INCLUDED
