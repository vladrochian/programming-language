#include "logger.h"

#include <cstdio>

void Logger::print(const Token& token, const std::string& separator) {
    switch (token.getType()) {
        case Token::BOOLEAN:
            std::printf("[BOOLEAN:%s]", token.getBoolValue() ? "true" : "false");
            break;
        case Token::NUMBER:
            std::printf("[NUMBER:%f]", token.getDoubleValue());
            break;
        case Token::STRING:
            std::printf("[STRING:\"%s\"]", token.getStringValue());
            break;
        case Token::OPERATOR:
            std::printf("[OPERATOR:%s]", toString(token.getOperator()));
            break;
        case Token::KEYWORD:
            std::printf("[KEYWORD:%s]", toString(token.getKeyword()));
            break;
        case Token::IDENTIFIER:
            std::printf("[ID:%s]", token.getStringValue());
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
}

std::string Logger::toString(Operator op) {
    for (const auto& it : GET_OPERATOR) {
        if (it.second == op) {
            return it.first;
        }
    }
    return ""s;
}

std::string Logger::toString(Keyword keyword) {
    for (const auto& it : GET_KEYWORD) {
        if (it.second == keyword) {
            return it.first;
        }
    }
    return ""s;
}