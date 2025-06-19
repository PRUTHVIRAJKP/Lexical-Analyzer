#include <stdio.h>
#ifndef LEXER_H
#define LEXER_H
#define MAX_KEYWORDS 20
#define MAX_STDWORDS 13
#define MAX_TOKEN_SIZE 1000

typedef enum {
    KEYWORD,
    OPERATOR,
    SPECIAL_CHARACTER,
    CONSTANT,
    IDENTIFIER,
    LITERAL,
    PREPROCESSOR,
    COMMENTED_LINE,
    UNKNOWN
} TokenType;

typedef struct {
    char lexeme[MAX_TOKEN_SIZE];
    TokenType type;
} Token;


FILE * initializeLexer(const char* filename);
Token getNextToken(FILE * fptr,int size,Token prevtok);
Token categorizeToken(Token,FILE *,int size,Token prevtok);
int isKeyword(const char* str);
int isOperator(const char* str);
int isSpecialCharacter(const char* str);
int isConstant(const char* str);
int isIdentifier(const char* str);
int isliteral(const char* str);
int ispreprocessor(const char* str);
int iscomment(const char* str);
int checkendpar(FILE * fptr,int size);
int checkendflower(FILE * fptr,int size);
int checkendinvertedcoma(FILE *fptr, int size);
int checksemicolon(const char* str,FILE *fptr,int size,Token prevtok);
int linecount(FILE *fptr,int limit);
int checkendsingleinvertedcoma(FILE *fptr, int size);
int checkendsqarebracket(FILE *fptr, int size);
#endif
