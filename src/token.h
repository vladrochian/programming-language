#ifndef BEAUTY_LANG_TOKEN_H
#define BEAUTY_LANG_TOKEN_H

#include <string>
#include <vector>

#include "keyword.h"
#include "operator.h"

class Token {
  public:
    enum Type {
        BOOLEAN,
        NUMBER,
        STRING,
        OPERATOR,
        KEYWORD,
        IDENTIFIER,
        INDENT,
        LINE_FEED,
        END_OF_FILE
    };

    Token(int line, int col, Type type);
    Token(int line, int col, Type type, bool value);
    Token(int line, int col, Type type, int value);
    Token(int line, int col, Type type, double value);
    Token(int line, int col, Type type, std::string value);
    Token(int line, int col, Type type, OperatorToken value);
    Token(int line, int col, Type type, Keyword value);

    Type getType() const;
    std::pair<int, int> getLocation() const;
    bool getBoolValue() const;
    int getIntValue() const;
    double getDoubleValue() const;
    std::string getStringValue() const;
    OperatorToken getOperator() const;
    Keyword getKeyword() const;

  private:
    Type type;
    std::pair<int, int> location;
    bool boolValue;
    int intValue;
    double doubleValue;
    std::string stringValue;
    OperatorToken operatorValue;
    Keyword keywordValue;
};

typedef std::vector<Token>::iterator TokenIter;

#endif //BEAUTY_LANG_TOKEN_H
