#include "logger.h"

#include <cstdio>

void Logger::print(const Token& token) {
    std::printf("[");
    switch (token.getType()) {
        case Token::BOOLEAN:
            std::printf("%s", token.getBoolValue() ? "true" : "false");
            break;
        case Token::NUMBER:
            std::printf("%f", token.getDoubleValue());
            break;
        case Token::STRING:
            std::printf("\"%s\"", token.getStringValue().c_str());
            break;
        case Token::OPERATOR:
            std::printf("%s", toString(token.getOperator()).c_str());
            break;
        case Token::KEYWORD:
            std::printf("KW:%s", toString(token.getKeyword()).c_str());
            break;
        case Token::IDENTIFIER:
            std::printf("ID:%s", token.getStringValue().c_str());
            break;
        case Token::INDENT:
            std::printf("INDENT:%d", token.getIntValue());
            break;
        case Token::LINE_FEED:
            std::printf("LF");
            break;
        case Token::END_OF_FILE:
            std::printf("EOF");
            break;
    }
    auto location = token.getLocation();
    std::printf("|%d,%d] ", location.first, location.second);
    if (token.getType() == Token::LINE_FEED || token.getType() == Token::END_OF_FILE) {
        printf("\n");
    }
}

void Logger::print(Node* node, int indent) {
    auto varNode = dynamic_cast<VariableDeclarationNode*>(node);
    auto ifNode = dynamic_cast<IfNode*>(node);
    auto whlNode = dynamic_cast<WhileNode*>(node);
    printIndent(indent);
    switch (node->getType()) {
        case Node::BLOCK:
            std::printf("[BLOCK:\n");
            for (auto it : dynamic_cast<BlockNode*>(node)->getContent()) {
                print(it, indent + 1);
            }
            printIndent(indent);
            std::printf("]\n");
            break;
        case Node::STANDALONE_EXPRESSION:
            std::printf("[EXP:");
            printExpression(dynamic_cast<StandaloneExpressionNode*>(node)->getExpression());
            std::printf("]\n");
            break;
        case Node::VARIABLE_DECLARATION:
            std::printf("[VAR:%s,%d]\n", varNode->getVariableName().c_str(), varNode->getVariableType());
            break;
        case Node::RETURN_INSTRUCTION:
            std::printf("[RET:");
            printExpression(dynamic_cast<ReturnInstructionNode*>(node)->getExpression());
            std::printf("]\n");
            break;
        case Node::PRINT_INSTRUCTION:
            std::printf("[PRN:");
            printExpression(dynamic_cast<PrintInstructionNode*>(node)->getExpression());
            std::printf("]\n");
            break;
        case Node::IF_STATEMENT:
            std::printf("IF [CND:");
            printExpression(ifNode->getCondition());
            std::printf("]:\n");
            print(ifNode->getThenBlock(), indent);
            if (ifNode->getElseBlock() != nullptr) {
                printIndent(indent);
                std::printf("ELSE:\n");
                print(ifNode->getElseBlock(), indent);
            }
            break;
        case Node::WHILE_STATEMENT:
            std::printf("WHILE [CND:");
            printExpression(whlNode->getCondition());
            std::printf("]:\n");
            print(whlNode->getBlock(), indent);
            break;
        default:
            std::printf("[NODE]\n");
    }
}

std::string Logger::toString(OperatorToken op) {
    for (const auto& it : GET_OPERATOR) {
        if (it.second == op) {
            return it.first;
        }
    }
    return "";
}

std::string Logger::toString(Keyword keyword) {
    for (const auto& it : GET_KEYWORD) {
        if (it.second == keyword) {
            return it.first;
        }
    }
    return "";
}

void Logger::printExpression(ExpressionNode* node) {
    ExpressionNode* p1;
    ExpressionNode* p2;
    auto unOp = dynamic_cast<UnaryOperatorNode*>(node);
    auto binOp = dynamic_cast<BinaryOperatorNode*>(node);
    switch (node->getType()) {
        case Node::VARIABLE:
            std::printf("(VAR:%s)", dynamic_cast<VariableNode*>(node)->getName().c_str());
            break;
        case Node::BOOLEAN_VALUE:
            std::printf("(BOOL:%s)", dynamic_cast<BooleanValueNode*>(node)->getValue() ? "true" : "false");
            break;
        case Node::NUMBER_VALUE:
            std::printf("(NMB:%f)", dynamic_cast<NumberValueNode*>(node)->getValue());
            break;
        case Node::STRING_VALUE:
            std::printf("(STR:%s)", dynamic_cast<StringValueNode*>(node)->getValue().c_str());
            break;
        case Node::UNARY_OPERATOR:
            p1 = unOp->getOperand();
            std::printf("(%d ", unOp->getOperator());
            printExpression(p1);
            std::printf(")");
            break;
        case Node::BINARY_OPERATOR:
            p1 = binOp->getLeftOperand();
            p2 = binOp->getRightOperand();
            std::printf("(%d ", binOp->getOperator());
            printExpression(p1);
            std::printf(" ");
            printExpression(p2);
            std::printf(")");
            break;
        default:
            std::printf("(EXP)");
    }
}

void Logger::printIndent(int size) {
    while (size--) {
        std::printf(" ");
    }
}
