#include "vm.h"

#include <cmath>
#include <iostream>

#include "runtime_error.h"
#include "store.h"

void VirtualMachine::run(Node* node) {
    if (node->getType() == Node::BLOCK) {
        auto blockNode = dynamic_cast<BlockNode*>(node);
        store.newLevel();
        for (auto it : blockNode->getContent()) {
            run(it);
        }
        store.deleteLevel();
    } else if (node->getType() == Node::STANDALONE_EXPRESSION) {
        evalExp(dynamic_cast<StandaloneExpressionNode*>(node)->getExpression());
    } else if (node->getType() == Node::RETURN_INSTRUCTION) {
        // TODO: functions
        evalExp(dynamic_cast<ReturnInstructionNode*>(node)->getExpression());
    } else if (node->getType() == Node::PRINT_INSTRUCTION) {
        auto value = evalExp(dynamic_cast<PrintInstructionNode*>(node)->getExpression());
        switch (value->getType()) {
            case TYPE_BOOLEAN:
                std::cout << (value->getBooleanValue() ? "true\n" : "false\n");
                break;
            case TYPE_NUMBER:
                std::cout << value->getNumberValue() << "\n";
                break;
            case TYPE_STRING:
                std::cout << value->getStringValue() << "\n";
                break;
        }
    } else if (node->getType() == Node::VARIABLE_DECLARATION) {
        auto varDecNode = dynamic_cast<VariableDeclarationNode*>(node);
        store.registerName(varDecNode->getVariableName(), new VariableData(varDecNode->getVariableType()));
    } else if (node->getType() == Node::IF_STATEMENT) {
        auto ifNode = dynamic_cast<IfNode*>(node);
        if (evalExp(ifNode->getCondition())->getBooleanValue()) {
            run(ifNode->getThenBlock());
        } else if (ifNode->getElseBlock() != nullptr) {
            run(ifNode->getElseBlock());
        }
    } else if (node->getType() == Node::WHILE_STATEMENT) {
        auto whileNode = dynamic_cast<WhileNode*>(node);
        while (evalExp(whileNode->getCondition())->getBooleanValue()) {
            run(whileNode->getBlock());
        }
    }
}

