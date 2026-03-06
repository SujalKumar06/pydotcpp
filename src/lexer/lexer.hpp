#pragma once

#include <unordered_map>
#include <string>
#include <vector>
#include <cctype> //to check if char is alphanumeric
#include <stack>

#include "token.hpp"

class Lexer {
public:
    Lexer(std::string input_string);
    std::vector<Token> scan_Tokens();
    

private:
    std::string source_code;
    std::vector<Token> tokens;
    int start = 0;
    int current_index = 0;
    int line = 1;
    int column = 1;
    std::stack<int> indent_stack;


    // FUNCTIONS NEEDED FOR LEXER TO WORK
    bool isAtEnd() const;         // Checks for last character
    char advance();         // Return current char and move forward
    char peek() const;            // Sometimes, we don't actually want to read a character and may only want to peek at it
    char peekNext() const;        // Peak at the next character

    void addToken(TokenType type);

    // Specific scanners for complex types
    void scanString(std::string first);
    void scanNumber(std::string first);
    void scanIdentifier(std::string first);
    void processIndent();   // To process indents at the start of every line
};
