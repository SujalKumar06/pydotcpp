#include "stmtparser.hpp"
#include "token.hpp"
#include <vector>

StmtParser::StmtParser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), exprparser(this->tokens, index) {}

Token StmtParser::peek() {
  if (index < tokens.size()) {
    return tokens[index];
  } else {
    return tokens.back(); // EOF Token
  }
}

Token StmtParser::peekNext() {
  if (index + 1 < tokens.size()) {
    return tokens[index + 1];
  } else {
    return tokens.back(); // EOF Token
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
  if (peek().type == TokenType::PRINT) {
    return parsePrintStatement();
  } else if (peek().type == TokenType::IF) {
    return parseIfStatement();
  } else if (peek().type == TokenType::WHILE) {
    return parseWhileStatement();
  } else if (peek().type == TokenType::FOR) {
    return parseForStatement();
  } else if (peek().type == TokenType::REFERENCE &&
             peekNext().type == TokenType::ASSIGN) {
    return parseVarDeclaration();
  } else {
    return parseExpressionStatement(); // fallback
  }
}

std::unique_ptr<ASTStmtNode> StmtParser::parseVarDeclaration() {
  Token nameToken = peek();
  advance(); // variable name
  advance(); // =

  auto value = exprparser.parseExpr(); // RHS

  if (peek().type == TokenType::NEWLINE)
    advance();

  auto nameNode = std::make_unique<ReferenceNode>(nameToken.value);

  return std::make_unique<VarDeclNode>(
      std::move(nameNode),
      std::move(value) // ptr can't have more than one owner so we give the
                       // ownership to VarDeclNode
  );
}

std::unique_ptr<ASTStmtNode> StmtParser::parseExpressionStatement() {
  auto expr = exprparser.parseExpr();

  if (peek().type == TokenType::NEWLINE) {
    advance();
  }

  return std::make_unique<PrintStmtNode>(std::move(expr));
}

std::unique_ptr<ASTStmtNode> StmtParser::parsePrintStatement() {}