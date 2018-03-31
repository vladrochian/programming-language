#include <iostream>
#include "Lexer.h";
#include "LexerCodes.h"

Token* Lexer::getNext(std::ifstream &file) {
    char c = 0;
    int state = TOKEN_START;

    for(;c != EOF;) {
        file.get(c);
        this -> skipWhitespaces(file, c);
        switch (state) {
            case TOKEN_START: {
                if (isalpha(c) || c == '_') { //start of an TOKEN_ID/Keyword
//                    return this.getID(file);
                    std::cout << "Found an id start";
                    state = TOKEN_ID; // maybe split into get id?
                    return this -> getID(file, c);
                }
                else if(c == ':') { //type definition coming next
                    return new Token(TOKEN_COLON);
                }
                else if(c == ';') {
                    return new Token(TOKEN_SEMICOLON);
                }
                else if(c == EOF) {
                    return new Token(TOKEN_EOF);
                }
            }
        }
    }
}


void Lexer::skipWhitespaces(std::ifstream& file, char& c) {
    while (c == ' ') {
        file.get(c);
    }
}

Token* Lexer::getID(std::ifstream &file, char &c) {
    std::string value;

    while (isalpha(c) || c == '_') {
        value += c;
        file.get(c);
    }

    if (!value.compare("int")) {
        return new Token(TOKEN_INT);
    }

    return new Token(TOKEN_ID, value);

}
//<-------->
//        Parsez, la fiecare cuvant tb sa vad ce este. Parsez token(getNext) -> skip whitespaces, parse again
//cum arata o posibilitate de cod?
//
//anastasia: int;
//anastasia = constant;
//anastasia = expr ( token operator token)
//fun test(): type {
//    code block
//}