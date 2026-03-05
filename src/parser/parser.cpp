#include "exprparser.hpp"
#include "ast.hpp"
#include "token.hpp"
#include <vector>
#include <memory>

class Parser {
public:
    Parser(std::vector<Token> tokens) : tokens(tokens), index(0) {}
    
    std::unique_ptr<ProgramNode> parseProgram() {
        //make a new program node and then push all the other tokens into the statements vector of
        //this node after converting everything into proper ASTNodes using the parseStatements command
        auto program = std::make_unique<ProgramNode>();

        while (index < tokens.size() && peek().type != TokenType::EOF_TOKEN) {
            program->statements.push_back(parseStatement());
        }
        return program;
    }

private:
    std::vector<Token> tokens;  //the tokens vector that is returned by the lexer
    int index; //where are we in the token's vector

    Token peek() {
        if (index < tokens.size()){ //check if the index is still smaller than the size of tokens
            return tokens[index];   //return the value of the token in that index if yes
        }
        else{
            return tokens.back();   //Just give the last element, typically EOF
        }
    }
    void advance() { if (index < tokens.size()) index++; }

    std::unique_ptr<ASTNode> parseStatement() {
        //Returns a unique pointer to a new ASTNode created which is of the type
        //of the value that was sent to it
        switch (peek().type) {
            case TokenType::PRINT: return parsePrintStatement();
            case TokenType::IDENTIFIER: return parseVarDeclaration();
            case TokenType::IF: return parseIfStatement();
            case TokenType::WHILE: return parseWhileStatement();
            case TokenType::FOR: return parseForStatement();
            default: return parseExpressionStatement();
        }
    }

    std::unique_ptr<ASTNode> parsePrintStatement() {
        advance(); 
        auto expr = parseExpressionStatement(); 
        return std::make_unique<PrintStmtNode>(std::move(expr));
    }

    std::unique_ptr<ASTNode> parseVarDeclaration() {
        //we will make a VARDECL ASTNode, where we will send the first value IDENTIFIER, as the name
        //and the thing in front of the '=' will be send to parseExpressionStatement to get its result
        //then we will send that result to the previously made VARDECL as the value
    }

    std::unique_ptr<ASTNode> parseIfStatement() {
        //parse if, elif and else statements
        //the elif and else statements will be attached to the IfNode
        //We then return the IfNode
    }

    std::unique_ptr<ASTNode> parseWhileStatement() {
        //parse while statement
        //NOTE: Haven't added ASTNodes for this yet, I'll add it in a few hours, dinner time
    }

    std::unique_ptr<ASTNode> parseForStatement() {
        //parse for statement
        //NOTE: Haven't added ASTNodes for this yet, I'll add it in a few hours, dinner time
    }

    std::unique_ptr<ASTNode> parseExpressionStatement() {
        //have the exprparser made in exprparser.cpp return the final value of this statement
    }
    
    Token peekNext() {
        if (index+1 < tokens.size()) return tokens[index + 1];
        return tokens.back();
    }
};