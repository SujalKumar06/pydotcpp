#include <token.hpp>
#include <keywords.hpp>
#include <lexer.hpp>
#include <string>
#include <stdexcept>

Lexer::Lexer(std::string input_string) : source_code(input_string){}
bool Lexer::isAtEnd(){
    if (current_index >= source_code.size()){
        return true;
    }
    return false;
}
char Lexer::peek(){
    if (current_index >= source_code.size()){
        return '\0';
    }
    return source_code[current_index];
}

char Lexer::peekNext(){
    if (current_index+1 >= source_code.size()){
        return '\0';
    }
    return source_code[current_index+1];
}

char Lexer::advance(){
    if (current_index >= source_code.size()){
        return '\0';
    }
    char c = source_code[current_index++];
    if (c == '\n'){
        line++;
        column=1;
    }
    else {column++;}
    return c;
}

void Lexer::scanNumber(std::string curr){
    int start = column-1;
    while (std::isdigit(peek())){
        curr += Lexer::advance();
    }
    if (std::isalnum(peek())){
        // throw an error
    }
    Token token(TokenType::NUMBER, curr, line, start);
    tokens.push_back(token);
}

void Lexer::scanString(std::string quote){
    int start = column-1;
    while (!isAtEnd() && peek()!=quote[0] && peek()!='\n'){
        quote += advance();
    }
    if (isAtEnd()){
        // throw an error
    }
    else if (peek()=='\n'){
        // throw an error
    }
    else{
        quote += advance();
    }
    Token token(TokenType::STRING, quote, line, start);
    tokens.push_back(token);
}

void Lexer::scanIdentifier(std::string curr){
    int start = column-1;
    while (std::isalnum(peek()) || peek()=='_'){
        curr += advance();
    }
    Token token(TokenType::IDENTIFIER, curr, line, start);
    if (keywords.count(curr)){
        token.type = keywords.at(curr);
    }
    tokens.push_back(token);
}


std::vector<Token> Lexer::scan_Tokens(){
    while (true){
    if (isAtEnd()){
        Token token(TokenType::EOF_TOKEN, "", line, column);
        tokens.push_back(token);
        break;
    }
    std::string curr = "";
    curr += advance();
    if (keywords.count(curr)){
        TokenType type = keywords.at(curr);
        if (type == TokenType::NEWLINE){
            column = 0;
            line++;
        }
        else if (type == TokenType::INDENT){
            column += 3;
        }
        else if (type == TokenType::DEDENT){
            column -= 5;
        }
        Token token(type, curr, line, column);
        tokens.push_back(token);
    }
    else{     
        if (std::isdigit(curr[0])){
            scanNumber(curr);
        }
        else if (std::isalpha(curr[0])){
            scanIdentifier(curr);
        }
    } 
}
}
