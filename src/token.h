#ifndef PROG_LANG_TOKEN_H
#define PROG_LANG_TOKEN_H

#include <memory>
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

  virtual ~Token() = default;
  virtual Type getType() const = 0;

  virtual std::pair<int, int> getLocation() const final { return location; }

 protected:
  Token(int line, int col) : location(line, col) {}

 private:
  std::pair<int, int> location;
};

class BooleanToken : public Token {
 public:
  BooleanToken(int line, int col, bool value) : Token(line, col), value(value) {}

  Type getType() const override { return BOOLEAN; }

  bool getValue() const { return value; }

 private:
  bool value;
};

class NumberToken : public Token {
 public:
  NumberToken(int line, int col, double value) : Token(line, col), value(value) {}

  Type getType() const override { return NUMBER; }

  double getValue() const { return value; }

 private:
  double value;
};

class StringToken : public Token {
 public:
  StringToken(int line, int col, std::string value) : Token(line, col), value(std::move(value)) {}

  Type getType() const override { return STRING; }

  std::string getValue() const { return value; }

 private:
  std::string value;
};

class OperatorToken : public Token {
 public:
  OperatorToken(int line, int col, OperatorTokenType op) : Token(line, col), op(op) {}

  Type getType() const override { return OPERATOR; }

  OperatorTokenType getOperator() const { return op; }

 private:
  OperatorTokenType op;
};

class KeywordToken : public Token {
 public:
  KeywordToken(int line, int col, Keyword keyword) : Token(line, col), keyword(keyword) {}

  Type getType() const override { return KEYWORD; }

  Keyword getKeyword() const { return keyword; }

 private:
  Keyword keyword;
};

class IdentifierToken : public Token {
 public:
  IdentifierToken(int line, int col, std::string name) : Token(line, col), name(std::move(name)) {}

  Type getType() const override { return IDENTIFIER; }

  std::string getName() const { return name; }

 private:
  std::string name;
};

class IndentToken : public Token {
 public:
  IndentToken(int line, int col, int size) : Token(line, col), size(size) {}

  Type getType() const override { return INDENT; }

  int getSize() const { return size; }

 private:
  int size;
};

class LineFeedToken : public Token {
 public:
  LineFeedToken(int line, int col) : Token(line, col) {}

  Type getType() const override { return LINE_FEED; }
};

class EndOfFileToken : public Token {
 public:
  EndOfFileToken(int line, int col) : Token(line, col) {}

  Type getType() const override { return END_OF_FILE; }
};

typedef std::vector<std::shared_ptr<Token>> TokenList;
typedef TokenList::const_iterator TokenIter;

#endif //PROG_LANG_TOKEN_H
