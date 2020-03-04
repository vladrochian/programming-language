#ifndef PROG_LANG_LOGGER_H
#define PROG_LANG_LOGGER_H

#include <string>

#include "keyword.h"
#include "node.h"
#include "operator.h"
#include "token.h"

class Logger {
 public:
  static void print(const Token& token);
  static void print(Node* node, int indent = 0);

 private:
  static std::string toString(OperatorToken op);
  static std::string toString(Keyword keyword);
  static void printExpression(ExpressionNode* node);
  static void printIndent(int size);
};

#endif //PROG_LANG_LOGGER_H
