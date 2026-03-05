#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "token.hpp"

inline const std::unordered_map<std::string, TokenType> keywords = {
        {"def",     TokenType::DEF},
        {"return",  TokenType::RETURN},
        {"for",     TokenType::FOR},
        {"while",   TokenType::WHILE},
        {"break",   TokenType::BREAK},
        {"continue",TokenType::CONTINUE},
        {"pass",    TokenType::PASS},
        {"if",      TokenType::IF},
        {"elif",    TokenType::ELIF},
        {"else",    TokenType::ELSE},
        {"print",   TokenType::PRINT},
        {"=",       TokenType::ASSIGN},
        {"+",       TokenType::PLUS},
        {"-",       TokenType::MINUS},
        {"*",       TokenType::STAR},
        {"/",       TokenType::SLASH},
        {"(",       TokenType::LPAREN},
        {")",       TokenType::RPAREN},
        {":",       TokenType::COLON},
        {",",       TokenType::COMMA},
        {"\n",      TokenType::NEWLINE},
        {"\t",      TokenType::INDENT},
        {"and",     TokenType::AND},
        {"or",      TokenType::OR},
        {"not",     TokenType::NOT},
        {"in",      TokenType::IN},
        {"is",      TokenType::IS},
        {"False",   TokenType::FALSE},
        {"True",    TokenType::TRUE},
        {"None",    TokenType::NONE},
        {">",       TokenType::GREATERTHAN},
        {"<",       TokenType::LESSERTHAN},
        {"%",       TokenType::MODULO},
    };
