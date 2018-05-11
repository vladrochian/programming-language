#include "expression_parser.h"

#include <algorithm>
#include <vector>

namespace {

const std::vector<OperatorToken> assignmentOperators = {
        OP_EQUALS,
        OP_PLUS_EQUALS,
        OP_MINUS_EQUALS,
        OP_ASTERISK_EQUALS,
        OP_SLASH_EQUALS,
        OP_PERCENT_EQUALS,
        OP_AND_EQUALS,
        OP_OR_EQUALS
};

const std::vector<OperatorToken> predicates = {
        OP_IS_EQUAL_TO,
        OP_IS_LESS_THAN,
        OP_IS_GREATER_THAN,
        OP_IS_LESS_EQUAL,
        OP_IS_GREATER_EQUAL,
        OP_IS_DIFFERENT
};

const std::vector<OperatorToken> additionOperators = {
        OP_PLUS,
        OP_MINUS
};

const std::vector<OperatorToken> multiplicationOperators = {
        OP_ASTERISK,
        OP_SLASH,
        OP_PERCENT
};

}

ExpressionNode* ExpressionParser::parse(TokenIter& iter) {
    // TODO: implement this
    while (iter->getType() != Token::LINE_FEED)
        ++iter;
    return new ExpressionNode;
}

//ExpressionNode* ExpressionParser::parseAssignmentLevel(TokenIter& iter) {
//    auto node = parseOrLevel(iter);
//    while (iter->getType() == Token::OPERATOR &&
//           std::count(assignmentOperators.begin(), assignmentOperators.end(), iter->getOperator())) {
//        break;
//    }
//}
