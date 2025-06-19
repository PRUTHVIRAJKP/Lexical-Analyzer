#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include "lexer.h"
#include <stdlib.h>
#include <unistd.h>
static const char *keywords[MAX_KEYWORDS] = {
    "int", "float", "return", "if", "else", "while", "for", "do", "break", "continue",
    "char", "double", "void", "switch", "case", "default", "const", "static", "sizeof", "struct"};

static const char *stdwords[MAX_STDWORDS] = {
    "int", "float", "return", "break", "continue", "char", "double", "void", "const", "static", "struct", "printf", "scanf"};

static const char *operators = "+-*/%=!<>|&";
static const char *specialCharacters = ",;{}()[]";

FILE *initializeLexer(const char *filename) // this function initializes lexer by opening attetsted .c file in the command line argument
{
    FILE *fptr;
    fptr = fopen(filename, "r");
    if (fptr == NULL)
    {
        fprintf(stderr, "ERROR : FILE NOT OPENING\n");
        return NULL;
    }
    printf("File %s is opening succesfully\n", filename);
    printf("Parsing\t: %s :  Started\n", filename);
    return fptr;
}
Token getNextToken(FILE *fptr, int size, Token prevtok) // this function is used to get next token
{
    Token tokensam;
    char ch, str[1000];
    fread(&ch, 1, 1, fptr);
    if (ch == EOF) // here it checks for the EOF
    {
        printf("End of file is encountered\n");
        tokensam.type = UNKNOWN;
        return tokensam;
    } // sleep(1);
    while (ch == ' ' || ch == '\n' || ch == '\r') // in this loop it tries to skip the space, newline, \r
    {
        fread(&ch, 1, 1, fptr);
        if (ftell(fptr) >= size)
        {
            printf("Parsing : Done\n");
            exit(1);
        }
    }
    if (ch == '(')
    {
        int sizef = ftell(fptr);
        if (checkendpar(fptr, size) == 1)
            fseek(fptr, -1, SEEK_CUR);
        else
        {
            printf("ERROR : paranthesis is not ended\n");
            exit(1);
        }
        int sizel = ftell(fptr);
        int move = sizef - sizel;
        fseek(fptr, move, SEEK_CUR);
    }

    if (ch == '"')
    {
        int sizef = ftell(fptr);
        //printf("%ld\n",ftell(fptr));
        int flag = 0;
        char chtemp = fgetc(fptr);
        // printf("%ld\n",ftell(fptr));
        fseek(fptr, -1, SEEK_CUR);
        if (chtemp == '\''){
            flag = 1; //printf("%c\n", chtemp);
        }
        if (flag == 0)
        {
            if (checkendinvertedcoma(fptr, size) == 1)
                fseek(fptr, -1, SEEK_CUR);
            else
            {
                printf("ERROR: double inverted coma  is not ended\n");
                exit(1);
            }
        }
        int sizel = ftell(fptr);
        int move = sizef - sizel;
        // printf("%ld\n",ftell(fptr));
        fseek(fptr, move, SEEK_CUR);
        // printf("%ld\n",ftell(fptr));

    }

    if (ch == '[')
    {
        int sizef = ftell(fptr);

        int flag = 0;
        char chtemp = fgetc(fptr);
        //printf("%c\n", chtemp);
        fseek(fptr, -1, SEEK_CUR);
        if (chtemp == '\''){
            flag = 1;       printf("%c\n", chtemp);
        }
        printf("%c\n", chtemp);
        if (flag == 0)
        {
            if (checkendsqarebracket(fptr, size) == 1)
                fseek(fptr, -1, SEEK_CUR);
            else
            {
                printf("ERROR: SQUARE BRACKET is not ended\n");
                exit(1);
            }
        }
        int sizel = ftell(fptr);
        int move = sizef - sizel;
        fseek(fptr, move, SEEK_CUR);
    }

    if (ch == '\'') // here it checks for the single inverted coma
    {
        char c;
        fread(&c, 1, 1, fptr);
        int flag = 0;
        if (c == ' ' || c == '\'')
            flag = 1;
        if (c == '\\')
        {
            fread(&c, 1, 1, fptr);
            if (c == 'n' || c == 't' || c == 'r' || c == '\'')
            {
                flag = 1;
                fseek(fptr, -2, SEEK_CUR);
            }
        }
        if (c == ';' || c == '\n' || c == ',' || c == '\r' || c == ')')
        {
            flag = 1;
            //fseek(fptr, -1, SEEK_CUR);
        }
        if (flag == 0)
        {
            fread(&c, 1, 1, fptr);
            int innerflag = 0;
            fseek(fptr, -2, SEEK_CUR);
            if (c == '\'')
                innerflag = 1;
            if (innerflag == 0)
            {
                int sizef = ftell(fptr);
                int ans = checkendsingleinvertedcoma(fptr, size);
                int limit = ftell(fptr);
                int linecont = linecount(fptr, limit);
                if (ans == 1)
                {
                    printf("ERROR : Single inverted coma should has single character, multiple character in a single inverted coma is not valid in line %d\n", linecont + 1);
                    exit(1);
                }
                else
                {
                    printf("ERROR : single inverted coma  is not ended in the line, expected single inverted coma in the line %d\n", linecont);
                    exit(1);
                }
                int sizel = ftell(fptr);
                int move = sizef - sizel;
                fseek(fptr, move, SEEK_CUR);
            }
        }
    }
    if (ch == '{') // here it checks for the flower bracket
    {
        int sizef = ftell(fptr);
        if (checkendflower(fptr, size) == 1)
            fseek(fptr, -1, SEEK_CUR);
        else
        {
            printf("ERROR : FLOWER BRACKET is not ended\n");
            exit(1);
        }
        int sizel = ftell(fptr);
        int move = sizef - sizel;
        fseek(fptr, move, SEEK_CUR);
    }

    if (ch == '#') // it checks for the preprocessor having # as its first character
    {
        int i = 0;
        while (1)
        {
            str[i] = ch;
            fread(&ch, 1, 1, fptr);
            i++;
            if (ch == '\n' || ch == '\r')
                break;
        }
        str[i] = '\0';
        strcpy(tokensam.lexeme, str);
        tokensam = categorizeToken(tokensam, fptr, size, prevtok);
        return tokensam;
    }

    if (ch == '/') // it checks for the single line comment having // or /*as its first two character
    {
        int i = 1;
        str[0] = ch;
        fread(&ch, 1, 1, fptr);
        if (ch == '/')
        {
            while (1)
            {
                str[i] = ch;
                fread(&ch, 1, 1, fptr);
                i++;
                if (ch == '\n' || ch == '\r')
                    break;
            }
            str[i] = '\0';
            strcpy(tokensam.lexeme, str);
            tokensam = categorizeToken(tokensam, fptr, size, prevtok);
            return tokensam;
        }
        if (ch == '*')
        {
            while (1)
            {
                str[i] = ch;
                fread(&ch, 1, 1, fptr);
                i++;
                if (ch == '*')
                    break;
            }
            fseek(fptr, 1, SEEK_CUR);
            str[i] = '\0';
            strcpy(tokensam.lexeme, str);
            tokensam = categorizeToken(tokensam, fptr, size, prevtok);
            return tokensam;
        }
        else
        {
            fseek(fptr, -2, SEEK_CUR);
        }
    }

    if (ch == '"') // here it would check for the " for a string literal as its first char
    {
        int i = 0;
        while (1)
        {
            str[i] = ch;
            fread(&ch, 1, 1, fptr);
            i++;
            if (ch == '"') // loop breaks when there is second " for a string literal
                break;
        }
        str[i] = '"';
        str[i + 1] = '\0';
        strcpy(tokensam.lexeme, str);
        tokensam = categorizeToken(tokensam, fptr, size, prevtok);
        return tokensam;
    }
    if (ch >= '0' && ch <= '9')
    {
        int i = 0;
        while (ch >= '0' && ch <= '9')
        {
            str[i] = ch;
            ch = fgetc(fptr);
            i++;
            if ((ch >= '0' && ch <= '9') == 0)
                break;
        }
        str[i] = '\0';
        fseek(fptr, -1, SEEK_CUR);
        strcpy(tokensam.lexeme, str);
        tokensam = categorizeToken(tokensam, fptr, size, prevtok);
        return tokensam;
    }
    if ((ch >= 'A' && ch <= 'Z') || (ch >= 'a' && ch <= 'z')) // checks for the char is alphabatic or not
    {
        char ch2;
        str[0] = ch;
        int i = 1, flag = 0;
        while (1)
        {
            fread(&ch2, 1, 1, fptr);
            for (int j = 0; j < 11; j++)
            {
                if (ch2 == operators[j] || ch2 == ' ' || ch2 == '\'')
                {
                    flag = 1;
                    str[i] = '\0';
                }
            }
            for (int j = 0; j < 8; j++)
            {
                if (ch2 == specialCharacters[j])
                {
                    flag = 1;
                    str[i] = '\0';
                }
            }
            if (flag == 1)
                break;
            str[i] = ch2;
            i++;
        }
        str[i] = '\0';
        fseek(fptr, -1, SEEK_CUR);
        strcpy(tokensam.lexeme, str);
    }
    else
    {
        str[0] = ch;
        str[1] = '\0'; // if it didnt find any thing nor alphabait or " , then it may be a specal character
        strcpy(tokensam.lexeme, str);
    }
    tokensam = categorizeToken(tokensam, fptr, size, prevtok);
    return tokensam;
}
int isKeyword(const char *str) // checks for whether the passing string is keyword or not
{
    for (int i = 0; i < 20; i++)
    {
        if (strcmp(str, keywords[i]) == 0)
            return 1;
    }
    return 0;
}

