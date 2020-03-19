#include "expression_parser.h"

#include <algorithm>
#include <map>

#include "syntax_error.h"

namespace {
typedef std::vector<OperatorTokenType> TokenVector;
std::unique_ptr<TokenVector> assignmentOpTokens;
std::unique_ptr<TokenVector> predicateTokens;
std::unique_ptr<TokenVector> additionOpTokens;
std::unique_ptr<TokenVector> multiplicationOpTokens;
std::unique_ptr<TokenVector> unaryOpTokens;

void initializeTokenVector(std::unique_ptr<TokenVector>& v, std::initializer_list<OperatorTokenType> init) {
  v = std::make_unique<TokenVector>(init);
}

typedef std::map<OperatorTokenType, UnaryOperatorNode::UnaryOperator> UnaryOpMap;
std::unique_ptr<UnaryOpMap> unaryOpNodeMap;

typedef std::map<OperatorTokenType, BinaryOperatorNode::BinaryOperator> BinaryOpMap;
std::unique_ptr<BinaryOpMap> binaryOpNodeMap;
}

void ExpressionParser::initializeData() {
  initializeTokenVector(assignmentOpTokens, {
      OP_EQUALS,
      OP_PLUS_EQUALS,
      OP_MINUS_EQUALS,
      OP_ASTERISK_EQUALS,
      OP_SLASH_EQUALS,
      OP_PERCENT_EQUALS,
      OP_AND_EQUALS,
      OP_OR_EQUALS
  });

  initializeTokenVector(predicateTokens, {
      OP_IS_EQUAL_TO,
      OP_IS_LESS_THAN,
      OP_IS_GREATER_THAN,
      OP_IS_LESS_EQUAL,
      OP_IS_GREATER_EQUAL,
      OP_IS_DIFFERENT
  });

  initializeTokenVector(additionOpTokens, {
      OP_PLUS,
      OP_MINUS
  });

  initializeTokenVector(multiplicationOpTokens, {
      OP_ASTERISK,
      OP_SLASH,
      OP_PERCENT
  });

  initializeTokenVector(unaryOpTokens, {
      OP_PLUS,
      OP_MINUS,
      OP_NOT
  });

  UnaryOpMap unary{
      {OP_PLUS, UnaryOperatorNode::PLUS},
      {OP_MINUS, UnaryOperatorNode::MINUS},
      {OP_NOT, UnaryOperatorNode::NOT}
  };
  unaryOpNodeMap = std::make_unique<UnaryOpMap>(std::move(unary));

  BinaryOpMap binary{
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
  binaryOpNodeMap = std::make_unique<BinaryOpMap>(std::move(binary));
}

std::unique_ptr<ExpressionNode> ExpressionParser::parse(TokenIter& iter) {
  return parseAssignmentLevel(iter);
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseAssignmentLevel(TokenIter& iter) {
  auto node = parseOrLevel(iter);
  if (isOperator(*iter) && isOnLevel(*assignmentOpTokens, *iter)) {
    auto op = binaryOpNodeMap->at(std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator());
    node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseAssignmentLevel(++iter));
  }
  return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseOrLevel(TokenIter& iter) {
  auto node = parseAndLevel(iter);
  while (isOperator(*iter) && getOperator(*iter) == OP_OR) {
    node = std::make_unique<BinaryOperatorNode>(BinaryOperatorNode::OR, std::move(node), parseAndLevel(++iter));
  }
  return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseAndLevel(TokenIter& iter) {
  auto node = parsePredicateLevel(iter);
  while (isOperator(*iter) && getOperator(*iter) == OP_AND) {
    node = std::make_unique<BinaryOperatorNode>(BinaryOperatorNode::AND, std::move(node), parsePredicateLevel(++iter));
  }
  return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parsePredicateLevel(TokenIter& iter) {
  auto node = parseAdditionLevel(iter);
  while (isOperator(*iter) && isOnLevel(*predicateTokens, *iter)) {
    auto op = binaryOpNodeMap->at(getOperator(*iter));
    node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseAdditionLevel(++iter));
  }
  return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseAdditionLevel(TokenIter& iter) {
  auto node = parseMultiplicationLevel(iter);
  while (isOperator(*iter) && isOnLevel(*additionOpTokens, *iter)) {
    auto op = binaryOpNodeMap->at(getOperator(*iter));
    node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseMultiplicationLevel(++iter));
  }
  return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseMultiplicationLevel(TokenIter& iter) {
  auto node = parseUnaryOperatorsLevel(iter);
  while (isOperator(*iter) && isOnLevel(*multiplicationOpTokens, *iter)) {
    auto op = binaryOpNodeMap->at(getOperator(*iter));
    node = std::make_unique<BinaryOperatorNode>(op, std::move(node), parseUnaryOperatorsLevel(++iter));
  }
  return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseUnaryOperatorsLevel(TokenIter& iter) {
  if (isOperator(*iter) && isOnLevel(*unaryOpTokens, *iter)) {
    auto op = unaryOpNodeMap->at(getOperator(*iter));
    return std::make_unique<UnaryOperatorNode>(op, parseUnaryOperatorsLevel(++iter));
  }
  return parseIndexOperatorLevel(iter);
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseIndexOperatorLevel(TokenIter& iter) {
  auto node = parseOperand(iter);
  while (isOperator(*iter) && getOperator(*iter) == OP_OPENING_SQUARE) {
    ++iter;
    node = std::make_unique<BinaryOperatorNode>(BinaryOperatorNode::INDEX, std::move(node), parseAssignmentLevel(iter));
    if (!isOperator(*iter) || getOperator(*iter) != OP_CLOSING_SQUARE) {
      auto location = (*iter)->getLocation();
      throw SyntaxError(location.first, location.second, "expected closing square bracket");
    }
    ++iter;
  }
  return node;
}

std::unique_ptr<ExpressionNode> ExpressionParser::parseOperand(TokenIter& iter) {
  auto location = (*iter)->getLocation();
  std::unique_ptr<ExpressionNode> node;
  switch ((*iter)->getType()) {
    case Token::BOOLEAN:
      return std::make_unique<BooleanValueNode>(std::dynamic_pointer_cast<BooleanToken>(*(iter++))->getValue());
    case Token::NUMBER:
      return std::make_unique<NumberValueNode>(std::dynamic_pointer_cast<NumberToken>(*(iter++))->getValue());
    case Token::STRING:
      return std::make_unique<StringValueNode>(std::dynamic_pointer_cast<StringToken>(*(iter++))->getValue());
    case Token::IDENTIFIER:
      // TODO: function call
      return std::make_unique<VariableNode>(std::dynamic_pointer_cast<IdentifierToken>(*(iter++))->getName());
    case Token::OPERATOR:
      if (getOperator(*iter) != OP_OPENING_ROUND) {
        throw SyntaxError(location.first, location.second,
            "expected open parenthesis, unary operator or operand");
      }
      node = parseAssignmentLevel(++iter);
      if (!isOperator(*iter) || getOperator(*iter) != OP_CLOSING_ROUND) {
        throw SyntaxError(location.first, location.second, "expected binary operator or closing parenthesis");
      }
      ++iter;
      return node;
    default:
      throw SyntaxError(location.first, location.second, "expected open parenthesis, unary operator or operand");
  }
}

bool ExpressionParser::isOperator(const std::shared_ptr<Token>& token) {
  return token->getType() == Token::OPERATOR;
}

bool ExpressionParser::isOnLevel(const std::vector<OperatorTokenType>& opList, const std::shared_ptr<Token>& token) {
  return std::count(opList.begin(), opList.end(), getOperator(token)) == 1;
}

OperatorTokenType ExpressionParser::getOperator(const std::shared_ptr<Token>& token) {
  return std::dynamic_pointer_cast<OperatorToken>(token)->getOperator();
}
