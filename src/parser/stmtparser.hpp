#pragma once

#include <memory>
#include <vector>

#include "aststmt.hpp"
#include "exprparser.hpp"
#include "token.hpp"

class StmtParser {
   public:
    StmtParser(std::vector<Token> tokens);

    std::unique_ptr<ProgramNode> parseProgram();

   private:
    std::vector<Token> tokens;  // the tokens vector that is returned by the lexer
    int index = 0;              // where are we in the token's vector

    ExprParser exprparser;

    // helpers
    Token peek();
    Token peekNext();
    void advance();

    // statement parsing
    std::unique_ptr<ASTStmtNode> parseStatement();
    std::unique_ptr<ASTStmtNode> parsePrintStatement();
    std::unique_ptr<ASTStmtNode> parseVarDeclaration();
    std::unique_ptr<ASTStmtNode> parseFunctionDeclaration();
    std::unique_ptr<ASTStmtNode> parseBlock(); //helper function for indented blocks
    std::unique_ptr<ASTStmtNode> parseIfStatement();
    std::unique_ptr<ASTStmtNode> parseWhileStatement();
    std::unique_ptr<ASTStmtNode> parseForStatement();
    std::unique_ptr<ASTStmtNode> parseExpressionStatement();
};
