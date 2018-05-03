#include "lexer.h";

#include <fstream>

#include "keyword.h"
#include "operator.h"

std::vector<Token> Lexer::readfile(const std::string& fileName) {
    std::ifstream input(fileName);
    std::string buffer;
    std::vector<Token> tokenList;
    int currentLine = 1;
    // TODO: check what happens with empty lines
    while (std::getline(input, buffer)) {
        std::vector<Token> line = readLine(currentLine, buffer);
        tokenList.insert(tokenList.end(), line.begin(), line.end());
        ++currentLine;
    }
    tokenList.emplace_back(currentLine, 1, Token::END_OF_FILE);
    return tokenList;
}

std::vector<Token> Lexer::readLine(int lineIndex, const std::string& buffer) {
    std::string::const_iterator it = buffer.begin();
    int indentSize = skipWhitespace(buffer, it);
    if (it == buffer.end()) {
        return std::vector<Token>();
    }
    std::vector<Token> tokenList;
    tokenList.emplace_back(lineIndex, 1, Token::INDENT, indentSize);
    while (it != buffer.end()) {
        int col = static_cast<int>(it - buffer.begin() + 1);
        std::string s;
        double n;
        if (isdigit(*it)) {
            n = getNumber(buffer, it);
            tokenList.emplace_back(lineIndex, col, Token::NUMBER, n);
        } else if (isalpha(*it) || *it == '_') {
            s = getWord(buffer, it);
            if (s == "false") {
                tokenList.emplace_back(lineIndex, col, Token::BOOLEAN, false);
            } else if (s == "true") {
                tokenList.emplace_back(lineIndex, col, Token::BOOLEAN, true);
            } else if (GET_KEYWORD.count(s) == 1) {
                tokenList.emplace_back(lineIndex, col, Token::KEYWORD, GET_KEYWORD.at(s));
            } else {
                tokenList.emplace_back(lineIndex, col, Token::IDENTIFIER, s);
            }
        } else if (*it == '\'' || *it == '\"') {
            s = getString(buffer, it);
            tokenList.emplace_back(lineIndex, col, Token::STRING, s);
        } else {
            s = getOperator(buffer, it);
            if (GET_OPERATOR.count(s)) {
                tokenList.emplace_back(lineIndex, col, Token::OPERATOR, GET_OPERATOR.at(s));
            } else {
                // TODO: Syntax exc
            }
        }
        skipWhitespace(buffer, it);
    }
    tokenList.emplace_back(lineIndex, buffer.size() + 1, Token::LINE_FEED);
    return tokenList;
}

int Lexer::skipWhitespace(const std::string& buffer, std::string::const_iterator& it) {
    int count = 0;
    while (it != buffer.end() && std::isspace(*it)) {
        ++it;
        ++count;
    }
    return count;
}
