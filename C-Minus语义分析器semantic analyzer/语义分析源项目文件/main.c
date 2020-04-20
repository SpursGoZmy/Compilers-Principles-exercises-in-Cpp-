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



    source=fopen("test.txt","r");   //�����ļ�
    listing=stdout;

    fprintf(listing,"C-Minus Scanner: \n");
    TreeNode* syntaxTree;

    syntaxTree=parse();   //�����﷨����,ͬʱ��ӡɨ��Ľ��



    fprintf(listing,"\nSyntax tree:\n");
    printTree(syntaxTree);         //��ӡ�﷨����listing�У�Ϊ�����������Ϊ��׼�����stdout

    printf("���д��ڵ�������Ϊ��\n");         //��ӡ���е�������
    for(int i=0;i<Scope_List->current_num;i++)
        printf("%s\n",Scope_List->scope_names[i]);

    buildSymtab(syntaxTree);        //������ű�
    printSymTab(listing);            //��ӡ���ű�
    fclose(source);
    return 0;
}
