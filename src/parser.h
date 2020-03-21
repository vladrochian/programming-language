#ifndef PROG_LANG_PARSER_H
#define PROG_LANG_PARSER_H

#include <vector>
#include <tuple>

#include "token.h"
#include "node.h"

class Parser {
 public:
  static std::unique_ptr<BlockNode> parseFile(const TokenList& file);
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
    PRINT_STATEMENT,
    READ_STATEMENT
  };
  static TokenList parseInstruction(TokenIter& iter);
  static Type getInstructionType(const TokenList& tokenList);
  static std::unique_ptr<BlockNode> parseBlock(TokenIter& iter);
  static std::unique_ptr<StandaloneExpressionNode> parseExpression(TokenIter& iter);
  static int parseType(TokenIter& iter);
  static std::unique_ptr<VariableDeclarationNode> parseVariableDeclaration(const TokenList& tokenList);
  static std::tuple<std::string, std::vector<std::pair<std::string, int>>, int>
  parseFunctionSignature(const TokenList& tokenList);
  static std::unique_ptr<ReturnInstructionNode> parseReturnStatement(const TokenList& tokenList);
  static std::unique_ptr<PrintInstructionNode> parsePrintStatement(const TokenList& tokenList);
  static std::unique_ptr<ReadInstructionNode> parseReadStatement(const TokenList& tokenList);
  static std::unique_ptr<ExpressionNode> parseCondition(const TokenList& tokenList);
};

#endif //PROG_LANG_PARSER_H