int isOperator(const char *str) // checks whether the passing string isoperator
{
    char ch = str[0];
    for (int i = 0; i < 9; i++)
    {
        if (ch == operators[i])
            return 1;
    }
    return 0;
}

int isSpecialCharacter(const char *str) // checks whether the passing string is specail character or not
{
    char ch = str[0];
    for (int i = 0; i < 10; i++)
    {
        if (ch == specialCharacters[i])
            return 1;
    }
    return 0;
}

int isConstant(const char *str) // checks whetherr the passing string is constant or not
{
    char ch = str[0];
    if (ch >= '0' && ch <= '9')
    {
        return 1;
    }
    return 0;
}

int isliteral(const char *str) // checks whether the passing string is string literal or not
{
    char ch = str[0];
    if (ch == '"')
        return 1;
    else
        return 0;
}

int ispreprocessor(const char *str) // checks whether the string is preprocessor or not
{
    char ch = str[0];
    // printf("%c\n",ch);
    if (ch == '#')
        return 1;
    else
        return 0;
}

int iscomment(const char *str) // checks whether the string is preprocessor or not
{
    char ch = str[0];
    char ch2 = str[1];
    if ((ch == '/' && ch2 == '*') || (ch == '/' && ch2 == '/'))
        return 1;
    else
        return 0;
}

