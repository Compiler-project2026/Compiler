# Compiler Construction — PA#1 Step by Step Guide

---

## Step 1 — fd.h
Define the FileDescriptor class interface.

```cpp
#define SET 1
#define UNSET 0
#define BUFFER_SIZE 256

class FileDescriptor {
private:
    FILE *fp;
    int line_number;
    int char_number;
    int flag;
    int buf_size;
    char *buffer;
    char *file;

public:
    FileDescriptor(const char *FileName);
    FileDescriptor();
    ~FileDescriptor();
    char *GetFileName();
    bool IsOpen();
    char *GetCurrLine();
    int GetLineNum();
    int GetCharNum();
    void Close();
    char GetChar();
    void ReportError(char *msg);
    void UngetChar(char c);
};
```

---

## Step 2 — fd.cpp
Implement all functions from fd.h.

Key rules:
- Open file in constructor, close in destructor
- `GetChar()` reads character by character, loads new line when needed
- If line buffer is too small, keep doubling it until it fits
- `UngetChar()` puts one character back, cannot do two in a row
- `ReportError()` must print in this exact format:

```
A:= A + 1
      ^
Error: "message" on line 5 of file.txt
```

---

## Step 3 — Test fd.cpp
Write a small `main.cpp` to verify the file descriptor works:

```cpp
int main() {
    FileDescriptor fd("test.txt");
    char c;
    while ((c = fd.GetChar()) != EOF)
        cout << c;
    return 0;
}
```

---

## Step 4 — scanner.h
Define the Scanner class, TOKEN struct, and LEXEME_TYPE enum.

```cpp
#define LETTER_CHAR    1
#define NUMERIC_DIGIT  2
#define SEPARATOR      3
#define OPERATOR       4
#define SPECIAL_CHAR   5
#define COMMENT_MARKER 6

typedef enum {
    lx_identifier, lx_integer, lx_string, lx_float,
    kw_program, kw_var, kw_constant, kw_integer,
    kw_bool, kw_string, kw_float, kw_true, kw_false,
    kw_if, kw_fi, kw_then, kw_else, kw_while, kw_do,
    kw_od, kw_and, kw_or, kw_read, kw_write, kw_for,
    kw_from, kw_to, kw_by, kw_function, kw_procedure,
    kw_return, kw_not, kw_begin, kw_end,
    lx_lparen, lx_rparen, lx_lbracket, lx_rbracket,
    lx_lsbracket, lx_rsbracket, lx_colon, lx_dot,
    lx_semicolon, lx_comma, lx_colon_eq,
    lx_plus, lx_minus, lx_star, lx_slash,
    lx_eq, lx_neq, lx_lt, lx_le, lx_gt, lx_ge,
    lx_eof, illegal_token
} LEXEME_TYPE;

class TOKEN {
public:
    LEXEME_TYPE type;
    int value;
    float float_value;
    char *str_ptr;
    TOKEN();
    ~TOKEN();
};

class Scanner {
public:
    FileDescriptor *fd;
    Scanner(FileDescriptor *fd);
    ~Scanner();
    TOKEN* Scan();
    TOKEN* getId(char c);
    TOKEN* getString(char c);
    TOKEN* getInt(char c);
    TOKEN* getOperator(char c);
    int    checkKeyword(char *key);
    void   skipComments(char &c);
    void   skipSpaces(char &c);
    int    getClass(char c);
};
```

---

## Step 5 — scanner.cpp
Implement all scanner functions.

### getClass()
Classifies a single character:
```
letter      → LETTER_CHAR
digit       → NUMERIC_DIGIT
whitespace  → SEPARATOR
operator    → OPERATOR
underscore  → SPECIAL_CHAR
#           → COMMENT_MARKER
```

### Scan()
Main function — reads one character and decides what to do:
```
whitespace  → skipSpaces()
#           → skipComments()
letter or _ → getId()
digit       → getInt()
"           → getString()
operator    → getOperator()
EOF         → return lx_eof token
```

### getId()
Implements RE: `[a-zA-Z_][a-zA-Z0-9_]*`
```
keep reading while: letter OR digit OR underscore
stop when: anything else → UngetChar()
then check if it is a keyword → return keyword token or identifier token
```

### getInt()
Implements RE: `-?[0-9]+` and `-?[0-9]+\.[0-9]+`
```
keep reading while: digit
if next char is '.' → keep reading digits → return lx_float
otherwise           → return lx_integer
```

