#include "parser.h"

#include "expression_parser.h"
#include "syntax_error.h"
#include "types.h"

std::unique_ptr<BlockNode> Parser::parseFile(const TokenList& file) {
  auto iter = file.begin();
  auto node = parseBlock(iter);
  if ((*iter)->getType() != Token::END_OF_FILE) {
    throw SyntaxError((*iter)->getLocation(), "expected end of file");
  }
  return node;
}

TokenList Parser::parseInstruction(TokenIter& iter) {
  TokenList ans;
  while ((*iter)->getType() != Token::LINE_FEED) {
    ans.push_back(*(iter++));
  }
  ans.push_back(*(iter++));
  return ans;
}

Parser::Type Parser::getInstructionType(const TokenList& tokenList) {
  if (tokenList.size() > 3 && tokenList[1]->getType() == Token::IDENTIFIER &&
      tokenList[2]->getType() == Token::OPERATOR &&
      std::dynamic_pointer_cast<OperatorToken>(tokenList[2])->getOperator() == OP_COLON) {
    return VARIABLE_DECLARATION;
  }
  if (tokenList[1]->getType() == Token::KEYWORD) {
    switch (std::dynamic_pointer_cast<KeywordToken>(tokenList[1])->getKeyword()) {
      case KEYWORD_DEF:
        return FUNCTION_DEFINITION;
      case KEYWORD_IF:
        return IF;
      case KEYWORD_ELSE:
        return ELSE;
      case KEYWORD_WHILE:
        return WHILE;
      case KEYWORD_FOR:
        return FOR;
      case KEYWORD_RETURN:
        return RETURN_STATEMENT;
      case KEYWORD_PRINT:
        return PRINT_STATEMENT;
      case KEYWORD_READ:
        return READ_STATEMENT;
      default:
        return EXPRESSION;
    }
  }
  return EXPRESSION;
}

std::unique_ptr<BlockNode> Parser::parseBlock(TokenIter& iter) {
  int baseIndent = std::dynamic_pointer_cast<IndentToken>(*iter)->getSize();
  std::vector<std::unique_ptr<Node>> nodeList;
  while ((*iter)->getType() != Token::END_OF_FILE &&
         std::dynamic_pointer_cast<IndentToken>(*iter)->getSize() == baseIndent) {
    // TODO: multiple lines instruction
    auto currentInstruction = parseInstruction(iter);
    std::unique_ptr<Node> node;
    std::unique_ptr<ExpressionNode> condition;
    std::unique_ptr<BlockNode> block1;
    std::unique_ptr<BlockNode> block2;
    auto it = currentInstruction.cbegin();
    switch (getInstructionType(currentInstruction)) {
      case EXPRESSION:
        node = parseExpression(it);
        if ((*it)->getType() != Token::LINE_FEED) {
          throw SyntaxError((*it)->getLocation(), "expected operator or end of expression");
        }
        break;
      case VARIABLE_DECLARATION:
        if (currentInstruction[3]->getType() == Token::OPERATOR
            && std::dynamic_pointer_cast<OperatorToken>(currentInstruction[3])->getOperator() == OP_OPENING_ROUND) {
          auto sgn = parseFunctionSignature(currentInstruction);
          if ((*iter)->getType() != Token::INDENT ||
              std::dynamic_pointer_cast<IndentToken>(*iter)->getSize() <= baseIndent) {
            throw SyntaxError((*iter)->getLocation(), "expected function implementation");
          }
          block1 = parseBlock(iter);
          node = std::make_unique<FunctionDefinitionNode>(std::get<0>(sgn), std::get<1>(sgn), std::get<2>(sgn),
              std::move(block1));
        } else {
          node = parseVariableDeclaration(currentInstruction);
        }
        break;
      case RETURN_STATEMENT:
        node = parseReturnStatement(currentInstruction);
        break;
      case PRINT_STATEMENT:
        node = parsePrintStatement(currentInstruction);
        break;
      case READ_STATEMENT:
        node = parseReadStatement(currentInstruction);
        break;
      case IF:
        condition = parseCondition(currentInstruction);
        if ((*iter)->getType() != Token::INDENT ||
            std::dynamic_pointer_cast<IndentToken>(*iter)->getSize() <= baseIndent) {
          throw SyntaxError((*iter)->getLocation(), "expected if block");
        }
        block1 = parseBlock(iter);
        block2 = nullptr;
        if ((*iter)->getType() != Token::END_OF_FILE) {
          it = iter;
          currentInstruction = parseInstruction(it);
          if (currentInstruction[0]->getType() == Token::INDENT &&
              std::dynamic_pointer_cast<IndentToken>(currentInstruction[0])->getSize() == baseIndent &&
              getInstructionType(currentInstruction) == ELSE) {
            if ((*it)->getType() != Token::INDENT ||
                std::dynamic_pointer_cast<IndentToken>(*it)->getSize() <= baseIndent) {
              throw SyntaxError((*it)->getLocation(), "expected else block");
            }
            iter = it;
            block2 = parseBlock(iter);
          }
        }
        node = std::make_unique<IfNode>(std::move(condition), std::move(block1), std::move(block2));
        break;
      case WHILE:
        condition = parseCondition(currentInstruction);
        if ((*iter)->getType() != Token::INDENT ||
            std::dynamic_pointer_cast<IndentToken>(*iter)->getSize() <= baseIndent) {
          throw SyntaxError((*iter)->getLocation(), "expected while block");
        }
        block1 = parseBlock(iter);
        node = std::make_unique<WhileNode>(std::move(condition), std::move(block1));
        break;
      case FOR: {
        if (currentInstruction[2]->getType() != Token::IDENTIFIER) {
          throw SyntaxError(currentInstruction[2]->getLocation(), "expected identifier");
        }
        if (currentInstruction[3]->getType() != Token::OPERATOR
            || std::dynamic_pointer_cast<OperatorToken>(currentInstruction[3])->getOperator() != OP_COLON) {
          throw SyntaxError(currentInstruction[3]->getLocation(), "expected colon");
        }
        if (currentInstruction[4]->getType() == Token::LINE_FEED) {
          throw SyntaxError(currentInstruction[4]->getLocation(), "expected expression");
        }
        auto i = currentInstruction.cbegin() + 4;
        auto loop = ExpressionParser::parse(i);
        if ((*iter)->getType() != Token::INDENT ||
            std::dynamic_pointer_cast<IndentToken>(*iter)->getSize() <= baseIndent) {
          throw SyntaxError((*iter)->getLocation(), "expected for block");
        }
        block1 = parseBlock(iter);
        node = std::make_unique<ForNode>(
            std::dynamic_pointer_cast<IdentifierToken>(currentInstruction[2])->getName(),
            std::move(loop),
            std::move(block1)
        );
        break;
      }
      default:
        // TODO: implement structures
        break;
    }
    nodeList.push_back(std::move(node));
  }
  return std::make_unique<BlockNode>(std::move(nodeList));
}

