#ifndef PROG_LANG_NODE_H
#define PROG_LANG_NODE_H

#include <memory>
#include <string>
#include <vector>

#include "types.h"

class Node {
 public:
  enum Type {
    NODE,
    BLOCK,
    VARIABLE_DECLARATION,
    FUNCTION_DEFINITION,
    EXPRESSION,
    BOOLEAN_VALUE,
    NUMBER_VALUE,
    STRING_VALUE,
    LIST_VALUE,
    OBJECT,
    VARIABLE,
    FUNCTION_CALL,
    UNARY_OPERATOR,
    BINARY_OPERATOR,
    STANDALONE_EXPRESSION,
    RETURN_INSTRUCTION,
    PRINT_INSTRUCTION,
    READ_INSTRUCTION,
    IF_STATEMENT,
    WHILE_STATEMENT,
    FOR_STATEMENT
  };

  virtual ~Node() = default;
  virtual Type getType() const;
};

class BlockNode : public Node {
 public:
  explicit BlockNode(std::vector<std::unique_ptr<Node>> content);
  Type getType() const override;
  const std::vector<std::unique_ptr<Node>>& getContent() const;

 private:
  std::vector<std::unique_ptr<Node>> content;
};

class ExpressionNode : public Node {
 public:
  Type getType() const override;
};

class BooleanValueNode : public ExpressionNode {
 public:
  explicit BooleanValueNode(bool value);
  Type getType() const override;
  bool getValue() const;

 private:
  bool value;
};

class NumberValueNode : public ExpressionNode {
 public:
  explicit NumberValueNode(double value);
  Type getType() const override;
  double getValue() const;

 private:
  double value;
};

class StringValueNode : public ExpressionNode {
 public:
  explicit StringValueNode(std::string value);
  Type getType() const override;
  std::string getValue() const;

 private:
  std::string value;
};

class ListValueNode : public ExpressionNode {
 public:
  explicit ListValueNode(std::vector<std::unique_ptr<ExpressionNode>> elements);
  Type getType() const override;
  const std::vector<std::unique_ptr<ExpressionNode>>& getElements() const;

 private:
  std::vector<std::unique_ptr<ExpressionNode>> elements;
};

class UnaryOperatorNode : public ExpressionNode {
 public:
  enum UnaryOperator {
    PLUS,
    MINUS,
    NOT
  };

  UnaryOperatorNode(UnaryOperator op, std::unique_ptr<ExpressionNode> operand);
  Type getType() const override;
  UnaryOperator getOperator() const;
  const std::unique_ptr<ExpressionNode>& getOperand() const;

 private:
  UnaryOperator op;
  std::unique_ptr<ExpressionNode> operand;
};

class BinaryOperatorNode : public ExpressionNode {
 public:
  enum BinaryOperator {
    ADD,
    SUBTRACT,
    MULTIPLY,
    DIVIDE,
    REMAINDER,
    OR,
    AND,
    ASSIGN,
    ADD_ASSIGN,
    SUBTRACT_ASSIGN,
    MULTIPLY_ASSIGN,
    DIVIDE_ASSIGN,
    REMAINDER_ASSIGN,
    OR_ASSIGN,
    AND_ASSIGN,
    EQUAL,
    DIFFERENT,
    LESS,
    GREATER,
    LESS_EQUAL,
    GREATER_EQUAL,
    INDEX
  };

  BinaryOperatorNode(BinaryOperator op, std::unique_ptr<ExpressionNode> leftOperand,
                     std::unique_ptr<ExpressionNode> rightOperand);
  Type getType() const override;
  BinaryOperator getOperator() const;
  const std::unique_ptr<ExpressionNode>& getLeftOperand() const;
  const std::unique_ptr<ExpressionNode>& getRightOperand() const;

 private:
  BinaryOperator op;
  std::unique_ptr<ExpressionNode> leftOperand;
  std::unique_ptr<ExpressionNode> rightOperand;
};

class VariableNode : public ExpressionNode {
 public:
  explicit VariableNode(std::string name);
  Type getType() const override;
  std::string getName() const;

 private:
  std::string name;
};

class StandaloneExpressionNode : public Node {
 public:
  explicit StandaloneExpressionNode(std::unique_ptr<ExpressionNode> expression);
  Type getType() const override;
  const std::unique_ptr<ExpressionNode>& getExpression() const;

 private:
  std::unique_ptr<ExpressionNode> expression;
};

class VariableDeclarationNode : public Node {
 public:
  VariableDeclarationNode(std::string name, int type, std::unique_ptr<ExpressionNode> initializer);
  Type getType() const override;
  std::string getVariableName() const;
  int getVariableType() const;
  const std::unique_ptr<ExpressionNode>& getInitializer() const;

 private:
  std::string name;
  int type;
  std::unique_ptr<ExpressionNode> initializer;
};

class ReturnInstructionNode : public Node {
 public:
  explicit ReturnInstructionNode(std::unique_ptr<ExpressionNode> expression);
  Type getType() const override;
  const std::unique_ptr<ExpressionNode>& getExpression() const;

 private:
  std::unique_ptr<ExpressionNode> expression;
};

class PrintInstructionNode : public Node {
 public:
  explicit PrintInstructionNode(std::unique_ptr<ExpressionNode> expression);
  Type getType() const override;
  const std::unique_ptr<ExpressionNode>& getExpression() const;

 private:
  std::unique_ptr<ExpressionNode> expression;
};

class ReadInstructionNode : public Node {
 public:
  explicit ReadInstructionNode(std::unique_ptr<ExpressionNode> expression);
  Type getType() const override;
  const std::unique_ptr<ExpressionNode>& getExpression() const;

 private:
  std::unique_ptr<ExpressionNode> expression;
};

class IfNode : public Node {
 public:
  IfNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<BlockNode> thenBlock,
         std::unique_ptr<BlockNode> elseBlock = nullptr);
  Type getType() const override;
  const std::unique_ptr<ExpressionNode>& getCondition() const;
  const std::unique_ptr<BlockNode>& getThenBlock() const;
  const std::unique_ptr<BlockNode>& getElseBlock() const;

 private:
  std::unique_ptr<ExpressionNode> condition;
  std::unique_ptr<BlockNode> thenBlock;
  std::unique_ptr<BlockNode> elseBlock;
};

class WhileNode : public Node {
 public:
  WhileNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<BlockNode> block);
  Type getType() const override;
  const std::unique_ptr<ExpressionNode>& getCondition() const;
  const std::unique_ptr<BlockNode>& getBlock() const;

 private:
  std::unique_ptr<ExpressionNode> condition;
  std::unique_ptr<BlockNode> block;
};

class ForNode : public Node {
 public:
  ForNode(std::string it, std::unique_ptr<ExpressionNode> range, std::unique_ptr<BlockNode> block);
  Type getType() const override;
  std::string getIterName() const;
  const std::unique_ptr<ExpressionNode>& getRangeExpression() const;
  const std::unique_ptr<BlockNode>& getBlock() const;

 private:
  std::string it;
  std::unique_ptr<ExpressionNode> range;
  std::unique_ptr<BlockNode> block;
};

#endif //PROG_LANG_NODE_H
