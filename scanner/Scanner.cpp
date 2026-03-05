Token* Scanner::scan(FileDescriptor *fd)
{
    skipWhitespace(fd);

    char c = fd->GetChar();

    if(c == EOF)
        return new Token{T_EOF};

    if(isalpha(c) || c == '_')
        return scanIdentifier(fd, c);

    if(isdigit(c) || c == '-')
        return scanNumber(fd, c);

    if(c == '"')
        return scanString(fd);

    // operators
    switch(c)
    {
        case '+': return new Token{T_PLUS};
        case '-': return new Token{T_MINUS};
        case '*': return new Token{T_MULT};
        case '/': return new Token{T_DIV};

        case '(' : return new Token{T_LPAREN};
        case ')' : return new Token{T_RPAREN};

        case ':' :
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                return new Token{T_ASSIGN};
            }
            return new Token{T_COLON};

        case '<':
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                return new Token{T_LTE};
            }
            return new Token{T_LT};

        case '>':
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                return new Token{T_GTE};
            }
            return new Token{T_GT};

        case '=':
            return new Token{T_EQUAL};

        case '!':
            if(fd->PeekChar() == '=')
            {
                fd->GetChar();
                return new Token{T_NOTEQUAL};
            }

        default:
            fd->ReportError("Illegal character");
            return new Token{T_ERROR};
    }
}