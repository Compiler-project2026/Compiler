#include <iostream>
#include <string>
#include "Token.h"

extern Token* yylex();
extern FILE* yyin;

/*
Linux(WSL) (when compiling flex )
Step 1: generate lex.yy.c
cd "/mnt/c/dev/Compiler/scanner/flex scanner"
flex flex_scanner.l

Step 2: compile
g++ lex.yy.c /mnt/c/dev/Compiler/test/test_flex_scanner.cpp \
    -I/mnt/c/dev/Compiler/include \
    -o /mnt/c/dev/Compiler/output/scanner/flex_scanner -lfl

Step 3: run
/mnt/c/dev/Compiler/output/scanner/flex_scanner
*/

    // when changin this file 
 /* -- CHANGED test_flex_scanner.cpp or Token.h (skip flex) --
    Step 1: g++ lex.yy.c /mnt/c/dev/Compiler/test/test_flex_scanner.cpp
                -I/mnt/c/dev/Compiler/include
            -o /mnt/c/dev/Compiler/output/scanner/flex_scanner -lfl
 *   Step 2: /mnt/c/dev/Compiler/output/scanner/flex_scanner
 *
 * -- NO CHANGES, just run --
 *   /mnt/c/dev/Compiler/output/scanner/flex_scanner
 */

/*
Windows (PowerShell)
Step 1: copy lex.yy.c from WSL or generate with win_flex
win_flex flex_scanner.l

Step 2: compile
g++ lex.yy.c C:\Users\Lenovo\Desktop\compiler\Project\test\test_flex_scanner.cpp `
    -IC:\Users\Lenovo\Desktop\compiler\Project\include `
    -o C:\Users\Lenovo\Desktop\compiler\Project\output\flex_scanner.exe

Step 3: run
C:\Users\Lenovo\Desktop\compiler\Project\output\flex_scanner.exe
*/

/* -- CHANGED test_flex_scanner.cpp or Token.h (skip win_flex) --
 *   Step 1: g++ lex.yy.c C:\Users\Lenovo\Desktop\compiler\Project\test\test_flex_scanner.cpp `
 *               -IC:\Users\Lenovo\Desktop\compiler\Project\include `
 *               -o C:\Users\Lenovo\Desktop\compiler\Project\output\flex_scanner.exe
 *   Step 2: C:\Users\Lenovo\Desktop\compiler\Project\output\flex_scanner.exe
 *
 * -- NO CHANGES, just run --
 *   C:\Users\Lenovo\Desktop\compiler\Project\output\flex_scanner.exe
 */

// Function to print a token in readable format
void printToken(Token* token) {
    if (!token) return;

    std::cout << "[Token] ";

    switch(token->type) {
        case T_EOF: std::cout << "EOF"; break;
        case T_IDENTIFIER: std::cout << "IDENTIFIER: " << token->str_value; break;
        case T_INTEGER: std::cout << "INTEGER: " << token->int_value; break;
        case T_STRING: std::cout << "STRING: \"" << token->str_value << "\""; break;
        case T_FLOAT: std::cout << "FLOAT: " << token->float_value; break;

        // Operators
        case T_PLUS: std::cout << "PLUS"; break;
        case T_MINUS: std::cout << "MINUS"; break;
        case T_MULT: std::cout << "MULT"; break;
        case T_DIV: std::cout << "DIV"; break;
        case T_ASSIGN: std::cout << "ASSIGN"; break;
        case T_EQUAL: std::cout << "EQUAL"; break;
        case T_NOTEQUAL: std::cout << "NOTEQUAL"; break;
        case T_LT: std::cout << "LT"; break;
        case T_LTE: std::cout << "LTE"; break;
        case T_GT: std::cout << "GT"; break;
        case T_GTE: std::cout << "GTE"; break;

        // Punctuation
        case T_LPAREN: std::cout << "LPAREN"; break;
        case T_RPAREN: std::cout << "RPAREN"; break;
        case T_LBRACE: std::cout << "LBRACE"; break;
        case T_RBRACE: std::cout << "RBRACE"; break;
        case T_LBRACKET: std::cout << "LBRACKET"; break;
        case T_RBRACKET: std::cout << "RBRACKET"; break;
        case T_SEMICOLON: std::cout << "SEMICOLON"; break;
        case T_COLON: std::cout << "COLON"; break;
        case T_COMMA: std::cout << "COMMA"; break;
        case T_DOT: std::cout << "DOT"; break;

        // Keywords
        case T_PROGRAM: std::cout << "PROGRAM"; break;
        case T_VAR: std::cout << "VAR"; break;
        case T_CONSTANT: std::cout << "CONSTANT"; break;
        case T_INTEGER_KW: std::cout << "INTEGER_KW"; break;
        case T_BOOL: std::cout << "BOOL"; break;
        case T_STRING_KW: std::cout << "STRING_KW"; break;
        case T_FLOAT_KW: std::cout << "FLOAT_KW"; break;
        case T_TRUE: std::cout << "TRUE"; break;
        case T_FALSE: std::cout << "FALSE"; break;
        case T_IF: std::cout << "IF"; break;
        case T_FI: std::cout << "FI"; break;
        case T_THEN: std::cout << "THEN"; break;
        case T_ELSE: std::cout << "ELSE"; break;
        case T_WHILE: std::cout << "WHILE"; break;
        case T_DO: std::cout << "DO"; break;
        case T_OD: std::cout << "OD"; break;
        case T_AND: std::cout << "AND"; break;
        case T_OR: std::cout << "OR"; break;
        case T_READ: std::cout << "READ"; break;
        case T_WRITE: std::cout << "WRITE"; break;
        case T_FOR: std::cout << "FOR"; break;
        case T_FROM: std::cout << "FROM"; break;
        case T_TO: std::cout << "TO"; break;
        case T_BY: std::cout << "BY"; break;

        case T_ERROR: std::cout << "ERROR"; break;
        default: std::cout << "UNKNOWN"; break;
    }

    std::cout << std::endl;
}

int main() {
    const char* filename = "C:/Users/Lenovo/Desktop/compiler/Project/test/test_input.comp"; // Make sure this exists in the same folder as the exe
    const char* filename2 = "/mnt/c/dev/Compiler/test/test_input.comp";
    const char* filename3 = "/mnt/c/dev/Compiler/test/test_scanner.txt";

    yyin = fopen(filename3, "r");
    if (!yyin) {
        std::cerr << "Failed to open input file: " << filename << std::endl;
        return 1;
    }

    while (true) {
        Token* token = yylex();
        printToken(token);

        if (token->type == T_EOF) {
            delete token;
            break;
        }

        delete token;
    }

    fclose(yyin);
    return 0;
}