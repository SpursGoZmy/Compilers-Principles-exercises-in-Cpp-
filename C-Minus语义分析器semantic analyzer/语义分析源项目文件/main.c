#include "globals.h"
#include "scanner.h"
#include "util.h"
#include "parse.h"
#include "analyze.h"


int lineno=0;
FILE* source;
FILE* listing;
FILE* syntax_tree_file;
FILE* code;

int EchoSource=true;
int TraceScan=true;

All_scopes* Scope_List;
int main()
{
    Scope_List=(All_scopes*) malloc(sizeof(struct scope_list));
    Scope_List->current_num=1;
    Scope_List->scope_names[0]=copyString(scope);



    source=fopen("test.txt","r");   //输入文件
    listing=stdout;

    fprintf(listing,"C-Minus Scanner: \n");
    TreeNode* syntaxTree;

    syntaxTree=parse();   //进行语法分析,同时打印扫描的结果



    fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);         //打印语法树到listing中，为方便起见设置为标准输出流stdout

    printf("所有存在的作用域为：\n");         //打印所有的作用域
    for(int i=0;i<Scope_List->current_num;i++)
        printf("%s\n",Scope_List->scope_names[i]);

    buildSymtab(syntaxTree);        //构造符号表
    printSymTab(listing);            //打印符号表
    fclose(source);
    return 0;
}
