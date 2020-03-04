#include "parser.h"

#include "expression_parser.h"
#include "syntax_error.h"
#include "types.h"

std::unique_ptr<BlockNode> Parser::parseFile(const std::vector<Token>& file) {
    auto iter = file.begin();
    auto node = parseBlock(iter);
    if (iter->getType() != Token::END_OF_FILE) {
        auto location = iter->getLocation();
        throw SyntaxError(location.first, location.second, "expected end of file");
    }
    return node;
}

std::vector<Token> Parser::parseInstruction(TokenIter& iter) {
    std::vector<Token> ans;
    while (iter->getType() != Token::LINE_FEED) {
        ans.push_back(*(iter++));
    }
    ans.push_back(*(iter++));
    return ans;
}

Parser::Type Parser::getInstructionType(const std::vector<Token>& tokenList) {
    if (tokenList.size() > 4 && tokenList[1].getType() == Token::IDENTIFIER &&
        tokenList[2].getType() == Token::OPERATOR && tokenList[2].getOperator() == OP_COLON) {
        return VARIABLE_DECLARATION;
    }
    if (tokenList[1].getType() == Token::KEYWORD) {
        switch (tokenList[1].getKeyword()) {
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
            default:
                return EXPRESSION;
        }
    }
    return EXPRESSION;
}

std::unique_ptr<BlockNode> Parser::parseBlock(TokenIter& iter) {
    int baseIndent = iter->getIntValue();
    std::vector<std::unique_ptr<Node>> nodeList;
    while (iter->getType() != Token::END_OF_FILE && iter->getIntValue() == baseIndent) {
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
                if (it->getType() != Token::LINE_FEED) {
                    auto location = it->getLocation();
                    throw SyntaxError(location.first, location.second, "expected operator or end of expression");
                }
                break;
            case VARIABLE_DECLARATION:
                node = parseVariableDeclaration(currentInstruction);
                break;
            case RETURN_STATEMENT:
                node = parseReturnStatement(currentInstruction);
                break;
            case PRINT_STATEMENT:
                node = parsePrintStatement(currentInstruction);
                break;
            case IF:
                condition = parseCondition(currentInstruction);
                if (iter->getType() != Token::INDENT || iter->getIntValue() <= baseIndent) {
                    auto location = iter->getLocation();
                    throw SyntaxError(location.first, location.second, "expected if block");
                }
                block1 = parseBlock(iter);
                block2 = nullptr;
                if (iter->getType() != Token::END_OF_FILE) {
                    it = iter;
                    currentInstruction = parseInstruction(it);
                    if (currentInstruction[0].getType() == Token::INDENT &&
                        currentInstruction[0].getIntValue() == baseIndent &&
                        getInstructionType(currentInstruction) == ELSE) {
                        if (it->getType() != Token::INDENT || it->getIntValue() <= baseIndent) {
                            auto location = it->getLocation();
                            throw SyntaxError(location.first, location.second, "expected else block");
                        }
                        iter = it;
                        block2 = parseBlock(iter);
                    }
                }
                node = std::make_unique<IfNode>(std::move(condition), std::move(block1), std::move(block2));
                break;
            case WHILE:
                condition = parseCondition(currentInstruction);
                if (iter->getType() != Token::INDENT || iter->getIntValue() <= baseIndent) {
                    auto location = iter->getLocation();
                    throw SyntaxError(location.first, location.second, "expected while block");
                }
                block1 = parseBlock(iter);
                node = std::make_unique<WhileNode>(std::move(condition), std::move(block1));
                break;
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

std::unique_ptr<VariableDeclarationNode> Parser::parseVariableDeclaration(const std::vector<Token>& tokenList) {
    // TODO: initialization
    std::string id = tokenList[1].getStringValue();
    PrimitiveType type;
    if (tokenList[3].getType() == Token::KEYWORD) {
        switch (tokenList[3].getKeyword()) {
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
                auto location = tokenList[3].getLocation();
                throw SyntaxError(location.first, location.second, "expected type name");
        }
    } else {
        // TODO: object; implement this
        auto location = tokenList[3].getLocation();
        throw SyntaxError(location.first, location.second, "expected type name");
    }
    return std::make_unique<VariableDeclarationNode>(id, type);
}

std::unique_ptr<ReturnInstructionNode> Parser::parseReturnStatement(const std::vector<Token>& tokenList) {
    auto iter = tokenList.begin() + 2;
    if (iter->getType() == Token::LINE_FEED) {
        auto location = iter->getLocation();
        throw SyntaxError(location.first, location.second, "expected expression");
    }
    return std::make_unique<ReturnInstructionNode>(ExpressionParser::parse(iter));
}

std::unique_ptr<PrintInstructionNode> Parser::parsePrintStatement(const std::vector<Token>& tokenList) {
    auto iter = tokenList.begin() + 2;
    if (iter->getType() == Token::LINE_FEED) {
        auto location = iter->getLocation();
        throw SyntaxError(location.first, location.second, "expected expression");
    }
    return std::make_unique<PrintInstructionNode>(ExpressionParser::parse(iter));
}

std::unique_ptr<ExpressionNode> Parser::parseCondition(const std::vector<Token>& tokenList) {
    auto iter = tokenList.begin() + 2;
    if (iter->getType() == Token::LINE_FEED) {
        auto location = iter->getLocation();
        throw SyntaxError(location.first, location.second, "expected expression");
    }
    return ExpressionParser::parse(iter);
}
