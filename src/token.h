#ifndef BEAUTY_LANG_TOKEN_H
#define BEAUTY_LANG_TOKEN_H

class Token {
  public:
    int code;

    enum {
        TOKEN_ID,
        TOKEN_CT_INT,

        TOKEN_SEMICOLON,
        TOKEN_COLON,
        TOKEN_INT,
        TOKEN_FUN,
        TOKEN_EOF
    };

    Token(int code);
};

#endif //BEAUTY_LANG_TOKEN_H
