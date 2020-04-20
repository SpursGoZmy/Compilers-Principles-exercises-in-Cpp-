#ifndef SYMTAB_H_INCLUDED
#define SYMTAB_H_INCLUDED

#include "globals.h"
void st_insert(char *name,char *scope, TokenType type,int lineno,int memloc,int isArr);  //插入符号表

TokenType search_type(char * name,char * scope);    //在符号表里查找变量的类型

int st_lookup(char * name, char * scope);   //在符号表里查找变量的位置

int search_arr(char *name,char *scope);    //确定某一个变量是否是数组,返回0为假，返回1为真

void printSymTab(FILE* listing);    //输出符号表

#endif // SYMTAB_H_INCLUDED
