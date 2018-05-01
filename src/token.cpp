#include "token.h"

Token::Token(int line, int col, Token::Type type) : location(line, col), type(type) {}

Token::Token(int line, int col, Token::Type type, bool value) : location(line, col), type(type), boolValue(value) {}

Token::Token(int line, int col, Token::Type type, int value) : location(line, col), type(type), intValue(value) {}

Token::Token(int line, int col, Token::Type type, double value) : location(line, col), type(type), doubleValue(value) {}

Token::Token(int line, int col, Token::Type type, std::string value)
        : location(line, col), type(type), stringValue(std::move(value)) {}

Token::Token(int line, int col, Token::Type type, Operator value)
        : location(line, col), type(type), operatorValue(value) {}

Token::Token(int line, int col, Token::Type type, Keyword value)
        : location(line, col), type(type), keywordValue(value) {}

Token::Type Token::getType() const { return type; }

std::pair<int, int> Token::getLocation() const { return location; }

Operator Token::getOperator() const { return operatorValue; }

bool Token::getBoolValue() const { return boolValue; }

int Token::getIntValue() const { return intValue; }

double Token::getDoubleValue() const { return doubleValue; }

std::string Token::getStringValue() const { return stringValue; }

Keyword Token::getKeyword() const { return keywordValue; }
