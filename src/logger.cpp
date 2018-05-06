#include "logger.h"

#include <cstdio>

void Logger::print(const Token& token, const std::string& separator) {
    switch (token.getType()) {
        case Token::BOOLEAN:
            std::printf("[%s]", token.getBoolValue() ? "true" : "false");
            break;
        case Token::NUMBER:
            std::printf("[%f]", token.getDoubleValue());
            break;
        case Token::STRING:
            std::printf("[\"%s\"]", token.getStringValue().c_str());
            break;
        case Token::OPERATOR:
            std::printf("[%s]", toString(token.getOperator()).c_str());
            break;
        case Token::KEYWORD:
            std::printf("[KW:%s]", toString(token.getKeyword()).c_str());
            break;
        case Token::IDENTIFIER:
            std::printf("[ID:%s]", token.getStringValue().c_str());
            break;
        case Token::INDENT:
            std::printf("[INDENT:%d]", token.getIntValue());
            break;
        case Token::LINE_FEED:
            std::printf("[LF]");
            break;
        case Token::END_OF_FILE:
            std::printf("[EOF]");
            break;
    }
    printf("%s", separator.c_str());
    if (token.getType() == Token::LINE_FEED) {
        printf("\n");
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