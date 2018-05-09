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
    auto blockNode = dynamic_cast<BlockNode*>(node);
    auto varNode = dynamic_cast<VariableDeclarationNode*>(node);
    auto retNode = dynamic_cast<ReturnInstructionNode*>(node);
    auto prnNode = dynamic_cast<PrintInstructionNode*>(node);
    printIndent(indent);
    switch (node->getType()) {
        case Node::BLOCK:
            std::printf("[BLOCK:\n");
            for (auto it : blockNode->getContent()) {
                print(it, indent + 1);
            }
            printIndent(indent);
            std::printf("]\n");
            break;
        case Node::EXPRESSION:
            std::printf("[EXP:");
            printExpression(dynamic_cast<ExpressionNode*>(node));
            std::printf("]\n");
            break;
        case Node::VARIABLE_DECLARATION:
            std::printf("[VAR:%s,%d]\n", varNode->getVariableName().c_str(), varNode->getVariableType());
            break;
        case Node::RETURN_INSTRUCTION:
            std::printf("[RET:");
            printExpression(retNode->getExpression());
            std::printf("]\n");
            break;
        case Node::PRINT_INSTRUCTION:
            std::printf("[PRN:");
            printExpression(prnNode->getExpression());
            std::printf("]\n");
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
    switch (node->getType()) {
        default:
            std::printf("(EXP)");
    }
}

void Logger::printIndent(int size) {
    while (size--) {
        std::printf(" ");
    }
}
