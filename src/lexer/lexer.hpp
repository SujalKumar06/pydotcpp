#pragma once

#include <unordered_map>
#include <string>
#include <vector>

#include "token.hpp"

class Lexer {
public:
    Lexer(std::string input_string);
    std::vector<Token> scan_Tokens(void);

private:
    std::string source_code;
    static inline const std::unordered_map<std::string, TokenType> keywords = {
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
        {"\t",     TokenType::INDENT}
    };
    std::vector<Token> tokens;
    int start = 0;
    int current_index = 0;
    int line = 1;


    // FUNCTIONS NEEDED FOR LEXER TO WORK
    bool isAtEnd(void);         // Checks for last character
    char advance(void);         // Return current char and move forward
    char peek(void);            // Sometimes, we don't actually want to read a character and may only want to peek at it
    char peekNext(void);        // Peak at the next character

    void addToken(TokenType type);

    // Specific scanners for complex types
    void scanString(void);
    void scanNumber(void);
    void scanIdentifier(void);
    std::string preprocess_indents(std::string raw);
};
