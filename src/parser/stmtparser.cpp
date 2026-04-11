#include "stmtparser.hpp"

#include <stdexcept>
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

std::unique_ptr<ASTStmtNode> StmtParser::parseBlock() {
    auto block = std::make_unique<BlockNode>();
    if (peek().type != TokenType::INDENT) {  // A block must start with an INDENT
        throw std::runtime_error("Expected indentation block");
    }
    advance();
    // Keep parsing statements until we hit a DEDENT or EOF
    while (peek().type != TokenType::DEDENT && peek().type != TokenType::EOF_TOKEN) {
        // Skip empty newlines
        if (peek().type == TokenType::NEWLINE) {
            advance();
            continue;
        }
        auto stmt = parseStatement();
        if (stmt) {  // Check that there is code - stmt is not a nullptr
            block->statements.push_back(
                std::move(stmt));  // Store all statements as a vector in the block
        }
    }
    // A block must close with a DEDENT
    if (peek().type == TokenType::DEDENT) {
        advance();
    }
    return block;
}

std::unique_ptr<ASTStmtNode> StmtParser::parseIfStatement() {
    advance();  // Consume the 'if' (or 'elif') token
    // Parse the condition using ExprParser
    auto condition = exprparser.parseExpr();
    //  Consume the required ':' and NEWLINE
    if (peek().type != TokenType::COLON) {
        throw std::runtime_error("Expected ':' after condition");
    }
    advance();  // Consume ':'
    if (peek().type == TokenType::NEWLINE) {
        advance();  // Consume NEWLINE before the block
    }
    // Parse the indented block
    auto block = parseBlock();
    // Create the IfNode (we will handle elif/else below)
    auto ifNode = std::make_unique<IfStmtNode>(std::move(condition), std::move(block));
    // Check for 'elif'
    if (peek().type == TokenType::ELIF) {
        // Recursively call parseIfStatement.
        ifNode->elif_ptr = parseIfStatement();  // Can use same logic as done for if
    }
    // Check for 'else'
    else if (peek().type == TokenType::ELSE) {
        advance();  // Consume 'else'
        if (peek().type != TokenType::COLON) {
            throw std::runtime_error("Expected ':' after else");
        }
        advance();  // Consume ':'
        if (peek().type == TokenType::NEWLINE) {
            advance();  // Consume NEWLINE
        }
        // Parse the else block and wrap it in an ElseStmtNode
        auto elseBlock   = parseBlock();
        ifNode->else_ptr = std::make_unique<ElseStmtNode>(std::move(elseBlock));
    }
    return ifNode;
}

std::unique_ptr<ASTStmtNode> StmtParser::parseWhileStatement() {
    advance();
    auto condition = exprparser.parseExpr();
    if (peek().type != TokenType::COLON) {
        throw std::runtime_error("Expected ':' after condition");
    }
    advance();
    if (peek().type == TokenType::NEWLINE) {
        advance();
    }
    auto block = parseBlock();

    auto whileNode = std::make_unique<WhileStmtNode>(std::move(condition), std::move(block));

    return whileNode;
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

    if (peek().type == TokenType::PRINT) {
        return parsePrintStatement();
    }
    // } else if (peek().type == TokenType::FOR) {
    //     return parseForStatement();
    else if (peek().type == TokenType::IDENTIFIER && peekNext().type == TokenType::ASSIGN) {
        return parseVarDeclaration();
    } else if (peek().type == TokenType::IF) {
        return parseIfStatement();
    } else if (peek().type == TokenType::WHILE) {
        return parseWhileStatement();
    } else if (peek().type == TokenType::BREAK) {
        advance();
        if (peek().type == TokenType::NEWLINE) {
            advance();
        }
        return std::make_unique<BreakStmtNode>();
    } else if (peek().type == TokenType::CONTINUE) {
        advance();
        if (peek().type == TokenType::NEWLINE) {
            advance();
        }
        return std::make_unique<ContinueStmtNode>();
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

std::unique_ptr<ASTStmtNode> StmtParser::parsePrintStatement() {
    advance();
    auto expr = exprparser.parseExpr();
    if (peek().type == TokenType::NEWLINE) {
        advance();
    }
    return std::make_unique<PrintStmtNode>(std::move(expr));
}

// TODO: Think if just ignoring an expression statement is ok,
// how to make it better
void StmtParser::ignoreExpressionStatement() {}
