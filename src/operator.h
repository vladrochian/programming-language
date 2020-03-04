#ifndef PROG_LANG_OPERATOR_H
#define PROG_LANG_OPERATOR_H

#include <map>
#include <string>

enum OperatorToken {
    OP_DOT,
    OP_COMMA,
    OP_PLUS,
    OP_MINUS,
    OP_ASTERISK,
    OP_SLASH,
    OP_PERCENT,
    OP_NOT,
    OP_OR,
    OP_AND,
    OP_OPENING_ROUND,
    OP_CLOSING_ROUND,
    OP_OPENING_SQUARE,
    OP_CLOSING_SQUARE,
    OP_OPENING_CURLY,
    OP_CLOSING_CURLY,
    OP_EQUALS,
    OP_PLUS_EQUALS,
    OP_MINUS_EQUALS,
    OP_ASTERISK_EQUALS,
    OP_SLASH_EQUALS,
    OP_PERCENT_EQUALS,
    OP_OR_EQUALS,
    OP_AND_EQUALS,
    OP_COLON,
    OP_IS_EQUAL_TO,
    OP_IS_LESS_THAN,
    OP_IS_GREATER_THAN,
    OP_IS_LESS_EQUAL,
    OP_IS_GREATER_EQUAL,
    OP_IS_DIFFERENT
};

void initializeOperatorTokenMapping();
const std::map<std::string, OperatorToken>& operatorTokenMap();

#endif //PROG_LANG_OPERATOR_H