std::unique_ptr<StandaloneExpressionNode> Parser::parseExpression(TokenIter& iter) {
  return std::make_unique<StandaloneExpressionNode>(ExpressionParser::parse(++iter));
}

int Parser::parseType(TokenIter& iter) {
  int nestedArrays = 0;
  while ((*iter)->getType() == Token::KEYWORD
         && std::dynamic_pointer_cast<KeywordToken>(*iter)->getKeyword() == KEYWORD_ARRAY) {
    ++nestedArrays;
    ++iter;
    if ((*iter)->getType() != Token::OPERATOR
        || std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator() != OP_IS_LESS_THAN) {
      throw SyntaxError((*iter)->getLocation(), "expected array type specifier");
    }
    ++iter;
  }
  if ((*iter)->getType() != Token::KEYWORD) {
    throw SyntaxError((*iter)->getLocation(), "expected type specifier");
  }
  int type;
  switch (std::dynamic_pointer_cast<KeywordToken>(*iter)->getKeyword()) {
    case KEYWORD_BOOLEAN:
      type = TYPE_BOOLEAN;
      break;
    case KEYWORD_NUMBER:
      type = TYPE_NUMBER;
      break;
    case KEYWORD_STRING:
      type = TYPE_STRING;
      break;
    default:
      throw SyntaxError((*iter)->getLocation(), "expected type specifier");
  }
  ++iter;
  while (nestedArrays--) {
    type = TYPE_ARRAY(type);
    if ((*iter)->getType() != Token::OPERATOR
        || std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator() != OP_IS_GREATER_THAN) {
      throw SyntaxError((*iter)->getLocation(), "expected closing angular bracket");
    }
    ++iter;
  }
  return type;
}

