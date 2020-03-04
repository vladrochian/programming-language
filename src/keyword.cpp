#include "keyword.h"

#include <memory>

namespace {
typedef std::map<std::string, Keyword> Map;
std::unique_ptr<Map> keywordMapping;
}

void initializeKeywordMapping() {
  Map mp{
      {"if", KEYWORD_IF},
      {"else", KEYWORD_ELSE},
      {"while", KEYWORD_WHILE},
      {"for", KEYWORD_FOR},
      {"boolean", KEYWORD_BOOLEAN},
      {"number", KEYWORD_NUMBER},
      {"string", KEYWORD_STRING},
      {"def", KEYWORD_DEF},
      {"return", KEYWORD_RETURN},
      {"print", KEYWORD_PRINT},
      {"read", KEYWORD_READ}
  };
  keywordMapping = std::make_unique<Map>(std::move(mp));
}

const Map& keywordMap() {
  return *keywordMapping;
}
