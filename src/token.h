#ifndef BEAUTY_LANG_TOKEN_H
#define BEAUTY_LANG_TOKEN_H

#include <string>

class Token {
  public:
    enum TokenType {
        TOKEN_ID,
        TOKEN_CT_INT,
        TOKEN_OPERATOR,

        TOKEN_SEMICOLON,
        TOKEN_COLON,
        TOKEN_INT,
        TOKEN_FUN,
        TOKEN_EOF
    };

    TokenType type;

    union {
        int intValue;
        char op;
        std::string stringValue;
    };

    Token(TokenType type);
    Token(TokenType type, int intValue);
    Token(TokenType type, char op);
    Token(TokenType type, std::string stringValue);
};

#endif //BEAUTY_LANG_TOKEN_H
