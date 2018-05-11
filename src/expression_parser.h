#ifndef BEAUTY_LANG_EXPRESSION_PARSER_H
#define BEAUTY_LANG_EXPRESSION_PARSER_H

#include <string>
#include <vector>

#include "node.h"
#include "token.h"

class ExpressionParser {
  public:
    static ExpressionNode* parse(TokenIter& iter);

  private:
    static ExpressionNode* parseAssignmentLevel(TokenIter& iter);
    static ExpressionNode* parseOrLevel(TokenIter& iter);
    static ExpressionNode* parseAndLevel(TokenIter& iter);
    static ExpressionNode* parsePredicateLevel(TokenIter& iter);
    static ExpressionNode* parseAdditionLevel(TokenIter& iter);
    static ExpressionNode* parseMultiplicationLevel(TokenIter& iter);
    static ExpressionNode* parseUnaryOperatorsLevel(TokenIter& iter);
    static ExpressionNode* parseOperand(TokenIter& iter);
};

#endif //BEAUTY_LANG_EXPRESSION_PARSER_H