Value* VirtualMachine::evalExp(ExpressionNode* node) {
    if (node->getType() == Node::BOOLEAN_VALUE || node->getType() == Node::NUMBER_VALUE ||
        node->getType() == Node::STRING_VALUE) {
        return new Rvalue(node);
    }
    if (node->getType() == Node::VARIABLE) {
        return new Lvalue(dynamic_cast<VariableNode*>(node)->getName());
    }
    if (node->getType() == Node::UNARY_OPERATOR) {
        auto unOpNode = dynamic_cast<UnaryOperatorNode*>(node);
        switch (unOpNode->getOperator()) {
            case UnaryOperatorNode::PLUS:
                return evalExp(unOpNode->getOperand());
            case UnaryOperatorNode::MINUS:
                return new Rvalue(-evalExp(unOpNode->getOperand())->getNumberValue());
            case UnaryOperatorNode::NOT:
                return new Rvalue(!evalExp(unOpNode->getOperand())->getBooleanValue());
        }
    }
    auto binOpNode = dynamic_cast<BinaryOperatorNode*>(node);
    auto ls = evalExp(binOpNode->getLeftOperand());
    auto rs = evalExp(binOpNode->getRightOperand());
    switch (binOpNode->getOperator()) {
        case BinaryOperatorNode::ADD:
            if (ls->getType() == TYPE_NUMBER) {
                return new Rvalue(ls->getNumberValue() + rs->getNumberValue());
            }
            return new Rvalue(ls->getStringValue() + rs->getStringValue());
        case BinaryOperatorNode::SUBTRACT:
            return new Rvalue(ls->getNumberValue() - rs->getNumberValue());
        case BinaryOperatorNode::MULTIPLY:
            return new Rvalue(ls->getNumberValue() * rs->getNumberValue());
        case BinaryOperatorNode::DIVIDE:
            if (rs->getNumberValue() == 0.0) {
                throw RuntimeError("division by 0");
            }
            return new Rvalue(ls->getNumberValue() / rs->getNumberValue());
        case BinaryOperatorNode::REMAINDER:
            return new Rvalue(ls->getNumberValue() -
                              std::floor(ls->getNumberValue() / rs->getNumberValue()) * rs->getNumberValue());
        case BinaryOperatorNode::OR:
            return new Rvalue(ls->getBooleanValue() || rs->getBooleanValue());
        case BinaryOperatorNode::AND:
            return new Rvalue(ls->getBooleanValue() && rs->getBooleanValue());
        case BinaryOperatorNode::ASSIGN:
            dynamic_cast<Lvalue*>(ls)->setValue(rs);
            return ls;
        case BinaryOperatorNode::ADD_ASSIGN:
            dynamic_cast<Lvalue*>(ls)->setValue(
                    (ls->getType() == TYPE_NUMBER) ? new Rvalue(ls->getNumberValue() + rs->getNumberValue())
                                                   : new Rvalue(ls->getStringValue() + rs->getStringValue())
            );
            return ls;
        case BinaryOperatorNode::SUBTRACT_ASSIGN:
            dynamic_cast<Lvalue*>(ls)->setValue(new Rvalue(ls->getNumberValue() - rs->getNumberValue()));
            return ls;
        case BinaryOperatorNode::MULTIPLY_ASSIGN:
            dynamic_cast<Lvalue*>(ls)->setValue(new Rvalue(ls->getNumberValue() * rs->getNumberValue()));
            return ls;
        case BinaryOperatorNode::DIVIDE_ASSIGN:
            if (rs->getNumberValue() == 0.0) {
                throw RuntimeError("division by 0");
            }
            dynamic_cast<Lvalue*>(ls)->setValue(new Rvalue(ls->getNumberValue() / rs->getNumberValue()));
            return ls;
        case BinaryOperatorNode::REMAINDER_ASSIGN:
            dynamic_cast<Lvalue*>(ls)->setValue(new Rvalue(ls->getNumberValue() -
                                                           std::floor(ls->getNumberValue() / rs->getNumberValue()) *
                                                           rs->getNumberValue()));
            return ls;
        case BinaryOperatorNode::OR_ASSIGN:
            dynamic_cast<Lvalue*>(ls)->setValue(new Rvalue(ls->getBooleanValue() || rs->getBooleanValue()));
            return ls;
        case BinaryOperatorNode::AND_ASSIGN:
            dynamic_cast<Lvalue*>(ls)->setValue(new Rvalue(ls->getBooleanValue() && rs->getBooleanValue()));
            return ls;
        case BinaryOperatorNode::EQUAL:
            switch (ls->getType()) {
                case TYPE_BOOLEAN:
                    return new Rvalue(ls->getBooleanValue() == rs->getBooleanValue());
                case TYPE_NUMBER:
                    return new Rvalue(ls->getNumberValue() == rs->getNumberValue());
                case TYPE_STRING:
                    return new Rvalue(ls->getStringValue() == rs->getStringValue());
            }
        case BinaryOperatorNode::DIFFERENT:
            switch (ls->getType()) {
                case TYPE_BOOLEAN:
                    return new Rvalue(ls->getBooleanValue() != rs->getBooleanValue());
                case TYPE_NUMBER:
                    return new Rvalue(ls->getNumberValue() != rs->getNumberValue());
                case TYPE_STRING:
                    return new Rvalue(ls->getStringValue() != rs->getStringValue());
            }
        case BinaryOperatorNode::LESS:
            if (ls->getType() == TYPE_NUMBER) {
                return new Rvalue(ls->getNumberValue() < rs->getNumberValue());
            }
            return new Rvalue(ls->getStringValue() < rs->getStringValue());
        case BinaryOperatorNode::GREATER:
            if (ls->getType() == TYPE_NUMBER) {
                return new Rvalue(ls->getNumberValue() > rs->getNumberValue());
            }
            return new Rvalue(ls->getStringValue() > rs->getStringValue());
        case BinaryOperatorNode::LESS_EQUAL:
            if (ls->getType() == TYPE_NUMBER) {
                return new Rvalue(ls->getNumberValue() <= rs->getNumberValue());
            }
            return new Rvalue(ls->getStringValue() <= rs->getStringValue());
        case BinaryOperatorNode::GREATER_EQUAL:
            if (ls->getType() == TYPE_NUMBER) {
                return new Rvalue(ls->getNumberValue() >= rs->getNumberValue());
            }
            return new Rvalue(ls->getStringValue() >= rs->getStringValue());
    }
}
