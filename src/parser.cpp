#include "parser.h"

#include "syntax_error.h"
#include "types.h"

Node* Parser::parseFile(TokenIter iter) {

}

std::vector<Token> Parser::getInstruction(TokenIter& iter) {
    std::vector<Token> ans;
    while (iter->getType() != Token::LINE_FEED) {
        ans.push_back(*(iter++));
    }
    ++iter;
    return ans;
}

Parser::Type Parser::getInstructionType(const std::vector<Token>& tokenList) {
    if (tokenList.size() == 4 && tokenList[1].getType() == Token::IDENTIFIER &&
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
            default:
                return EXPRESSION;
        }
    }
    return EXPRESSION;
}

Node* Parser::getVariableDeclaration(const std::vector<Token>& tokenList) {
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
        // object; implement this
        auto location = tokenList[3].getLocation();
        throw SyntaxError(location.first, location.second, "expected type name");
    }
    return new VariableDeclarationNode(id, type);
}

std::pair<Node*, TokenIter> Parser::parseFunction(TokenIter iter) {

}

std::pair<Node*, TokenIter> Parser::parseBlock(TokenIter iter) {}

std::pair<Node*, TokenIter> Parser::parseExpression(TokenIter iter) {}
