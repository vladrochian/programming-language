#include "semantic_analyzer.h"

#include "semantic_error.h"
#include "store.h"

void SemanticAnalyzer::analyze(Node* node) {
    if (node->getType() == Node::BLOCK) {
        auto blockNode = dynamic_cast<BlockNode*>(node);
        store.newLevel();
        for (auto it : blockNode->getContent()) {
            analyze(it);
        }
        store.deleteLevel();
    } else if (node->getType() == Node::STANDALONE_EXPRESSION) {
        analyzeExpr(dynamic_cast<StandaloneExpressionNode*>(node)->getExpression());
    } else if (node->getType() == Node::RETURN_INSTRUCTION) {
        analyzeExpr(dynamic_cast<ReturnInstructionNode*>(node)->getExpression());
    } else if (node->getType() == Node::PRINT_INSTRUCTION) {
        analyzeExpr(dynamic_cast<PrintInstructionNode*>(node)->getExpression());
    } else if (node->getType() == Node::VARIABLE_DECLARATION) {
        auto varDecNode = dynamic_cast<VariableDeclarationNode*>(node);
        store.registerName(varDecNode->getVariableName(), new VariableData(varDecNode->getVariableType()));
    } else if (node->getType() == Node::IF_STATEMENT) {
        auto ifNode = dynamic_cast<IfNode*>(node);
        analyzeExpr(ifNode->getCondition());
        if (getExpressionType(ifNode->getCondition()) != TYPE_BOOLEAN) {
            throw SemanticError("if condition should have boolean type");
        }
        analyze(ifNode->getThenBlock());
        if (ifNode->getElseBlock() != nullptr) {
            analyze(ifNode->getElseBlock());
        }
    } else if (node->getType() == Node::WHILE_STATEMENT) {
        auto whileNode = dynamic_cast<WhileNode*>(node);
        analyzeExpr(whileNode->getCondition());
        if (getExpressionType(whileNode->getCondition()) != TYPE_BOOLEAN) {
            throw SemanticError("while condition should have boolean type");
        }
        analyze(whileNode->getBlock());
    }
}

void SemanticAnalyzer::analyzeExpr(ExpressionNode* node) {
    getExpressionType(node);
    getExpressionMemoryClass(node);
}

PrimitiveType SemanticAnalyzer::getExpressionType(ExpressionNode* node) {
    auto unOpNode = dynamic_cast<UnaryOperatorNode*>(node);
    auto binOpNode = dynamic_cast<BinaryOperatorNode*>(node);
    switch (node->getType()) {
        case Node::BOOLEAN_VALUE:
            return TYPE_BOOLEAN;
        case Node::NUMBER_VALUE:
            return TYPE_NUMBER;
        case Node::STRING_VALUE:
            return TYPE_STRING;
        case Node::UNARY_OPERATOR:
            return getResultType(unOpNode->getOperator(), getExpressionType(unOpNode->getOperand()));
        case Node::BINARY_OPERATOR:
            return getResultType(binOpNode->getOperator(), getExpressionType(binOpNode->getLeftOperand()),
                                 getExpressionType(binOpNode->getRightOperand()));
        case Node::VARIABLE:
            return Lvalue(dynamic_cast<VariableNode*>(node)->getName()).getType();
        default:
            break;
    }
}

Value::MemoryClass SemanticAnalyzer::getExpressionMemoryClass(ExpressionNode* node) {
    auto unOpNode = dynamic_cast<UnaryOperatorNode*>(node);
    auto binOpNode = dynamic_cast<BinaryOperatorNode*>(node);
    switch (node->getType()) {
        case Node::BOOLEAN_VALUE:
        case Node::NUMBER_VALUE:
        case Node::STRING_VALUE:
            return Value::RVALUE;
        case Node::UNARY_OPERATOR:
            return getMemoryClass(unOpNode->getOperator(), getExpressionMemoryClass(unOpNode->getOperand()));
        case Node::BINARY_OPERATOR:
            return getMemoryClass(binOpNode->getOperator(), getExpressionMemoryClass(binOpNode->getLeftOperand()),
                                  getExpressionMemoryClass(binOpNode->getRightOperand()));
        case Node::VARIABLE:
            return Value::LVALUE;
        default:
            break;
    }
}