int checkendpar(FILE *fptr, int size) // This function called to find whether the respective closing paranthesis is found within the line
{
    char c;
    while (1)
    {
        fread(&c, 1, 1, fptr);
        if (c == ')')
        {
            return 1;
        }
        if (c == '\n' || c == '\r')
        {
            int limit = ftell(fptr);
            int linecont = linecount(fptr, limit);
            printf("Expected paranthesis end ) at line %d\n", linecont);
            return 0;
        }
    }
}

int checkendsingleinvertedcoma(FILE *fptr, int size) // checks whehter the line has single inverted coma or
{
    char c;
    int posstart = ftell(fptr);
    while (1)
    {
        fread(&c, 1, 1, fptr);
        if (c == '\'' || c == '"')
        {
            return 1;
        }
        if (c == '\n' || c == '\r')
        {
            int limit = ftell(fptr);
            int linecont = linecount(fptr, limit);
            // printf("single inverted coma is not found, expected double inverted coma in line %d\n", linecont);
            return 0;
        }
    }
}

int checkendinvertedcoma(FILE *fptr, int size) // This function called to find whether the respective closing paranthesis is found within the line
{
    char c;
    while (1)
    {
        fread(&c, 1, 1, fptr);
        if (c == '"')
        {
            return 1;
        }
        if (c == '\n' || c == '\r')
        {
            int limit = ftell(fptr);
            int linecont = linecount(fptr, limit);
            printf("double inverted coma is not found, expected double inverted coma in line %d\n", linecont);
            return 0;
        }
    }
}

