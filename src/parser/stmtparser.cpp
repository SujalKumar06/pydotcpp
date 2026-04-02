#include "stmtparser.hpp"

#include <stdexcept>
#include <vector>

#include "token.hpp"

StmtParser::StmtParser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), exprparser(this->tokens, index) {}

Token StmtParser::peek() {
    if (index < tokens.size())
        return tokens[index];
    return tokens.back();  // EOF
}

Token StmtParser::peekNext() {
    if (index + 1 < tokens.size())
        return tokens[index + 1];
    return tokens.back();
}

void StmtParser::advance() {
    if (index < tokens.size())
        index++;
}

std::unique_ptr<ProgramNode> StmtParser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();

    while (peek().type != TokenType::EOF_TOKEN) {
        if (peek().type == TokenType::NEWLINE) {
            advance();
            continue;
        }

        program->statements.push_back(parseStatement());
    }

    return program;
}

std::unique_ptr<ASTStmtNode> StmtParser::parseStatement() {
    if (peek().type == TokenType::PRINT)
        return parsePrintStatement();

    if (peek().type == TokenType::IF)
        return parseIfStatement();

    if (peek().type == TokenType::WHILE)
        return parseWhileStatement();

    if (peek().type == TokenType::FOR)
        return parseForStatement();

    if (peek().type == TokenType::IDENTIFIER && peekNext().type == TokenType::ASSIGN)
        return parseVarDeclaration();

    return parseExpressionStatement();
}

std::unique_ptr<ASTStmtNode> StmtParser::parseVarDeclaration() {
    Token nameToken = peek();
    advance();  // IDENTIFIER

    advance();  // ASSIGN

    auto value = exprparser.parseExpr();

    if (peek().type == TokenType::NEWLINE)
        advance();

    auto nameNode = std::make_unique<ReferenceNode>(nameToken.value);

    return std::make_unique<VarDeclNode>(std::move(nameNode), std::move(value));
}

std::unique_ptr<ASTStmtNode> StmtParser::parsePrintStatement() {
    advance();  // PRINT

    if (peek().type != TokenType::LPAREN)
        throw std::runtime_error("Expected '(' after print");

    advance();  // (

    auto expr = exprparser.parseExpr();

    if (peek().type != TokenType::RPAREN)
        throw std::runtime_error("Expected ')'");

    advance();  // )

    if (peek().type == TokenType::NEWLINE)
        advance();

    return std::make_unique<PrintStmtNode>(std::move(expr));
}

std::unique_ptr<ASTStmtNode> StmtParser::parseExpressionStatement() {
    auto expr = exprparser.parseExpr();

    if (peek().type == TokenType::NEWLINE)
        advance();

    return std::make_unique<PrintStmtNode>(std::move(expr));
}

std::unique_ptr<ASTStmtNode> StmtParser::parseIfStatement() {
    advance();  // IF

    auto condition = exprparser.parseExpr();

    if (peek().type != TokenType::COLON)
        throw std::runtime_error("Expected ':' after if condition");

    advance();  // :

    if (peek().type != TokenType::NEWLINE)
        throw std::runtime_error("Expected newline");

    advance();  // NEWLINE

    if (peek().type != TokenType::INDENT)
        throw std::runtime_error("Expected INDENT");

    advance();  // INDENT

    auto block = std::make_unique<BlockNode>();

    while (peek().type != TokenType::DEDENT) {
        block->statements.push_back(parseStatement());
    }

    advance();  // DEDENT

    return std::make_unique<IfStmtNode>(std::move(condition), std::move(block));
}

std::unique_ptr<ASTStmtNode> StmtParser::parseWhileStatement() {
    advance();  // WHILE

    auto condition = exprparser.parseExpr();

    if (peek().type != TokenType::COLON)
        throw std::runtime_error("Expected ':'");

    advance();  // :

    if (peek().type != TokenType::NEWLINE)
        throw std::runtime_error("Expected newline");

    advance();  // NEWLINE

    if (peek().type != TokenType::INDENT)
        throw std::runtime_error("Expected INDENT");

    advance();  // INDENT

    auto block = std::make_unique<BlockNode>();

    while (peek().type != TokenType::DEDENT) {
        block->statements.push_back(parseStatement());
    }

    advance();  // DEDENT

    return std::make_unique<WhileStmtNode>(std::move(condition), std::move(block));
}

std::unique_ptr<ASTStmtNode> StmtParser::parseForStatement() {
    throw std::runtime_error("FOR not implemented yet");
}
