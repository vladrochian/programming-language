#include "node.h"

Node::Type Node::getType() const { return NODE; }

BlockNode::BlockNode(std::vector<Node> content) : content(std::move(content)) {}

Node::Type BlockNode::getType() const { return BLOCK; }

std::vector<Node> BlockNode::getContent() const { return content; }

VariableDeclarationNode::VariableDeclarationNode(std::string name, std::string type)
        : name(std::move(name)), type(std::move(type)) {}

Node::Type VariableDeclarationNode::getType() const { return VARIABLE_DECLARATION; }

std::string VariableDeclarationNode::getVariableName() const { return name; }

std::string VariableDeclarationNode::getVariableType() const { return type; }

Node::Type ExpressionNode::getType() const { return EXPRESSION; }

BooleanValueNode::BooleanValueNode(bool value) : value(value) {}

Node::Type BooleanValueNode::getType() const { return BOOLEAN_VALUE; }

bool BooleanValueNode::getValue() const { return value; }

NumberValueNode::NumberValueNode(double value) : value(value) {}

Node::Type NumberValueNode::getType() const { return NUMBER_VALUE; }

double NumberValueNode::getValue() const { return value; }

StringValueNode::StringValueNode(std::string value) : value(std::move(value)) {}

Node::Type StringValueNode::getType() const { return STRING_VALUE; }

std::string StringValueNode::getValue() const { return value; }

UnaryOperatorNode::UnaryOperatorNode(UnaryOperatorNode::UnaryOperator op, ExpressionNode* operand)
        : op(op), operand(operand) {}

Node::Type UnaryOperatorNode::getType() const { return UNARY_OPERATOR; }

UnaryOperatorNode::UnaryOperator UnaryOperatorNode::getOperator() const { return op; }

ExpressionNode* UnaryOperatorNode::getOperand() const { return operand; }

BinaryOperatorNode::BinaryOperatorNode(BinaryOperatorNode::BinaryOperator op, ExpressionNode* leftOperand,
                                       ExpressionNode* rightOperand)
        : op(op), leftOperand(leftOperand), rightOperand(rightOperand) {}

Node::Type BinaryOperatorNode::getType() const { return BINARY_OPERATOR; }

BinaryOperatorNode::BinaryOperator BinaryOperatorNode::getOperator() const { return op; }

ExpressionNode* BinaryOperatorNode::getLeftOperand() const { return leftOperand; }

ExpressionNode* BinaryOperatorNode::getRightOperand() const { return rightOperand; }

VariableNode::VariableNode(std::string name) : name(std::move(name)) {}

Node::Type VariableNode::getType() const { return VARIABLE; }

std::string VariableNode::getName() const { return name; }
