#ifndef TOKEN_H
#define TOKEN_H

#include <string>

enum TokenType {

    // identifiers and literals
    T_IDENTIFIER,
    T_INTEGER,
    T_STRING,
    T_FLOAT,

    // keywords
    T_PROGRAM,
    T_VAR,
    T_CONSTANT,
    T_INTEGER_KW,
    T_BOOL,
    T_STRING_KW,
    T_FLOAT_KW,
    T_TRUE,
    T_FALSE,
    T_IF,
    T_FI,
    T_THEN,
    T_ELSE,
    T_WHILE,
    T_DO,
    T_OD,
    T_AND,
    T_OR,
    T_READ,
    T_WRITE,
    T_FOR,
    T_FROM,
    T_TO,
    T_BY,

    // operators
    T_LPAREN,
    T_RPAREN,
    T_ASSIGN,      // :=
    T_PLUS,
    T_MINUS,
    T_MULT,
    T_DIV,
    T_EQUAL,
    T_NOTEQUAL,
    T_LT,
    T_LTE,
    T_GT,
    T_GTE,
    T_DOT,
    T_SEMICOLON,
    T_COMMA,
    T_LBRACKET,
    T_RBRACKET,
    T_LBRACE,
    T_RBRACE,
    T_COLON,

    // special
    T_EOF,
    T_ERROR
};

struct Token {

    TokenType type;

    std::string str_value="";  // for identifiers and strings
    int int_value=0;          // for integers
    float float_value = 0.0f;
};

#endif