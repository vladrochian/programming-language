#ifndef BEAUTY_LANG_OPERATOR_PRECEDENCE_H
#define BEAUTY_LANG_OPERATOR_PRECEDENCE_H

#include <string>
#include <vector>

enum Associativity {LEFT_TO_RIGHT, RIGHT_TO_LEFT};

constexpr std::vector<Associativity> PRECEDENCE_LEVELS = {
        LEFT_TO_RIGHT, // 0 - member access, brackets, type
        RIGHT_TO_LEFT, // 1 - unary operators
        LEFT_TO_RIGHT, // 2 - multiplication, division, remainder
        LEFT_TO_RIGHT, // 3 - addition, subtraction
        LEFT_TO_RIGHT, // 4 - logical and
        LEFT_TO_RIGHT, // 5 - logical or
        RIGHT_TO_LEFT, // 6 - assignment
        LEFT_TO_RIGHT  // 7 - comma
};

#endif //BEAUTY_LANG_OPERATOR_PRECEDENCE_H
