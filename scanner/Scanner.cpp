#include "Scanner.h"
#include "Token.h"
#include "FileDescriptor.h"
#include <cctype>
#include <string>

std::unordered_map<std::string, TokenType> Scanner::keywordMap =
{
    {"program", T_PROGRAM}, {"var", T_VAR}, {"constant", T_CONSTANT},
    {"integer", T_INTEGER_KW}, {"bool", T_BOOL}, {"string", T_STRING_KW},
    {"float", T_FLOAT_KW}, {"true", T_TRUE}, {"false", T_FALSE},
    {"if", T_IF}, {"fi", T_FI}, {"then", T_THEN}, {"else", T_ELSE},
    {"while", T_WHILE}, {"do", T_DO}, {"od", T_OD}, {"and", T_AND},
    {"or", T_OR}, {"read", T_READ}, {"write", T_WRITE}, {"for", T_FOR},
    {"from", T_FROM}, {"to", T_TO}, {"by", T_BY}
};

Token* Scanner::scan(FileDescriptor *fd)
{
    skipWhitespace(fd);

    if(fd->IsEOF())
        return new Token{T_EOF};
        
    char c = fd->GetChar();

    if(c == '#') {
        skipComment(fd);
        return scan(fd);
    }

    if(isalpha(c) || c == '_')
        return scanIdentifier(fd, c);

    // Numbers (integer or float)
    if(isdigit(c) || (c == '-' && isdigit(fd->PeekChar())))
        return scanNumber(fd, c);

    if(c == '"')
        return scanString(fd);


     Token *token = new Token;
    // operators
    switch(c)
    {
        case '+': token->type = T_PLUS; return token;
        case '-': token->type = T_MINUS; return token;
        case '*': token->type = T_MULT; return token;
        case '/': token->type = T_DIV; return token;

        case '(' : token->type = T_LPAREN; return token;
        case ')' : token->type = T_RPAREN; return token;

        case '.': token->type = T_DOT; return token;
        case ';': token->type = T_SEMICOLON; return token;
        case ',': token->type = T_COMMA; return token;

        case '[': token->type = T_LBRACKET; return token;
        case ']': token->type = T_RBRACKET; return token;
        case '{': token->type = T_LBRACE; return token;
        case '}': token->type = T_RBRACE; return token;

        case ':':
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                token->type = T_ASSIGN;
            }
            else
                token->type = T_COLON;
            return token;

        case '<':
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                token->type = T_LTE;
            }
            else
                token->type = T_LT;
            return token;

        case '>':
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                token->type = T_GTE;
            }
            else
                token->type = T_GT;
            return token;

        case '=':
            token->type = T_EQUAL;
            return token;

        case '!':
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                token->type = T_NOTEQUAL;
                return token;
            }
            fd->ReportError("Invalid '!'");
            token->type = T_ERROR;
            return token;

        default:
            fd->ReportError("Illegal character");
            token->type = T_ERROR;
            return token;
    }
}

void Scanner::skipWhitespace(FileDescriptor *fd)
{

   while(!fd->IsEOF())
    {
        char c = fd->PeekChar();

        if(isspace(c))
            fd->GetChar();
        else
            break;
    }
}

void Scanner::skipComment(FileDescriptor *fd)
{
   // block comment ##
    if(fd->PeekChar() == '#')
    {
        fd->GetChar(); // consume second #

        while(!fd->IsEOF())
        {
            char c = fd->GetChar();

            if(c == '#' && fd->PeekChar() == '#')
            {
                fd->GetChar(); // consume second #
                return;
            }
        }

        fd->ReportError("Unterminated block comment");
    }
    else
    {
        // single line comment
        while(!fd->IsEOF() && fd->PeekChar() != '\n')
            fd->GetChar();
    }
}

Token* Scanner::scanIdentifier(FileDescriptor *fd, char first)
{
    std::string word;
    word += first;

    while(!fd->IsEOF())
    {
        char c = fd->PeekChar();

        if(isalnum(c) || c == '_')
            word += fd->GetChar();
        else
            break;
    }

    TokenType type = checkKeyword(word);

    Token *token = new Token;
    token->type = type;

    if(type == T_IDENTIFIER)
        token->str_value = word;

    return token;
}

Token* Scanner::scanNumber(FileDescriptor* fd, char first)
{
    std::string num;
    num += first;

    bool isFloat = false;

    while(!fd->IsEOF())
    {
        char c = fd->PeekChar();

        if(isdigit(c))
        {
            num += fd->GetChar();
        }
        else if(c == '.')
        {
            if(isFloat)
            {
                // Multiple dots → ERROR
                fd->GetChar(); // consume the second dot
                fd->ReportError("Bad floating point number: multiple dots");
                return new Token{T_ERROR};
            }

            // Consume the dot
            fd->GetChar();
            isFloat = true;
            num += '.';

            // If next char is not digit → bad float (like "12.")
            if(fd->IsEOF() || !isdigit(fd->PeekChar()))
            {
                fd->ReportError("Bad floating point number");
                return new Token{T_ERROR};
            }

            // Consume digits after dot
            while(!fd->IsEOF() && isdigit(fd->PeekChar()))
                num += fd->GetChar();
        }
        else
        {
            break;
        }
    }

    // Invalid identifier check like 9abc
    char nextChar = fd->PeekChar();
    if(isalpha(nextChar) || nextChar == '_')
    {
        fd->ReportError("Invalid identifier starting with digit");
        return new Token{T_ERROR};
    }

    Token* token = new Token;

    try
    {
        if(isFloat)
        {
            token->type = T_FLOAT;
            token->float_value = std::stof(num);
        }
        else
        {
            token->type = T_INTEGER;
            token->int_value = std::stoi(num);
        }
    }
    catch(...)
    {
        fd->ReportError("Invalid numeric literal");
        token->type = T_ERROR;
    }

    return token;
}
Token* Scanner::scanString(FileDescriptor *fd)
{
    std::string str;

    while(!fd->IsEOF())
    {
        char c = fd->GetChar();

        if(c == '"')
        {
            Token *token = new Token;
            token->type = T_STRING;
            token->str_value = str;
            return token;
        }

        if(c == '\n')
        {
            fd->ReportError("Unterminated string literal");
            return new Token{T_ERROR};
        }

        str += c;
    }

    fd->ReportError("Unterminated string literal");
    return new Token{T_ERROR};
}

TokenType Scanner::checkKeyword(const std::string& word)
{
    auto it = keywordMap.find(word);

    if(it != keywordMap.end())
        return it->second;

    return T_IDENTIFIER;
}