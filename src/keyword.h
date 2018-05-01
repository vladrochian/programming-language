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
    KEYWORD_RETURN
};

constexpr std::map<std::string, Keyword> GET_KEYWORD = {
        {"if"s, KEYWORD_IF},
        {"else"s, KEYWORD_ELSE},
        {"while"s, KEYWORD_WHILE},
        {"for"s, KEYWORD_FOR},
        {"boolean"s, KEYWORD_BOOLEAN},
        {"number"s, KEYWORD_NUMBER},
        {"string"s, KEYWORD_STRING},
        {"def"s, KEYWORD_DEF},
        {"return"s, KEYWORD_RETURN}
};

#endif //BEAUTY_LANG_KEYWORD_H
