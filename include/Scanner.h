#ifndef SCANNER_H
#define SCANNER_H

#include <unordered_map>
#include <string>
#include "Token.h"
#include "FileDescriptor.h"

class Scanner {

public:

    Token* scan(FileDescriptor *fd);

private:

    void skipWhitespace(FileDescriptor *fd);
    void skipComment(FileDescriptor *fd);

    Token* scanIdentifier(FileDescriptor *fd, char first);
    Token* scanNumber(FileDescriptor *fd, char first);
    Token* scanString(FileDescriptor *fd);

    TokenType checkKeyword(const std::string& word);

    static std::unordered_map<std::string, TokenType> keywordMap;
};

#endif