#include "globals.h"
#include "scanner.h"
#include "util.h"

int lineno=0;
FILE* source;
FILE* listing;
FILE* code;

int EchoSource=true;
int TraceScan=true;


int main()
{
    source=fopen("test.txt","r");
    listing=stdout;
    fprintf(listing,"C-Minus Scanner: \n");
    while(getToken()!=ENDFILE);
    fclose(source);
    return 0;
}
