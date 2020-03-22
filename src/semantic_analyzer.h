#ifndef PROG_LANG_SEMANTIC_ANALYZER_H
#define PROG_LANG_SEMANTIC_ANALYZER_H

#include "node.h"
#include "types.h"
#include "value.h"

class SemanticAnalyzer {
 public:
  static void analyze(Node* node, bool allowReturn = false, int retType = TYPE_NONE);

 private:
  static void analyzeExpr(ExpressionNode* node);
  static int getExpressionType(ExpressionNode* node);
  static Value::MemoryClass getExpressionMemoryClass(ExpressionNode* node);
  static int getResultType(UnaryOperatorNode::UnaryOperator op, int type);
  static int getResultType(BinaryOperatorNode::BinaryOperator op, int lhs, int rhs);
  static Value::MemoryClass getMemoryClass(UnaryOperatorNode::UnaryOperator op, Value::MemoryClass cls);
  static Value::MemoryClass getMemoryClass(BinaryOperatorNode::BinaryOperator op, Value::MemoryClass lhs,
                                           Value::MemoryClass rhs);
};

#endif //PROG_LANG_SEMANTIC_ANALYZER_H