### getString()
Implements RE: `"[^"\n]*"`
```
keep reading until: closing " found  → return lx_string
if newline or EOF reached first      → ReportError (unterminated string)
```

### getOperator()
Handles single and double character operators:
```
+   → lx_plus
-   → lx_minus
*   → lx_star
/   → lx_slash
=   → lx_eq
:=  → lx_colon_eq   (peek next char)
!=  → lx_neq        (peek next char)
<=  → lx_le         (peek next char)
>=  → lx_ge         (peek next char)
<   → lx_lt
>   → lx_gt
(   → lx_lparen
)   → lx_rparen
... and so on
```

### skipComments()
```
## ... ##   → block comment, skip everything until closing ##
# ...       → line comment, skip until end of line
```

---

## Step 6 — Test the Scanner
Write a `main.cpp` that calls `Scan()` in a loop:

```cpp
int main() {
    FileDescriptor fd("test.txt");
    Scanner scanner(&fd);
    TOKEN *t;

    while ((t = scanner.Scan())->type != lx_eof) {
        cout << "Token type: " << t->type;
        if (t->str_ptr && t->str_ptr[0] != '\0')
            cout << "  Value: " << t->str_ptr;
        cout << endl;
        delete t;
    }
    return 0;
}
```

Test with a file like:
```
program test
var x : integer
x := 123 + y
## this is a block comment ##
# this is a line comment
"hello world"
```

---

## Step 7 — Task 1: Write the Regular Expressions
Write these in your report document:

```
identifier  →  [a-zA-Z_][a-zA-Z0-9_]*
integer     →  -?[0-9]+
float       →  -?[0-9]+\.[0-9]+
string      →  "[^"\n]*"
block comment → ##[^#]*##
line comment  → #[^\n]*
whitespace  →  [ \t\n\f]+
operators   →  := | <= | >= | != | + | - | * | / | = | < | > | ( ) [ ] { } ; , .
```

---

## Step 8 — Task 4: Write the .l File
Create `scanner.l`:

```c
%{
#include <stdio.h>
%}

%%

[a-zA-Z_][a-zA-Z0-9_]*     printf("IDENTIFIER: %s\n", yytext);
-?[0-9]+\.[0-9]+            printf("FLOAT: %s\n", yytext);
-?[0-9]+                    printf("INTEGER: %s\n", yytext);
\"[^"\n]*\"                 printf("STRING: %s\n", yytext);
##[^#]*##                   /* skip block comment */;
#[^\n]*                     /* skip line comment */;
":="                        printf("ASSIGN\n");
"<="                        printf("LE\n");
">="                        printf("GE\n");
"!="                        printf("NEQ\n");
[ \t\n\f]+                  /* skip whitespace */;
.                           printf("UNKNOWN: %s\n", yytext);

%%

int yywrap(void) { return 1; }

int main(int argc, char **argv) {
    ++argv, --argc;
    if (argc > 0)
        yyin = fopen(argv[0], "r");
    else
        yyin = stdin;
    return yylex();
}
```

Then compile and run:
```bash
flex scanner.l
gcc lex.yy.c -o scanner
./scanner test.txt
```

---

## Step 9 — Task 4: Write the Report
Cover these points:

1. **What is lex/flex** — a tool that automatically generates a scanner from regular expressions
2. **How the .l file is structured** — three sections separated by `%%`
   - Section 1: includes and definitions
   - Section 2: RE rules and actions
   - Section 3: helper functions
3. **How flex works** — takes your REs and generates a state machine in C code
4. **Comparison to hand-written scanner**:
   - Lex: write REs → flex generates the code
   - Hand-written: you implement the REs yourself as if/while logic

---

## Final Project Structure

```
project/
├── include/
│   ├── fd.h
│   └── scanner.h
├── src/
│   ├── fd.cpp
│   ├── scanner.cpp
│   └── main.cpp
├── scanner.l
├── test.txt
└── report.pdf
```

---

## Build Commands

```bash
# Compile your hand-written scanner
g++ src/fd.cpp src/scanner.cpp src/main.cpp -o compiler
./compiler test.txt

# Compile the lex version
flex scanner.l
gcc lex.yy.c -o lex_scanner
./lex_scanner test.txt
```
