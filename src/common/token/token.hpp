#pragma once

#include <string>

enum class TokenType {
    DEF,
    RETURN,
    IF,
    ELSE,
    FOR,
    WHILE,

    IDENTIFIER,
    NUMBER,
    STRING,

    PLUS, // +
    MINUS, // -
    STAR, // *
    SLASH, // /
    ASSIGN, // =
    MODULO, // %
    GREATERTHAN, // >
    LESSERTHAN, // <

    LPAREN, // (
    RPAREN, // )

    NEWLINE,
    EOF_TOKEN,

    COMMA, // ,
    COLON, // :
    INDENT,
    DEDENT,
    COMMENT // #
};


class Token{
public:
    // Main part of a token
    TokenType type;
    std::string value;

    // Used to tell user about error in case error is found
    int line;
    int column;

    // This constructor will allow us to easily make the tokens while coding
    Token(TokenType type, std::string val, int l, int c)
};





