#ifndef PROG_LANG_SEMANTIC_ANALYZER_H
#define PROG_LANG_SEMANTIC_ANALYZER_H

#include "node.h"
#include "types.h"
#include "value.h"

class SemanticAnalyzer {
 public:
  static void analyze(Node* node);

 private:
  static void analyzeExpr(ExpressionNode* node);
  static PrimitiveType getExpressionType(ExpressionNode* node);
  static Value::MemoryClass getExpressionMemoryClass(ExpressionNode* node);
  static PrimitiveType getResultType(UnaryOperatorNode::UnaryOperator op, PrimitiveType type);
  static PrimitiveType getResultType(BinaryOperatorNode::BinaryOperator op, PrimitiveType lhs, PrimitiveType rhs);
  static Value::MemoryClass getMemoryClass(UnaryOperatorNode::UnaryOperator op, Value::MemoryClass cls);
  static Value::MemoryClass getMemoryClass(BinaryOperatorNode::BinaryOperator op, Value::MemoryClass lhs,
                                           Value::MemoryClass rhs);
};

#endif //PROG_LANG_SEMANTIC_ANALYZER_H
