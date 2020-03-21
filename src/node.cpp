#include "node.h"

Node::Type Node::getType() const { return NODE; }

BlockNode::BlockNode(std::vector<std::unique_ptr<Node>> content) : content(std::move(content)) {}

Node::Type BlockNode::getType() const { return BLOCK; }

const std::vector<std::unique_ptr<Node>>& BlockNode::getContent() const { return content; }

VariableDeclarationNode::VariableDeclarationNode(std::string name, int type, std::unique_ptr<ExpressionNode> initializer)
    : name(std::move(name)), type(type), initializer(std::move(initializer)) {}

Node::Type VariableDeclarationNode::getType() const { return VARIABLE_DECLARATION; }

std::string VariableDeclarationNode::getVariableName() const { return name; }

int VariableDeclarationNode::getVariableType() const { return type; }

const std::unique_ptr<ExpressionNode>& VariableDeclarationNode::getInitializer() const {
  return initializer;
}

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

UnaryOperatorNode::UnaryOperatorNode(UnaryOperatorNode::UnaryOperator op, std::unique_ptr<ExpressionNode> operand)
    : op(op), operand(std::move(operand)) {}

Node::Type UnaryOperatorNode::getType() const { return UNARY_OPERATOR; }

UnaryOperatorNode::UnaryOperator UnaryOperatorNode::getOperator() const { return op; }

const std::unique_ptr<ExpressionNode>& UnaryOperatorNode::getOperand() const { return operand; }

BinaryOperatorNode::BinaryOperatorNode(BinaryOperatorNode::BinaryOperator op,
                                       std::unique_ptr<ExpressionNode> leftOperand,
                                       std::unique_ptr<ExpressionNode> rightOperand)
    : op(op), leftOperand(std::move(leftOperand)), rightOperand(std::move(rightOperand)) {}

Node::Type BinaryOperatorNode::getType() const { return BINARY_OPERATOR; }

BinaryOperatorNode::BinaryOperator BinaryOperatorNode::getOperator() const { return op; }

const std::unique_ptr<ExpressionNode>& BinaryOperatorNode::getLeftOperand() const { return leftOperand; }

const std::unique_ptr<ExpressionNode>& BinaryOperatorNode::getRightOperand() const { return rightOperand; }

VariableNode::VariableNode(std::string name) : name(std::move(name)) {}

Node::Type VariableNode::getType() const { return VARIABLE; }

std::string VariableNode::getName() const { return name; }

StandaloneExpressionNode::StandaloneExpressionNode(std::unique_ptr<ExpressionNode> expression) : expression(
    std::move(expression)) {}

Node::Type StandaloneExpressionNode::getType() const { return STANDALONE_EXPRESSION; }

const std::unique_ptr<ExpressionNode>& StandaloneExpressionNode::getExpression() const { return expression; }

ReturnInstructionNode::ReturnInstructionNode(std::unique_ptr<ExpressionNode> expression) : expression(
    std::move(expression)) {}

Node::Type ReturnInstructionNode::getType() const { return RETURN_INSTRUCTION; }

const std::unique_ptr<ExpressionNode>& ReturnInstructionNode::getExpression() const { return expression; }

PrintInstructionNode::PrintInstructionNode(std::unique_ptr<ExpressionNode> expression) : expression(
    std::move(expression)) {}

Node::Type PrintInstructionNode::getType() const { return PRINT_INSTRUCTION; }

ReadInstructionNode::ReadInstructionNode(std::unique_ptr<ExpressionNode> expression) : expression(
    std::move(expression)) {}

const std::unique_ptr<ExpressionNode>& ReadInstructionNode::getExpression() const { return expression; }

Node::Type ReadInstructionNode::getType() const { return READ_INSTRUCTION; }

const std::unique_ptr<ExpressionNode>& PrintInstructionNode::getExpression() const { return expression; }

IfNode::IfNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<BlockNode> thenBlock,
               std::unique_ptr<BlockNode> elseBlock)
    : condition(std::move(condition)), thenBlock(std::move(thenBlock)), elseBlock(std::move(elseBlock)) {}

Node::Type IfNode::getType() const { return IF_STATEMENT; }

const std::unique_ptr<ExpressionNode>& IfNode::getCondition() const { return condition; }

const std::unique_ptr<BlockNode>& IfNode::getThenBlock() const { return thenBlock; }

const std::unique_ptr<BlockNode>& IfNode::getElseBlock() const { return elseBlock; }

WhileNode::WhileNode(std::unique_ptr<ExpressionNode> condition, std::unique_ptr<BlockNode> block) : condition(
    std::move(condition)), block(std::move(block)) {}

Node::Type WhileNode::getType() const { return WHILE_STATEMENT; }

const std::unique_ptr<ExpressionNode>& WhileNode::getCondition() const { return condition; }

const std::unique_ptr<BlockNode>& WhileNode::getBlock() const { return block; }

ListValueNode::ListValueNode(std::vector<std::unique_ptr<ExpressionNode>> elements) : elements(std::move(elements)) {}

Node::Type ListValueNode::getType() const {
  return LIST_VALUE;
}

const std::vector<std::unique_ptr<ExpressionNode>>& ListValueNode::getElements() const {
  return elements;
}
