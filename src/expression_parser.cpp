#include "expression_parser.h"

#include <algorithm>
#include <map>

#include "syntax_error.h"

namespace {

const std::vector<OperatorToken> ASSIGNMENT_OP_TOKENS = {
        OP_EQUALS,
        OP_PLUS_EQUALS,
        OP_MINUS_EQUALS,
        OP_ASTERISK_EQUALS,
        OP_SLASH_EQUALS,
        OP_PERCENT_EQUALS,
        OP_AND_EQUALS,
        OP_OR_EQUALS
};

const std::vector<OperatorToken> PREDICATE_TOKENS = {
        OP_IS_EQUAL_TO,
        OP_IS_LESS_THAN,
        OP_IS_GREATER_THAN,
        OP_IS_LESS_EQUAL,
        OP_IS_GREATER_EQUAL,
        OP_IS_DIFFERENT
};

const std::vector<OperatorToken> ADDITION_OP_TOKENS = {
        OP_PLUS,
        OP_MINUS
};

const std::vector<OperatorToken> MULTIPLICATION_OP_TOKENS = {
        OP_ASTERISK,
        OP_SLASH,
        OP_PERCENT
};

const std::vector<OperatorToken> UNARY_OP_TOKENS = {
        OP_PLUS,
        OP_MINUS,
        OP_NOT
};

const std::map<OperatorToken, UnaryOperatorNode::UnaryOperator> UNARY_OP_NODE = {
        {OP_PLUS, UnaryOperatorNode::PLUS},
        {OP_MINUS, UnaryOperatorNode::MINUS},
        {OP_NOT, UnaryOperatorNode::NOT}
};

const std::map<OperatorToken, BinaryOperatorNode::BinaryOperator> BINARY_OP_NODE = {
        {OP_EQUALS, BinaryOperatorNode::ASSIGN},
        {OP_PLUS_EQUALS, BinaryOperatorNode::ADD_ASSIGN},
        {OP_MINUS_EQUALS, BinaryOperatorNode::SUBTRACT_ASSIGN},
        {OP_ASTERISK_EQUALS, BinaryOperatorNode::MULTIPLY_ASSIGN},
        {OP_SLASH_EQUALS, BinaryOperatorNode::DIVIDE_ASSIGN},
        {OP_PERCENT_EQUALS, BinaryOperatorNode::REMAINDER_ASSIGN},
        {OP_OR_EQUALS, BinaryOperatorNode::OR_ASSIGN},
        {OP_AND_EQUALS, BinaryOperatorNode::AND_ASSIGN},
        {OP_IS_EQUAL_TO, BinaryOperatorNode::EQUAL},
        {OP_IS_LESS_THAN, BinaryOperatorNode::LESS},
        {OP_IS_GREATER_THAN, BinaryOperatorNode::GREATER},
        {OP_IS_LESS_EQUAL, BinaryOperatorNode::LESS_EQUAL},
        {OP_IS_GREATER_EQUAL, BinaryOperatorNode::GREATER_EQUAL},
        {OP_IS_DIFFERENT, BinaryOperatorNode::DIFFERENT},
        {OP_PLUS, BinaryOperatorNode::ADD},
        {OP_MINUS, BinaryOperatorNode::SUBTRACT},
        {OP_ASTERISK, BinaryOperatorNode::MULTIPLY},
        {OP_SLASH, BinaryOperatorNode::DIVIDE},
        {OP_PERCENT, BinaryOperatorNode::REMAINDER}
};

}

std::unique_ptr<ExpressionNode> ExpressionParser::parse(TokenIter& iter) {
    return parseAssignmentLevel(iter);
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseAssignmentLevel(TokenIter& iter) {
    auto node = parseOrLevel(iter);
    if (iter->getType() == Token::OPERATOR && isOnLevel(ASSIGNMENT_OP_TOKENS, *iter)) {
        auto op = BINARY_OP_NODE.at(iter->getOperator());
        node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseAssignmentLevel(++iter));
    }
    return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseOrLevel(TokenIter& iter) {
    auto node = parseAndLevel(iter);
    while (iter->getType() == Token::OPERATOR && iter->getOperator() == OP_OR) {
        node = std::make_unique<BinaryOperatorNode>(BinaryOperatorNode::OR, std::move(node), parseAndLevel(++iter));
    }
    return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseAndLevel(TokenIter& iter) {
    auto node = parsePredicateLevel(iter);
    while (iter->getType() == Token::OPERATOR && iter->getOperator() == OP_AND) {
        node = std::make_unique<BinaryOperatorNode>(BinaryOperatorNode::AND, std::move(node), parsePredicateLevel(++iter));
    }
    return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parsePredicateLevel(TokenIter& iter) {
    auto node = parseAdditionLevel(iter);
    while (iter->getType() == Token::OPERATOR && isOnLevel(PREDICATE_TOKENS, *iter)) {
        auto op = BINARY_OP_NODE.at(iter->getOperator());
        node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseAdditionLevel(++iter));
    }
    return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseAdditionLevel(TokenIter& iter) {
    auto node = parseMultiplicationLevel(iter);
    while (iter->getType() == Token::OPERATOR && isOnLevel(ADDITION_OP_TOKENS, *iter)) {
        auto op = BINARY_OP_NODE.at(iter->getOperator());
        node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseMultiplicationLevel(++iter));
    }
    return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseMultiplicationLevel(TokenIter& iter) {
    auto node = parseUnaryOperatorsLevel(iter);
    while (iter->getType() == Token::OPERATOR && isOnLevel(MULTIPLICATION_OP_TOKENS, *iter)) {
        auto op = BINARY_OP_NODE.at(iter->getOperator());
        node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseUnaryOperatorsLevel(++iter));
    }
    return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseUnaryOperatorsLevel(TokenIter& iter) {
    if (iter->getType() == Token::OPERATOR && isOnLevel(UNARY_OP_TOKENS, *iter)) {
        auto op = UNARY_OP_NODE.at(iter->getOperator());
        return std::make_unique<UnaryOperatorNode>(op, parseUnaryOperatorsLevel(++iter));
    }
    return parseOperand(iter);
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseOperand(TokenIter& iter) {
    auto location = iter->getLocation();
    std::unique_ptr<ExpressionNode> node;
    switch (iter->getType()) {
        case Token::BOOLEAN:
            return std::make_unique<BooleanValueNode>((iter++)->getBoolValue());
        case Token::NUMBER:
            return std::make_unique<NumberValueNode>((iter++)->getDoubleValue());
        case Token::STRING:
            return std::make_unique<StringValueNode>((iter++)->getStringValue());
        case Token::IDENTIFIER:
            // TODO: function call
            return std::make_unique<VariableNode>((iter++)->getStringValue());
        case Token::OPERATOR:
            if (iter->getOperator() != OP_OPENING_ROUND) {
                throw SyntaxError(location.first, location.second,
                                  "expected open parenthesis, unary operator or operand");
            }
            node = parseAssignmentLevel(++iter);
            if (iter->getType() != Token::OPERATOR || iter->getOperator() != OP_CLOSING_ROUND) {
                throw SyntaxError(location.first, location.second, "expected binary operator or closing parenthesis");
            }
            ++iter;
            return node;
        default:
            throw SyntaxError(location.first, location.second, "expected open parenthesis, unary operator or operand");
    }
}

bool ExpressionParser::isOnLevel(const std::vector<OperatorToken>& opList, const Token& token) {
    return std::count(opList.begin(), opList.end(), token.getOperator()) == 1;
}
