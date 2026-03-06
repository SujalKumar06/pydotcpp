#pragma once

#include <vector>
#include <string>
#include <memory>

#include "token.hpp"
#include "astexpr.hpp"

class ExprParser {
public:
    ExprParser(std::vector<Token> tokens);
    std::unique_ptr<ASTExprNode> parseExpr();

private:
    //precedence parsing(from highest precedence to lowest precedence)
    std::unique_ptr<ASTExprNode> parsePrimary();
    std::unique_ptr<ASTExprNode> parsePower();
    std::unique_ptr<ASTExprNode> parseUnary();
    std::unique_ptr<ASTExprNode> parseMultiplicative();
    std::unique_ptr<ASTExprNode> parseAdditive();
    std::unique_ptr<ASTExprNode> parseComparison();
    std::unique_ptr<ASTExprNode> parseLogicalNot();
    std::unique_ptr<ASTExprNode> parseLogicalAnd();
    std::unique_ptr<ASTExprNode> parseLogicalOr();

    //helpers
    Token peek();
    Token peekNext();
    void advance();
    bool match(TokenType type);
    OperatorType toOperatorType(TokenType type);

    //index in the token vector
    int index;
};