PrimitiveType SemanticAnalyzer::getResultType(UnaryOperatorNode::UnaryOperator op, PrimitiveType type) {
    if (op == UnaryOperatorNode::NOT && type == TYPE_BOOLEAN ||
        op == UnaryOperatorNode::PLUS && type == TYPE_NUMBER ||
        op == UnaryOperatorNode::MINUS && type == TYPE_NUMBER) {
        return type;
    }
    throw SemanticError("invalid operand");
}

PrimitiveType SemanticAnalyzer::getResultType(BinaryOperatorNode::BinaryOperator op, PrimitiveType lhs,
                                              PrimitiveType rhs) {
    switch (op) {
        case BinaryOperatorNode::ADD:
            if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
            if (lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_STRING;
            break;
        case BinaryOperatorNode::SUBTRACT:
        case BinaryOperatorNode::MULTIPLY:
        case BinaryOperatorNode::DIVIDE:
        case BinaryOperatorNode::REMAINDER:
            if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
            break;
        case BinaryOperatorNode::OR:
        case BinaryOperatorNode::AND:
            if (lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN) return TYPE_BOOLEAN;
            break;
        case BinaryOperatorNode::ASSIGN:
            if (lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN) return TYPE_BOOLEAN;
            if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
            if (lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_STRING;
            break;
        case BinaryOperatorNode::ADD_ASSIGN:
            if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
            if (lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_STRING;
            break;
        case BinaryOperatorNode::SUBTRACT_ASSIGN:
        case BinaryOperatorNode::MULTIPLY_ASSIGN:
        case BinaryOperatorNode::DIVIDE_ASSIGN:
        case BinaryOperatorNode::REMAINDER_ASSIGN:
            if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER) return TYPE_NUMBER;
            break;
        case BinaryOperatorNode::OR_ASSIGN:
        case BinaryOperatorNode::AND_ASSIGN:
            if (lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN) return TYPE_BOOLEAN;
            break;
        case BinaryOperatorNode::EQUAL:
        case BinaryOperatorNode::DIFFERENT:
            if (lhs == TYPE_BOOLEAN && rhs == TYPE_BOOLEAN ||
                lhs == TYPE_NUMBER && rhs == TYPE_NUMBER ||
                lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_BOOLEAN;
            break;
        case BinaryOperatorNode::LESS:
        case BinaryOperatorNode::GREATER:
        case BinaryOperatorNode::LESS_EQUAL:
        case BinaryOperatorNode::GREATER_EQUAL:
            if (lhs == TYPE_NUMBER && rhs == TYPE_NUMBER ||
                lhs == TYPE_STRING && rhs == TYPE_STRING) return TYPE_BOOLEAN;
            break;
    }
    throw SemanticError("invalid operands");
}

Value::MemoryClass SemanticAnalyzer::getMemoryClass(UnaryOperatorNode::UnaryOperator, Value::MemoryClass) {
    return Value::RVALUE;
}

Value::MemoryClass SemanticAnalyzer::getMemoryClass(BinaryOperatorNode::BinaryOperator op, Value::MemoryClass lhs,
                                                    Value::MemoryClass) {
    if (op == BinaryOperatorNode::ASSIGN || op == BinaryOperatorNode::ADD_ASSIGN ||
        op == BinaryOperatorNode::SUBTRACT_ASSIGN || op == BinaryOperatorNode::MULTIPLY_ASSIGN ||
        op == BinaryOperatorNode::DIVIDE_ASSIGN || op == BinaryOperatorNode::REMAINDER_ASSIGN ||
        op == BinaryOperatorNode::OR_ASSIGN || op == BinaryOperatorNode::AND_ASSIGN) {
        if (lhs == Value::LVALUE) return Value::LVALUE;
        throw SemanticError("rvalue as left hand side operand of an assignment");
    }
    return Value::RVALUE;
}