#include "lexer.h"

#include <fstream>
#include <iostream>

#include "keyword.h"
#include "operator.h"
#include "syntax_error.h"

std::vector<Token> Lexer::readfile(const std::string& fileName) {
    std::ifstream input(fileName);
    if (input) {
        std::cout << "Read file\n";
    }
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
        if (std::isdigit(*it)) {
            n = getNumber(buffer, it);
            if (it != buffer.end() && (std::isalpha(*it) || *it == '_')) {
                throw SyntaxError(lineIndex, static_cast<int>(it - buffer.begin() + 1),
                                  "unexpected symbol in numeric value");
            }
            tokenList.emplace_back(lineIndex, col, Token::NUMBER, n);
        } else if (std::isalpha(*it) || *it == '_') {
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
            if (it == buffer.end()) {
                throw SyntaxError(lineIndex, static_cast<int>(it - buffer.begin() + 1), "expected ending quote");
            }
            ++it;
            tokenList.emplace_back(lineIndex, col, Token::STRING, s);
        } else {
            s = getOperator(buffer, it);
            if (s.empty()) {
                throw SyntaxError(lineIndex, col, "unknown symbol");
            } else {
                tokenList.emplace_back(lineIndex, col, Token::OPERATOR, GET_OPERATOR.at(s));
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

std::string Lexer::getWord(const std::string& buffer, std::string::const_iterator& it) {
    std::string ans;
    while (it != buffer.end() && std::isalnum(*it) || *it == '_') {
        ans += *(it++);
    }
    return ans;
}

double Lexer::getNumber(const std::string& buffer, std::string::const_iterator& it) {
    // TODO: scientific notation?
    double ans = 0;
    while (it != buffer.end() && std::isdigit(*it)) {
        ans = ans * 10 + *(it++) - '0';
    }
    if (it != buffer.end() && *it == '.') {
        ++it;
        double pw = 1.0;
        while (it != buffer.end() && std::isdigit(*it)) {
            ans += (pw /= 10) * (*(it++) - '0');
        }
    }
    return ans;
}

std::string Lexer::getString(const std::string& buffer, std::string::const_iterator& it) {
    // TODO: escape characters
    char quote = *(it++);
    std::string ans;
    while (it != buffer.end() && *it != quote) {
        ans += *(it++);
    }
    return ans;
}

std::string Lexer::getOperator(const std::string& buffer, std::string::const_iterator& it) {
    std::string ans;
    while (it != buffer.end() && !std::isalnum(*it) && *it != '_' && !std::isspace(*it)) {
        ans += *(it++);
    }
    while (!ans.empty() && GET_OPERATOR.count(ans) == 0) {
        ans.pop_back();
        --it;
    }
    return ans;
}
