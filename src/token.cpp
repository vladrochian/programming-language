#include "token.h"

Token::Token(Token::TokenType type) : type(type) {}

Token::Token(Token::TokenType type, int intValue) : type(type), intValue(intValue) {}

Token::Token(Token::TokenType type, char op) : type(type), op(op) {}

Token::Token(Token::TokenType type, std::string stringValue) : type(type), stringValue(std::move(stringValue)) {}
