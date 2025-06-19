/*
Name : Pruthviraj K P

ID : 25002_125

project : Lexical Analyzer

project description : This project is a Lexical Analyzer developed in C that reads a .c source file and breaks it down into individual tokens
 such as keywords, operators, identifiers, constants, literals, preprocessor directives, and comments. It includes functionality to detect
 syntax errors like unmatched parentheses or braces and categorizes each token accurately using a structured approach with modular
 header and source files. The program is executed via command-line, offering real-time token parsing and classification output,
 simulating a basic compiler front-end.
*/
#include <stdio.h>
#include "lexer.h"
#include <unistd.h>
#include <stdlib.h>
int main(int argc, char *argv[])
{
        if (argc == 1 || argc > 2)
        {
                printf("ERROR : COMMAND LINE ARGUMENT NEEDS TO BE PASSED AS ./a.out <filename>\n");
                return 1;
        }
        FILE *fptr = initializeLexer(argv[1]);
        fseek(fptr, 0, SEEK_END);
        int size = ftell(fptr);
        printf("passed file size %d\n", size);
        fseek(fptr, 0, SEEK_SET);
        Token token,prevtok;
        while ((token = getNextToken(fptr, size,prevtok)).type != UNKNOWN)
        {       
                prevtok=token;
                int id = token.type;
                switch (id)
                {
                case 0: printf("Type : KEYWORD          "); break;
                case 1: printf("Type : OPERATOR         "); break;
                case 2: printf("Type : SPECIAL_CHARACTER"); break;
                case 3: printf("Type : CONSTANT         "); break;
                case 4: printf("Type : IDENTIFIER       "); break;
                case 5: printf("Type : LITERAL          "); break;
                case 6: printf("Type : PREPROCESSOR     "); break;
                case 7: printf("Type : COMMENTED_LINE   "); break;
                default:printf("default\t");
                }
                int limit = ftell(fptr);
                int linecont = linecount(fptr, limit);
                printf("----> Token: %-23s  line %d\n", token.lexeme,linecont+1);
                if (ftell(fptr) >= size)
                        break;
                
        }

        printf("Parsing  : %s : Done\n", argv[1]);
        return 0;
}
