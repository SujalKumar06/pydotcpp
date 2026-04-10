#include <cmath>  // for exponentiation in scientific notation
#include <keywords.hpp>
#include <lexer.hpp>
#include <string>
#include <token.hpp>

Lexer::Lexer(std::string input_string) : source_code(input_string) {
    indent_stack.push(0);  // stack initially must have 0
}
bool Lexer::isAtEnd() const {  // checks if we are at the end of source code
    if (current_index >= source_code.size()) {
        return true;
    }
    return false;
}

char Lexer::peek() const {  // returns character at current index
    if (isAtEnd()) {
        return '\0';
    }
    return source_code[current_index];
}

char Lexer::peekNext() const {  // returns character at next index
    if (current_index + 1 >= source_code.size()) {
        return '\0';
    }
    return source_code[current_index + 1];
}

char Lexer::advance() {  // returns character at current index and
    char c = peek();
    current_index++;
    if (c == '\n') {
        line++;
        column = 1;
    } else {
        column++;
    }
    return c;
}

void Lexer::processIndent() {  // maintains an indent stack and adds indent and dedent tokens
                               // wherever necessary
    int indent = 0;

    while (peek() == ' ' || peek() == '\t') {  // handles spaces and tabs
        switch (peek()) {
            case '\t':
                indent += 4;
                break;
            default:
                indent += 1;
        }
        advance();
    }
    if (peek() == '\n' || peek() == '\0' || peek() == '#')
        return;  // empty line, EOF, comments don't affect indentation

    if (indent > indent_stack.top()) {  // current indent is larger; add indent token
        indent_stack.push(indent);
        Token token(TokenType::INDENT, "", line, 1);
        tokens.push_back(token);
    } else if (indent < indent_stack.top()) {  // add dedent tokens until indentations match
        while (!indent_stack.empty() &&
               indent_stack.top() != indent) {  // current indent must be present elsewhere in the
            indent_stack.pop();                 // stack, else it is an error
            Token token(TokenType::DEDENT, "", line, 1);
            tokens.push_back(token);
        }
        if (indent_stack.empty()) {
            throw std::runtime_error("invalid indentation");
        }
    }
}

void Lexer::scanNumber(std::string num) {
    int start = column - 1;  // column of the number token

    if (num == "0" && std::isdigit(peek())) {
        throw std::runtime_error("leading zeros are not allowed");
        
    }

    while (std::isdigit(peek())) {
        num += Lexer::advance();
    }
    if (peek() == '.' && num[0] == '.') {
        throw std::runtime_error("we can have .23 and 23.23, but .23.23 is error");
    }
    if (peek() == '.' && std::isdigit(peekNext())) {  // for floating point numbers
        num += advance();
        while (std::isdigit(peek())) {
            num += advance();
        }
    }

    if ((peek() == 'e' || peek() == 'E')) {  // scientific notation
        num += advance();
        if (!std::isdigit(peek()) && peek() != '+' && peek() != '-') {
            throw std::runtime_error("invalid syntax");
        }
        // power can only be an integer in scientific notation in python
        if (peek() == '+' || peek() == '-') {
            if (!std::isdigit(peekNext())) {
                throw std::runtime_error("invalid syntax");
            }
            num += advance();
        }
        while (std::isdigit(peek())) {
            num += advance();
        }
    }

    if (std::isalnum(peek()) || peek() == '_' || peek() == '.') {
        throw std::runtime_error("invalid floating point number");
    }

    Token token(TokenType::NUMBER, num, line, start);
    tokens.push_back(token);
}

void Lexer::scanString(std::string str) {
    int start = column - 1;
    while (!isAtEnd() && peek() != str[0] && peek() != '\n') {
        str += advance();
    }
    if (isAtEnd()) {
        throw std::runtime_error("unterminated string - reached endOfFile");
    } else if (peek() == '\n') {
        throw std::runtime_error("unterminated string - reached newLine");
    } else {
        str += advance();
    }
    Token token(TokenType::STRING, str, line, start);
    tokens.push_back(token);
}

void Lexer::scanIdentifier(std::string identifier) {
    int start = column - 1;
    while (std::isalnum(peek()) || peek() == '_') {
        identifier += advance();
    }
    Token token(TokenType::IDENTIFIER, identifier, line, start);
    if (keywords.count(identifier)) {
        token.type = keywords.at(identifier);
    }
    tokens.push_back(token);
}

