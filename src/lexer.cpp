#include "lexer.h"

#include <fstream>

#include "keyword.h"
#include "operator.h"
#include "syntax_error.h"

TokenList Lexer::readfile(const std::string& fileName) {
  std::ifstream input(fileName);
  std::string buffer;
  TokenList tokenList;
  int currentLine = 1;
  // TODO: check what happens with empty lines
  while (std::getline(input, buffer)) {
    if (!buffer.empty() && buffer.back() == '\r') {
      buffer.pop_back();
    }
    TokenList line = readLine(currentLine, buffer);
    tokenList.insert(tokenList.end(), line.begin(), line.end());
    ++currentLine;
  }
  tokenList.push_back(std::make_shared<EndOfFileToken>(currentLine, 1));
  return tokenList;
}

TokenList Lexer::readLine(int lineIndex, const std::string& buffer) {
  std::string::const_iterator it = buffer.begin();
  int indentSize = skipWhitespace(buffer, it);
  if (it == buffer.end()) {
    return TokenList();
  }
  TokenList tokenList;
  tokenList.push_back(std::make_shared<IndentToken>(lineIndex, 1, indentSize));
  while (it != buffer.end()) {
    int col = static_cast<int>(it - buffer.begin() + 1);
    std::string s;
    double n;
    if (std::isdigit(*it)) {
      n = getNumber(buffer, it);
      if (it != buffer.end() && (std::isalpha(*it) || *it == '_')) {
        throw SyntaxError(lineIndex, static_cast<int>(it - buffer.begin() + 1),
            "unexpected symbol in numeric value");
      }
      tokenList.push_back(std::make_shared<NumberToken>(lineIndex, col, n));
    } else if (std::isalpha(*it) || *it == '_') {
      s = getWord(buffer, it);
      if (s == "false") {
        tokenList.push_back(std::make_shared<BooleanToken>(lineIndex, col, false));
      } else if (s == "true") {
        tokenList.push_back(std::make_shared<BooleanToken>(lineIndex, col, true));
      } else if (keywordMap().count(s) == 1) {
        tokenList.push_back(std::make_shared<KeywordToken>(lineIndex, col, keywordMap().at(s)));
      } else {
        tokenList.push_back(std::make_shared<IdentifierToken>(lineIndex, col, s));
      }
    } else if (*it == '\'' || *it == '\"') {
      s = getString(buffer, it);
      if (it == buffer.end()) {
        throw SyntaxError(lineIndex, static_cast<int>(it - buffer.begin() + 1), "expected ending quote");
      }
      ++it;
      tokenList.push_back(std::make_shared<StringToken>(lineIndex, col, s));
    } else {
      s = getOperator(buffer, it);
      if (s.empty()) {
        throw SyntaxError(lineIndex, col, "unknown symbol");
      } else {
        tokenList.push_back(std::make_shared<OperatorToken>(lineIndex, col, operatorTokenMap().at(s)));
      }
    }
    skipWhitespace(buffer, it);
  }
  tokenList.push_back(std::make_shared<LineFeedToken>(lineIndex, buffer.size() + 1));
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

std::string Lexer::getWord(const std::string& buffer, std::string::const_iterator& it) {
  std::string ans;
  while ((it != buffer.end() && std::isalnum(*it)) || *it == '_') {
    ans += *(it++);
  }
  return ans;
}

double Lexer::getNumber(const std::string& buffer, std::string::const_iterator& it) {
  // TODO: scientific notation?
  double ans = 0;
  while (it != buffer.end() && std::isdigit(*it)) {
    ans = ans * 10 + *(it++) - '0';
  }
  if (it != buffer.end() && *it == '.') {
    ++it;
    double pw = 1.0;
    while (it != buffer.end() && std::isdigit(*it)) {
      ans += (pw /= 10) * (*(it++) - '0');
    }
  }
  return ans;
}

std::string Lexer::getString(const std::string& buffer, std::string::const_iterator& it) {
  // TODO: escape characters
  char quote = *(it++);
  std::string ans;
  while (it != buffer.end() && *it != quote) {
    ans += *(it++);
  }
  return ans;
}

std::string Lexer::getOperator(const std::string& buffer, std::string::const_iterator& it) {
  std::string ans;
  while (it != buffer.end() && !std::isalnum(*it) && *it != '_' && !std::isspace(*it)) {
    ans += *(it++);
  }
  while (!ans.empty() && operatorTokenMap().count(ans) == 0) {
    ans.pop_back();
    --it;
  }
  return ans;
}