int checkendsqarebracket(FILE *fptr, int size) // This function called to find whether the respective closing paranthesis is found within the line
{
    char c;
    while (1)
    {
        fread(&c, 1, 1, fptr);
        if (c == ']')
        {
            return 1;
        }
        if (c == '\n' || c == '\r')
        {
            int limit = ftell(fptr);
            int linecont = linecount(fptr, limit);
            printf("ERROR : SQUARE BRACKET is not found, expected SQUARE BRACKET in line %d\n", linecont);
            return 0;
        }
    }
}

int checkendflower(FILE *fptr, int size) // This function called to find whether the respective closing flowerbracket is found within the line
{
    char c;
    int posstart = ftell(fptr);
    while (1)
    {
        fread(&c, 1, 1, fptr);
        if (c == '}')
        {
            return 1;
        }
        if (ftell(fptr) >= size)
        {
            rewind(fptr);
            fseek(fptr, posstart, SEEK_CUR);
            int limit = ftell(fptr);
            int linecont = linecount(fptr, limit);
            printf("} is not found, expected to close the flower bracket of line %d\n", linecont + 1);
            return 0;
        }
    }
}

int checksemicolon(const char *str, FILE *fptr, int size, Token prevtok) // this function checks whether the line ends with the semicolon or not
{
    int posstart = ftell(fptr), semicoloncount = 0, flag = 0, mainflag = 0;
    char ch;
    if (strcmp(str, "int") == 0)
    {
        Token sample = getNextToken(fptr, size, prevtok);
        char *strdump = sample.lexeme;
        if (strcmp(strdump, "main") == 0)
        {
            mainflag = 1;
        }
    }
    if (mainflag == 0)
    {
        for (int i = 0; i < MAX_STDWORDS; i++)
        {
            if (strcmp(str, stdwords[i]) == 0)
            { // sleep(1);
                flag = 1;
                ch = fgetc(fptr);
                int size = strlen(str);
                fseek(fptr, -(size + 2), SEEK_CUR);
                char cb = fgetc(fptr);
                //printf("\n%c\n", cb);
                if (cb == '(' || (strcmp(prevtok.lexeme, "const") == 0) || cb == ',')
                { // flag=0;
                    semicoloncount++;
                    break;
                }
               // printf("\n%d this is a size", size);
               // printf("\n%s this is a string\n", str);
                //printf("%c ", ch);
                while (ch != '\n')
                {

                    //printf("%c ", ch);
                    if (ch == ';')
                        semicoloncount++;

                    if (ch == '{')
                    {
                        semicoloncount++;
                    }
                    ch = fgetc(fptr);
                    if (ch == '\r')
                        continue;
                }
            }
        }
        if (flag == 0)
        {
            return 1;
        }
        if (semicoloncount == 0)
        {
            int limit = ftell(fptr);
            int linecont = linecount(fptr, limit);
            printf("ERROR : Expected ';' at the end of the line with token %s at line %d\n", str, linecont);
            exit(1);
        }
    }
    int possend = ftell(fptr);
    int net = possend - posstart;
    fseek(fptr, -net, SEEK_CUR);
}
int linecount(FILE *fptr, int limit) // this funciton is used to count the no of lines to the line of error
{
    rewind(fptr);
    int linecount = 0;
    char ch;
    while (ftell(fptr) <= limit)
    {
        ch = fgetc(fptr);
        if (ch == '\n')
            linecount++;
    }
    return linecount;
}
Token categorizeToken(Token tokensam, FILE *fptr, int size, Token prevtok) // funciton called to categorize the passed token to which type is it actually
{
     int n = checksemicolon(tokensam.lexeme, fptr, size,prevtok);
    if (isKeyword(tokensam.lexeme) == 1)
        tokensam.type = KEYWORD;
    else if (iscomment(tokensam.lexeme) == 1)
        tokensam.type = COMMENTED_LINE;
    else if (isOperator(tokensam.lexeme) == 1)
        tokensam.type = OPERATOR;
    else if (isSpecialCharacter(tokensam.lexeme) == 1)
        tokensam.type = SPECIAL_CHARACTER;
    else if (isConstant(tokensam.lexeme) == 1)
        tokensam.type = CONSTANT;
    else if (isliteral(tokensam.lexeme) == 1)
        tokensam.type = LITERAL;
    else if (ispreprocessor(tokensam.lexeme) == 1)
        tokensam.type = PREPROCESSOR;
    else
        tokensam.type = IDENTIFIER;
    return tokensam;
}