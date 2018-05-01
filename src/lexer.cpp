#include "lexer.h";

#include <fstream>

std::vector<Token> Lexer::parsefile(const std::string& fileName) {
    std::ifstream input(fileName);
    std::string buffer;
    std::vector<Token> tokenList;
    int currentLine = 1;
    // TODO: check what happens with empty lines
    while (std::getline(input, buffer)) {
        std::vector<Token> line = parseLine(currentLine, buffer);
        tokenList.insert(tokenList.end(), line.begin(), line.end());
        ++currentLine;
    }
    tokenList.emplace_back(currentLine, 1, Token::END_OF_FILE);
    return tokenList;
}

std::vector<Token> Lexer::parseLine(int lineIndex, const std::string& buffer) {
    std::string::const_iterator it = buffer.begin();
    int indentSize = skipWhitespace(buffer, it);
    if (it == buffer.end()) {
        return std::vector<Token>();
    }
    std::vector<Token> tokenList;
    tokenList.emplace_back(lineIndex, 1, Token::INDENT, indentSize);
    while (it != buffer.end()) {
        // TODO: implement
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
