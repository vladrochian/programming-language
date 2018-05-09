#ifndef BEAUTY_LANG_PARSER_H
#define BEAUTY_LANG_PARSER_H

#include <vector>

#include "token.h"
#include "node.h"

class Parser {
  public:
    static BlockNode* parseFile(const std::vector<Token>& file);
  private:
    enum Type {
        EXPRESSION,
        IF,
        ELSE,
        WHILE,
        FOR,
        VARIABLE_DECLARATION,
        FUNCTION_DEFINITION,
        RETURN_STATEMENT,
        PRINT_STATEMENT
    };
    static std::vector<Token> parseInstruction(TokenIter& iter);
    static Type getInstructionType(const std::vector<Token>& tokenList);
    static BlockNode* parseBlock(TokenIter& iter);
    static ExpressionNode* parseExpression(TokenIter& iter);
    static VariableDeclarationNode* parseVariableDeclaration(const std::vector<Token>& tokenList);
    static ReturnInstructionNode* parseReturnStatement(const std::vector<Token>& tokenList);
    static PrintInstructionNode* parsePrintStatement(const std::vector<Token>& tokenList);
};

#endif //BEAUTY_LANG_PARSER_H
