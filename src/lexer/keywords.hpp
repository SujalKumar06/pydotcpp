#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "token.hpp"

inline const std::unordered_map<std::string, TokenType> keywords = {
    {"def",      TokenType::DEF},
    {"return",   TokenType::RETURN},
    {"if",       TokenType::IF},
    {"elif",     TokenType::ELIF},
    {"else",     TokenType::ELSE},
    {"for",      TokenType::FOR},
    {"while",    TokenType::WHILE},
    {"break",    TokenType::BREAK},
    {"continue", TokenType::CONTINUE},
    {"pass",     TokenType::PASS},
    {"True",     TokenType::TRUE},
    {"False",    TokenType::FALSE},
    {"None",     TokenType::NONE},
    {"and",      TokenType::AND},
    {"or",       TokenType::OR},
    {"not",      TokenType::NOT},
    {"in",       TokenType::IN},
    {"is",       TokenType::IS},
    {"print",    TokenType::PRINT},
};

inline const std::unordered_map<std::string, TokenType> operators = {
    {"+",  TokenType::PLUS},
    {"-",  TokenType::MINUS},
    {"*",  TokenType::STAR},
    {"/",  TokenType::SLASH},
    {"//", TokenType::FLOORDIV},
    {"**", TokenType::POWER},
    {"=",  TokenType::ASSIGN},
    {"%",  TokenType::MODULO},
    {">",  TokenType::GREATERTHAN},
    {"<",  TokenType::LESSERTHAN},
    {">=", TokenType::GREATEREQUAL},
    {"<=", TokenType::LESSEQUAL},
    {"==", TokenType::EQEQUAL},
    {"!=", TokenType::NOTEQUAL},
    {"+=", TokenType::PLUSEQUAL},
    {"-=", TokenType::MINUSEQUAL},
    {"*=", TokenType::STAREQUAL},
    {"/=", TokenType::SLASHEQUAL},
    {"//=", TokenType::FLOOREQUAL},
    {"**=", TokenType::POWEREQUAL},
    {"%=",  TokenType::MODULOEQUAL},
    {"&",  TokenType::AMPERSAND},
    {"|",  TokenType::PIPE},
    {"^", TokenType::XOR},
};

inline const std::unordered_map<std::string, TokenType> delimiters = {
    {"(",  TokenType::LPAREN},
    {")",  TokenType::RPAREN},
    {"[", TokenType::LBRACKET},
    {"]", TokenType::RBRACKET},
    {",",  TokenType::COMMA},
    {":",  TokenType::COLON},
    {"\n", TokenType::NEWLINE},
};
