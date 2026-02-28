#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "token.hpp"

inline const std::unordered_map<std::string, TokenType> keywords = {
        {"def",    TokenType::DEF},
        {"return", TokenType::RETURN},
        {"if",     TokenType::IF},
        {"else",   TokenType::ELSE},
        {"print",  TokenType::PRINT},
        {"=",      TokenType::ASSIGN},
        {"+",      TokenType::PLUS},
        {"-",      TokenType::MINUS},
        {"*",      TokenType::STAR},
        {"/",      TokenType::SLASH},
        {"(",      TokenType::LPAREN},
        {")",      TokenType::RPAREN},
        {":",      TokenType::COLON},
        {",",      TokenType::COMMA},
        {"\n",     TokenType::NEWLINE},
        {"\t",     TokenType::INDENT},
        {" ",     TokenType::SPACE}
    };