std::unique_ptr<VariableDeclarationNode> Parser::parseVariableDeclaration(const TokenList& tokenList) {
  // TODO: initialization
  if (tokenList.size() < 5) {
    throw SyntaxError(tokenList.back()->getLocation(), "expected type name or initializer");
  }
  std::string id = std::dynamic_pointer_cast<IdentifierToken>(tokenList[1])->getName();
  int type;
  std::unique_ptr<ExpressionNode> initializer;
  auto iter = tokenList.begin() + 3;
  if (tokenList[3]->getType() == Token::KEYWORD) {
    type = parseType(iter);
    if ((*iter)->getType() == Token::OPERATOR
        && std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator() == OP_EQUALS) {
      ++iter;
      if ((*iter)->getType() == Token::LINE_FEED) {
        throw SyntaxError(tokenList[5]->getLocation(), "expected expression");
      }
      initializer = ExpressionParser::parse(iter);
    }
  } else if (tokenList[3]->getType() == Token::OPERATOR
             && std::dynamic_pointer_cast<OperatorToken>(tokenList[3])->getOperator() == OP_EQUALS) {
    type = TYPE_NONE;
    auto iter = tokenList.begin() + 4;
    if (tokenList[4]->getType() == Token::LINE_FEED) {
      throw SyntaxError(tokenList[4]->getLocation(), "expected expression");
    }
    initializer = ExpressionParser::parse(iter);
  } else {
    // TODO: object; implement this
    throw SyntaxError(tokenList[3]->getLocation(), "expected type name or initializer");
  }
  return std::make_unique<VariableDeclarationNode>(id, type, std::move(initializer));
}

std::tuple<std::string, std::vector<std::pair<std::string, int>>, int>
Parser::parseFunctionSignature(const TokenList& tokenList) {
  std::string name = std::dynamic_pointer_cast<IdentifierToken>(tokenList[1])->getName();
  std::vector<std::pair<std::string, int>> arguments;
  int returnType = TYPE_NONE;
  auto iter = tokenList.cbegin() + 4;
  while ((*iter)->getType() == Token::IDENTIFIER) {
    auto pName = std::dynamic_pointer_cast<IdentifierToken>(*iter)->getName();
    ++iter;
    if ((*iter)->getType() != Token::OPERATOR
        || std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator() != OP_COLON) {
      throw SyntaxError((*iter)->getLocation(), "expected colon");
    }
    int pType = parseType(++iter);
    arguments.emplace_back(std::move(pName), pType);
    if ((*iter)->getType() != Token::OPERATOR) {
      throw SyntaxError((*iter)->getLocation(), "expected comma or closing parenthesis");
    }
    if (std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator() == OP_COMMA) {
      ++iter;
    }
  }
  if ((*iter)->getType() != Token::OPERATOR
      || std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator() != OP_CLOSING_ROUND) {
    throw SyntaxError((*iter)->getLocation(), "expected closing parenthesis");
  }
  ++iter;
  if ((*iter)->getType() == Token::OPERATOR
      && std::dynamic_pointer_cast<OperatorToken>(*iter)->getOperator() == OP_COLON) {
    returnType = parseType(++iter);
  }
  if ((*iter)->getType() != Token::LINE_FEED) {
    throw SyntaxError((*iter)->getLocation(), "expected end of line");
  }
  return std::make_tuple(std::move(name), std::move(arguments), returnType);
}

std::unique_ptr<ReturnInstructionNode> Parser::parseReturnStatement(const TokenList& tokenList) {
  auto iter = tokenList.begin() + 2;
  if ((*iter)->getType() == Token::LINE_FEED) {
    return std::make_unique<ReturnInstructionNode>(nullptr);
  }
  return std::make_unique<ReturnInstructionNode>(ExpressionParser::parse(iter));
}

std::unique_ptr<PrintInstructionNode> Parser::parsePrintStatement(const TokenList& tokenList) {
  auto iter = tokenList.begin() + 2;
  if ((*iter)->getType() == Token::LINE_FEED) {
    throw SyntaxError((*iter)->getLocation(), "expected expression");
  }
  return std::make_unique<PrintInstructionNode>(ExpressionParser::parse(iter));
}

std::unique_ptr<ReadInstructionNode> Parser::parseReadStatement(const TokenList& tokenList) {
  auto iter = tokenList.begin() + 2;
  if ((*iter)->getType() == Token::LINE_FEED) {
    throw SyntaxError((*iter)->getLocation(), "expected expression");
  }
  return std::make_unique<ReadInstructionNode>(ExpressionParser::parse(iter));
}

std::unique_ptr<ExpressionNode> Parser::parseCondition(const TokenList& tokenList) {
  auto iter = tokenList.begin() + 2;
  if ((*iter)->getType() == Token::LINE_FEED) {
    throw SyntaxError((*iter)->getLocation(), "expected expression");
  }
  return ExpressionParser::parse(iter);
}
