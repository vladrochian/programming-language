#ifndef BEAUTY_LANG_OPERATOR_H
#define BEAUTY_LANG_OPERATOR_H

#include <map>
#include <string>

enum Operator {
    OP_DOT,
    OP_PLUS,
    OP_MINUS,
    OP_ASTERISK,
    OP_SLASH,
    OP_PERCENT,
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
    OP_AND_EQUALS
};

constexpr std::map<std::string, Operator> GET_OPERATOR = {
        {"."s, OP_DOT},
        {"+"s, OP_PLUS},
        {"-"s, OP_MINUS},
        {"*"s, OP_ASTERISK},
        {"/"s, OP_SLASH},
        {"%"s, OP_PERCENT},
        {"|"s, OP_OR},
        {"&"s, OP_AND},
        {"("s, OP_OPENING_ROUND},
        {")"s, OP_CLOSING_ROUND},
        {"["s, OP_OPENING_SQUARE},
        {"]"s, OP_CLOSING_SQUARE},
        {"{"s, OP_OPENING_CURLY},
        {"}"s, OP_CLOSING_CURLY},
        {"="s, OP_EQUALS},
        {"+="s, OP_PLUS_EQUALS},
        {"-="s, OP_MINUS_EQUALS},
        {"*="s, OP_ASTERISK_EQUALS},
        {"/="s, OP_SLASH_EQUALS},
        {"%="s, OP_PERCENT_EQUALS},
        {"|="s, OP_OR_EQUALS},
        {"&="s, OP_AND_EQUALS}
};

#endif //BEAUTY_LANG_OPERATOR_H
