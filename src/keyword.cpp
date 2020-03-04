#include "keyword.h"

#include <memory>

namespace {
typedef std::map<std::string, Keyword> MapType;
std::unique_ptr<MapType> keywordMapping;
}

void initializeKeywordMapping() {
  MapType mp {
      {"if", KEYWORD_IF},
      {"else", KEYWORD_ELSE},
      {"while", KEYWORD_WHILE},
      {"for", KEYWORD_FOR},
      {"boolean", KEYWORD_BOOLEAN},
      {"number", KEYWORD_NUMBER},
      {"string", KEYWORD_STRING},
      {"def", KEYWORD_DEF},
      {"return", KEYWORD_RETURN},
      {"print", KEYWORD_PRINT}
  };
  keywordMapping = std::make_unique<MapType>(std::move(mp));
}

const MapType& keywordMap() {
  return *keywordMapping;
}
