#include "token.h"

Token::Token(Token::TokenType type) : type(type) {}

Token::Token(Operator op) : type(TOKEN_OPERATOR), op(op) {}

Token::Token(bool booleanValue) : type(TOKEN_BOOLEAN), booleanValue(booleanValue) {}

Token::Token(double numberValue) : type(TOKEN_NUMBER), numberValue(numberValue) {}

Token::Token(std::string stringValue, TokenType type) : type(type), stringValue(std::move(stringValue)) {}

Token::Token(Keyword keyword) : type(TOKEN_KEYWORD), keyword(keyword) {}
