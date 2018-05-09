#ifndef BEAUTY_LANG_KEYWORD_H
#define BEAUTY_LANG_KEYWORD_H

#include <map>
#include <string>

enum Keyword {
    KEYWORD_IF,
    KEYWORD_ELSE,
    KEYWORD_WHILE,
    KEYWORD_FOR,
    KEYWORD_BOOLEAN,
    KEYWORD_NUMBER,
    KEYWORD_STRING,
    KEYWORD_DEF,
    KEYWORD_RETURN,
    KEYWORD_PRINT
};

const std::map<std::string, Keyword> GET_KEYWORD = std::map<std::string, Keyword>({
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
});

#endif //BEAUTY_LANG_KEYWORD_H
