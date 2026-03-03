#pragma once

#include <vector>
#include <string>
#include <memory>

#include "token.hpp"
#include "ast.hpp"

class ExprParser {
public:
    ExprParser(std::vector<Token> tokens);
    std::unique_ptr<ASTNode> parseExpr();
    std::unique_ptr<ASTNode> parseExprStmt();

private:
    //precedence parsing(from highest precedence to lowest precedence)
    std::unique_ptr<ASTNode> parsePrimary();
    std::unique_ptr<ASTNode> parsePower();
    std::unique_ptr<ASTNode> parseUnary();
    std::unique_ptr<ASTNode> parseMultiplicative();
    std::unique_ptr<ASTNode> parseAdditive();
    std::unique_ptr<ASTNode> parseComparison();
    std::unique_ptr<ASTNode> parseLogicalNot();
    std::unique_ptr<ASTNode> parseLogicalAnd();
    std::unique_ptr<ASTNode> parseLogicalOr();
    std::unique_ptr<ASTNode> parseAssignment();

    //helpers
    Token peek();
    Token peekNext();
    void advance();
    bool match(TokenType type);
    OperatorType toOperatorType(TokenType type);

    //index in the token vector
    int index;
};
