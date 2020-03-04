#ifndef PROG_LANG_EXPRESSION_PARSER_H
#define PROG_LANG_EXPRESSION_PARSER_H

#include <memory>
#include <string>
#include <vector>

#include "node.h"
#include "token.h"

class ExpressionParser {
 public:
  static void initializeData();
  static std::unique_ptr<ExpressionNode> parse(TokenIter& iter);

 private:
  static std::unique_ptr<ExpressionNode> parseAssignmentLevel(TokenIter& iter);
  static std::unique_ptr<ExpressionNode> parseOrLevel(TokenIter& iter);
  static std::unique_ptr<ExpressionNode> parseAndLevel(TokenIter& iter);
  static std::unique_ptr<ExpressionNode> parsePredicateLevel(TokenIter& iter);
  static std::unique_ptr<ExpressionNode> parseAdditionLevel(TokenIter& iter);
  static std::unique_ptr<ExpressionNode> parseMultiplicationLevel(TokenIter& iter);
  static std::unique_ptr<ExpressionNode> parseUnaryOperatorsLevel(TokenIter& iter);
  static std::unique_ptr<ExpressionNode> parseOperand(TokenIter& iter);
  static bool isOperator(const std::shared_ptr<Token>& token);
  static bool isOnLevel(const std::vector<OperatorTokenType>& opList, const std::shared_ptr<Token>& token);
  static OperatorTokenType getOperator(const std::shared_ptr<Token>& token);
};

#endif //PROG_LANG_EXPRESSION_PARSER_H
