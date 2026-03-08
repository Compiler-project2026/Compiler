%{
#include <stdio.h>
#include<stdlib.h>
#include<string.h>
#include<string>
#include "Token.h"
int line_num = 1;
#define YY_DECL Token* yylex()
%}
%x BLOCK_COMMENT
DIGIT   [0-9]
LETTER  [a-zA-Z_]
SIGN        [+-]?
FLOAT       {SIGN}{DIGIT}+"."{DIGIT}+
BAD_FLOAT       {SIGN}{DIGIT}+"."[^0-9\n]
INTEGER     {SIGN}{DIGIT}+
ID      {LETTER}({LETTER}|{DIGIT})*
STRING      \"[^"\n]*\"
UNCLOSED_STRING  \"[^"\n]*\n
LINE_CMT    #[^#\n][^\n]*
WHITESPACE      [ \t\f]+

%%
"##"                    { BEGIN(BLOCK_COMMENT); }
<BLOCK_COMMENT>"##"     { BEGIN(INITIAL); }
<BLOCK_COMMENT>\n       { line_num++; }
<BLOCK_COMMENT>.        { /* consume */ }
<BLOCK_COMMENT><<EOF>>  {
                            fprintf(stdout,
                                "Error: Unterminated block comment on line %d\n", line_num);
                            Token* t = new Token();
                            t->type = T_EOF;
                            return t;
                        }


{LINE_CMT}      { /* consume to end of line */ }
"#"             { /* single lone hash, consume silently */ }
{WHITESPACE}    { /* consume silently */ }

\n              { line_num++; }

"program"   { Token* t = new Token(); t->type = T_PROGRAM;    return t; }
"var"       { Token* t = new Token(); t->type = T_VAR;        return t; }
"constant"  { Token* t = new Token(); t->type = T_CONSTANT;   return t; }
"integer"   { Token* t = new Token(); t->type = T_INTEGER_KW; return t; }
"bool"      { Token* t = new Token(); t->type = T_BOOL;       return t; }
"string"    { Token* t = new Token(); t->type = T_STRING_KW;  return t; }
"float"     { Token* t = new Token(); t->type = T_FLOAT_KW;   return t; }
"true"      { Token* t = new Token(); t->type = T_TRUE;       return t; }
"false"     { Token* t = new Token(); t->type = T_FALSE;      return t; }
"if"        { Token* t = new Token(); t->type = T_IF;         return t; }
"fi"        { Token* t = new Token(); t->type = T_FI;         return t; }
"then"      { Token* t = new Token(); t->type = T_THEN;       return t; }
"else"      { Token* t = new Token(); t->type = T_ELSE;       return t; }
"while"     { Token* t = new Token(); t->type = T_WHILE;      return t; }
"do"        { Token* t = new Token(); t->type = T_DO;         return t; }
"od"        { Token* t = new Token(); t->type = T_OD;         return t; }
"and"       { Token* t = new Token(); t->type = T_AND;        return t; }
"or"        { Token* t = new Token(); t->type = T_OR;         return t; }
"read"      { Token* t = new Token(); t->type = T_READ;       return t; }
"write"     { Token* t = new Token(); t->type = T_WRITE;      return t; }
"for"       { Token* t = new Token(); t->type = T_FOR;        return t; }
"from"      { Token* t = new Token(); t->type = T_FROM;       return t; }
"to"        { Token* t = new Token(); t->type = T_TO;         return t; }
"by"        { Token* t = new Token(); t->type = T_BY;         return t; }



{BAD_FLOAT} {
                fprintf(stdout,
                    "Error: Bad floating point number on line %d\n", line_num);
                yyless(yyleng - 1);
            }
{FLOAT} {
    Token* t = new Token();
    t->type = T_FLOAT;
    t->float_value = atof(yytext);
    return t;
    }

{INTEGER} {
    Token* t = new Token();
    t->type      = T_INTEGER;
    t->int_value = atoi(yytext);
    return t;
    }
{STRING}   {
    Token* t = new Token();
    t->type      = T_STRING;
    /* strip the surrounding quotes */
    t->str_value = std::string(yytext + 1, yyleng - 2);
    return t;
        }

{UNCLOSED_STRING} {
                fprintf(stdout,
                    "Error: Unclosed string on line %d\n", line_num);
                line_num++;   /* \n was consumed */
            }


{ID} {
                Token* t = new Token();
                t->type      = T_IDENTIFIER;
                t->str_value = yytext;
                return t;
}
":="        { Token* t = new Token(); t->type = T_ASSIGN;    return t; }
"<="        { Token* t = new Token(); t->type = T_LTE;       return t; }
">="        { Token* t = new Token(); t->type = T_GTE;       return t; }
"!="        { Token* t = new Token(); t->type = T_NOTEQUAL;  return t; }

"+"         { Token* t = new Token(); t->type = T_PLUS;      return t; }
"-"         { Token* t = new Token(); t->type = T_MINUS;     return t; }
"*"         { Token* t = new Token(); t->type = T_MULT;      return t; }
"/"         { Token* t = new Token(); t->type = T_DIV;       return t; }
"="         { Token* t = new Token(); t->type = T_EQUAL;     return t; }
"<"         { Token* t = new Token(); t->type = T_LT;        return t; }
">"         { Token* t = new Token(); t->type = T_GT;        return t; }
"("         { Token* t = new Token(); t->type = T_LPAREN;    return t; }
")"         { Token* t = new Token(); t->type = T_RPAREN;    return t; }
"["         { Token* t = new Token(); t->type = T_LBRACKET;  return t; }
"]"         { Token* t = new Token(); t->type = T_RBRACKET;  return t; }
"{"         { Token* t = new Token(); t->type = T_LBRACE;    return t; }
"}"         { Token* t = new Token(); t->type = T_RBRACE;    return t; }
";"         { Token* t = new Token(); t->type = T_SEMICOLON; return t; }
","         { Token* t = new Token(); t->type = T_COMMA;     return t; }
"."         { Token* t = new Token(); t->type = T_DOT;       return t; }
":"         { Token* t = new Token(); t->type = T_COLON;     return t; }

<<EOF>>     { Token* t = new Token(); t->type = T_EOF;       return t; }
.           {
                fprintf(stdout,
                    "Error: Illegal character '%s' on line %d\n",
                    yytext, line_num);
            }

%%
int yywrap(void) { return 1; }
