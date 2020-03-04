#ifndef PROG_LANG_PARSER_H
#define PROG_LANG_PARSER_H

#include <vector>

#include "token.h"
#include "node.h"

class Parser {
 public:
  static std::unique_ptr<BlockNode> parseFile(const std::vector<Token>& file);
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
  static std::unique_ptr<BlockNode> parseBlock(TokenIter& iter);
  static std::unique_ptr<StandaloneExpressionNode> parseExpression(TokenIter& iter);
  static std::unique_ptr<VariableDeclarationNode> parseVariableDeclaration(const std::vector<Token>& tokenList);
  static std::unique_ptr<ReturnInstructionNode> parseReturnStatement(const std::vector<Token>& tokenList);
  static std::unique_ptr<PrintInstructionNode> parsePrintStatement(const std::vector<Token>& tokenList);
  static std::unique_ptr<ExpressionNode> parseCondition(const std::vector<Token>& tokenList);
};

#endif //PROG_LANG_PARSER_H
