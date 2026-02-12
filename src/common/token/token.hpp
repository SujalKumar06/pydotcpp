<<<<<<< HEAD
#pragma once

#include <string>

enum class TokenType {
    DEF,
    RETURN,
    IF,
    ELIF,
    ELSE,
    FOR,
    WHILE,
    BREAK,
    CONTINUE,
    PASS,
    TRUE,
    FALSE,
    NONE,
    AND,
    OR,
    NOT,
    IN,
    IS,

    IDENTIFIER,
    NUMBER,
    STRING,

    PLUS, // +
    MINUS, // -
    STAR, // *
    SLASH, // /
    FLOORDIV, // //
    POWER, // **
    ASSIGN, // =
    MODULO, // %
    GREATERTHAN, // >
    LESSERTHAN, // <
    GREATEREQUAL, // >=
    LESSEQUAL, // <=
    EQEQUAL, // ==
    NOTEQUAL, // !=
    PLUSEQUAL, // +=
    MINUSEQUAL, // -=
    STAREQUAL, // *=
    SLASHEQUAL, // /=
    FLOOREQUAL, // //=
    POWEREQUAL, // **=
    MODULOEQUAL, // %=

    LPAREN, // (
    RPAREN, // )

    NEWLINE,
    EOF_TOKEN,

    COMMA, // ,
    COLON, // :
    INDENT,
    DEDENT,
    COMMENT, // #
    PRINT,
    AMPERSAND, // &
    PIPE, // |
    XOR,
    SPACE,
};


class Token {
public:
    // Main part of a token
    TokenType type;
    std::string value;

    // Used to tell user about error in case error is found
    int line;
    int column;

    // This constructor will allow us to easily make the tokens while coding
    Token(TokenType type, std::string val, int l, int c);
};
=======
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





>>>>>>> b754426 (filled token.hpp and lexer.hpp with basic declarations(from ankit's commit))
