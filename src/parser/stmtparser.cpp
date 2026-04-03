#include "stmtparser.hpp"

#include <vector>

#include "token.hpp"

StmtParser::StmtParser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), exprparser(this->tokens, index) {}

Token StmtParser::peek() {
    if (index < tokens.size()) {
        return tokens[index];
    } else {
        return tokens.back();  // EOF Token
    }
}

Token StmtParser::peekNext() {
    if (index + 1 < tokens.size()) {
        return tokens[index + 1];
    } else {
        return tokens.back();  // EOF Token
    }
}

void StmtParser::advance() {
    if (index < tokens.size()) {
        index++;
    }
}

std::unique_ptr<ProgramNode> StmtParser::parseProgram() {
    auto program = std::make_unique<ProgramNode>();

    while (peek().type != TokenType::EOF_TOKEN) {
        // skip empty lines
        if (peek().type == TokenType::NEWLINE) {
            advance();
            continue;
        }

        program->statements.push_back(parseStatement());
    }

    return program;
}

std::unique_ptr<ASTStmtNode> StmtParser::parseStatement() {
    // TODO: Implement print, if, while and for

    if (peek().type == TokenType::IF)
        return parseIfStatement();

    if (peek().type == TokenType::WHILE)
        return parseWhileStatement();

    if (peek().type == TokenType::FOR)
        return parseForStatement();

    if (peek().type == TokenType::IDENTIFIER && peekNext().type == TokenType::ASSIGN){
        return parseVarDeclaration();
    } else {
        ignoreExpressionStatement();  // fallback
        advance();
        return nullptr;
    }
}

std::unique_ptr<ASTStmtNode> StmtParser::parseVarDeclaration() {
    Token nameToken = peek();
    advance();  // variable name
    advance();  // =

    auto value = exprparser.parseExpr();  // RHS

    if (peek().type == TokenType::NEWLINE)
        advance();

    auto nameNode = std::make_unique<ReferenceNode>(nameToken.value);

    // return the node
    return std::make_unique<VarDeclNode>(std::move(nameNode), std::move(value));
}

// TODO: Think if just ignoring an expression statement is ok,
// how to make it better
void StmtParser::ignoreExpressionStatement() {}
