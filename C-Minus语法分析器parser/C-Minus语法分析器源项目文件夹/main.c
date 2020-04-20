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
    source=fopen("test.txt","r");   //�����ļ�
    //listing=stdout;
    //fprintf(listing,"C-Minus Scanner: \n");
    //while(getToken()!=ENDFILE);

    TreeNode* syntaxTree;
    syntaxTree=parse();   //�����﷨����
    listing=stdout;
    //fopen("syntax_tree.txt","w");
    fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);         //��ӡ�﷨����listing�У�Ϊ�����������Ϊ��׼�����stdout
    fclose(source);
    return 0;
}
