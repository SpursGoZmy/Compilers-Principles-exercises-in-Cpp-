#include "globals.h"
#include "scanner.h"
#include "util.h"

int lineno=0;
FILE* source;
FILE* listing;
FILE* syntax_tree_file;
FILE* code;

int EchoSource=true;
int TraceScan=true;


int main()
{
    source=fopen("test.txt","r");   //输入文件
    //listing=stdout;
    //fprintf(listing,"C-Minus Scanner: \n");
    //while(getToken()!=ENDFILE);

    TreeNode* syntaxTree;
    syntaxTree=parse();   //进行语法分析
    listing=stdout;
    //fopen("syntax_tree.txt","w");
    fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);         //打印语法树到listing中，为方便起见设置为标准输出流stdout
    fclose(source);
    return 0;
}
