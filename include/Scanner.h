#ifndef SCANNER_H
#define SCANNER_H

#include "token.h"
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
};

#endif