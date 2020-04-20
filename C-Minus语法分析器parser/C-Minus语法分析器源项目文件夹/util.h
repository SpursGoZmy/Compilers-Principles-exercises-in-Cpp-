#ifndef UTIL_H_INCLUDED
#define UTIL_H_INCLUDED
void printToken(TokenType token ,const char*);

TreeNode* newNode(NodeKind kind);
char* copyString(char *);
void printTree(TreeNode*);

#endif // UTIL_H_INCLUDED
