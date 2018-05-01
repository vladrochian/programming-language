#ifndef BEAUTY_LANG_TOKEN_H
#define BEAUTY_LANG_TOKEN_H

#include <string>
#include <vector>

#include "keyword.h"
#include "operator.h"

class Token {
  public:
    enum TokenType {
        TOKEN_IDENTIFIER,
        TOKEN_KEYWORD,
        TOKEN_BOOLEAN,
        TOKEN_NUMBER,
        TOKEN_STRING,
        TOKEN_OPERATOR,
        TOKEN_EOF
    };

    TokenType type;

    union {
        Operator op;
        bool booleanValue;
        double numberValue = 0.0;
        std::string stringValue;
        Keyword keyword;
    };

    explicit Token(TokenType type);
    explicit Token(Operator op);
    explicit Token(bool booleanValue);
    explicit Token(double numberValue);
    explicit Token(std::string stringValue, TokenType type = TOKEN_STRING);
    explicit Token(Keyword keyword);
};

typedef std::vector<Token>::iterator TokenIter;

#endif //BEAUTY_LANG_TOKEN_H
