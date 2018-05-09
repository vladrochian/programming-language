#ifndef BEAUTY_LANG_NODE_H
#define BEAUTY_LANG_NODE_H

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
        OBJECT,
        VARIABLE,
        FUNCTION_CALL,
        UNARY_OPERATOR,
        BINARY_OPERATOR
    };

    virtual Type getType() const;
};

class BlockNode : public Node {
  public:
    explicit BlockNode(std::vector<Node> content);
    Type getType() const override;
    std::vector<Node> getContent() const;

  private:
    std::vector<Node> content;
};

class VariableDeclarationNode : public Node {
  public:
    VariableDeclarationNode(std::string name, PrimitiveType type);
    Type getType() const override;
    std::string getVariableName() const;
    PrimitiveType getVariableType() const;

  private:
    std::string name;
    PrimitiveType type;
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

class UnaryOperatorNode : public ExpressionNode {
  public:
    enum UnaryOperator {
        PLUS,
        MINUS,
        NOT
    };

    UnaryOperatorNode(UnaryOperator op, ExpressionNode* operand);
    Type getType() const override;
    UnaryOperator getOperator() const;
    ExpressionNode* getOperand() const;

  private:
    UnaryOperator op;
    ExpressionNode* operand;
};

class BinaryOperatorNode : public ExpressionNode {
  public:
    enum BinaryOperator {
        PLUS,
        MINUS
    };

    BinaryOperatorNode(BinaryOperator op, ExpressionNode* leftOperand, ExpressionNode* rightOperand);
    Type getType() const override;
    BinaryOperator getOperator() const;
    ExpressionNode* getLeftOperand() const;
    ExpressionNode* getRightOperand() const;

  private:
    BinaryOperator op;
    ExpressionNode* leftOperand;
    ExpressionNode* rightOperand;
};

class VariableNode : public ExpressionNode {
  public:
    explicit VariableNode(std::string name);
    Type getType() const override;
    std::string getName() const;

  private:
    std::string name;
};

#endif //BEAUTY_LANG_NODE_H