std::vector<Token> Lexer::scan_Tokens() {
    while (true) {
        if (isAtEnd()) {
            // Give a \n before EOF
            if (tokens.empty() || tokens.back().type != TokenType::NEWLINE) {
                Token newline_token(TokenType::NEWLINE, "\n", line, column);
                tokens.push_back(newline_token);
                line += 1;
                column = 1;
            }
            // Fully dedent the stack before EOF
            while (indent_stack.size() > 1) {
                indent_stack.pop();
                Token dedent_token(TokenType::DEDENT, "", line, column);
                tokens.push_back(dedent_token);
            }
            Token token(TokenType::EOF_TOKEN, "", line, column);
            tokens.push_back(token);
            break;
        }  
        std::string curr = "";
        curr += advance();

        if (curr == " " || curr == "\t") {
            continue;  
        } 

        // handle two-character operators   
        if (curr == "=" && peek() == '=') {
            advance();
            Token token(TokenType::EQEQUAL, "==", line,
                        column - 2);  // using column-2 since we are advancing twice
            tokens.push_back(token);
            continue;
        }

        if (curr == ">" && peek() == '=') {
            advance();
            Token token(TokenType::GREATEREQUAL, ">=", line, column - 2);
            tokens.push_back(token);
            continue;
        }

        if (curr == "<" && peek() == '=') {
            advance();
            Token token(TokenType::LESSEQUAL, "<=", line, column - 2);
            tokens.push_back(token);
            continue;
        }

        if (curr == "+" && peek() == '=') {
            advance();
            Token token(TokenType::PLUSEQUAL, "+=", line, column - 2);
            tokens.push_back(token);
            continue;
        }

        if (curr == "-" && peek() == '=') {
            advance();
            Token token(TokenType::MINUSEQUAL, "-=", line, column - 2);
            tokens.push_back(token);
            continue;
        }

        if (curr == "*" && peek() == '=') {
            advance();
            Token token(TokenType::STAREQUAL, "*=", line, column - 2);
            tokens.push_back(token);
            continue;
        }

        if (curr == "/" && peek() == '=') {
            advance();
            Token token(TokenType::SLASHEQUAL, "/=", line, column - 2);
            tokens.push_back(token);
            continue;
        }

        if (curr == "%" && peek() == '=') {
            advance();
            Token token(TokenType::MODULOEQUAL, "%=", line, column - 2);
            tokens.push_back(token);
            continue;
        }

        if (curr == "/" && peek() == '/') {
            advance();
            if (peek() == '=') {
                advance();
                Token token(TokenType::FLOOREQUAL, "//=", line, column - 3);
                tokens.push_back(token);
            } else {
                Token token(TokenType::FLOORDIV, "//", line, column - 2);
                tokens.push_back(token);
            }
            continue;
        }

        if (curr == "*" && peek() == '*') {
            advance();
            if (peek() == '=') {
                advance();
                Token token(TokenType::POWEREQUAL, "**=", line, column - 3);
                tokens.push_back(token);
            } else {
                Token token(TokenType::POWER, "**", line, column - 2);
                tokens.push_back(token);
            }
            continue;
        }

        if (curr == "!") {
            if (peek() == '=') {
                advance();
                Token token(TokenType::NOTEQUAL, "!=", line, column - 2);
                tokens.push_back(token);
                continue;
            } else {
                throw std::runtime_error("invalid symbol - '!'");
            }
        }

        auto it = operators.find(
            curr);  // curr cannot be found in keywords at this point; it is a single character
        if ((it = operators.find(curr)) != operators.end()) {
            TokenType type = it->second;
            Token token(type, curr, line, column);
            tokens.push_back(token);
        } else if ((it = delimiters.find(curr)) != delimiters.end()) {
            TokenType type = it->second;

            if (type == TokenType::NEWLINE) {  // add newline token, and process indentation
                column = 1;
                Token token(type, curr, line - 1, column);
                tokens.push_back(token);
                processIndent();
            } else {
                Token token(type, curr, line, column);
                tokens.push_back(token);
            }
        } else {
            if (curr == "\"" || curr == "'") {
                scanString(curr);
            } else if (std::isdigit(curr[0]) ||
                       (curr[0] == '.' &&
                        std::isdigit(peek()))) {  // .23 is also valid syntax in python
                scanNumber(curr);
            } else if (std::isalpha(curr[0]) || curr[0] == '_') {  // _foo is also valid
                scanIdentifier(curr);
            } else if (curr[0] == '#') {  // comment, ignore everything until newline or EOF
                curr = "";
                while (peek() != '\n' && peek() != '\0') {
                    advance();
                }
            } else {
                throw std::runtime_error("unexpected character");
            }
        }
    }
    return tokens;
}
