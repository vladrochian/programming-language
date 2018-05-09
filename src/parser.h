#ifndef BEAUTY_LANG_PARSER_H
#define BEAUTY_LANG_PARSER_H

#include <vector>

#include "token.h"
#include "node.h"

class Parser {
  public:
    static Node* parseFile(TokenIter iter);
  private:
    enum Type {
        EXPRESSION,
        IF,
        ELSE,
        WHILE,
        FOR,
        VARIABLE_DECLARATION,
        FUNCTION_DEFINITION,
        RETURN_STATEMENT
    };
    static std::vector<Token> getInstruction(TokenIter& iter);
    static Type getInstructionType(const std::vector<Token>& tokenList);
    static Node* getVariableDeclaration(const std::vector<Token>& tokenList);
    static std::pair<Node*, TokenIter> parseFunction(TokenIter iter);
    static std::pair<Node*, TokenIter> parseBlock(TokenIter iter);
    static std::pair<Node*, TokenIter> parseExpression(TokenIter iter);
};

#endif //BEAUTY_LANG_PARSER_H
