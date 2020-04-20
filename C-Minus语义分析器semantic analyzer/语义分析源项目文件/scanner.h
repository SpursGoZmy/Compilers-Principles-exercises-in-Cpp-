#ifndef SCANNER_H_INCLUDED
#define SCANNER_H_INCLUDED

#define MAXTOKENLEN 40
extern char tokenString[MAXTOKENLEN+1];
TokenType getToken();

#endif // SCANNER_H_INCLUDED
