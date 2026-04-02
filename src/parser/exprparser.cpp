#include "exprparser.hpp"

#include <stdexcept>

ExprParser::ExprParser(std::vector<Token> &tokens, int &index)
    : tokens(tokens), index(index) {}

Token ExprParser::peek() {
  if (index < tokens.size())
    return tokens[index];
  else
    return tokens.back(); // EOF token
}

Token ExprParser::peekNext() {
  if (index + 1 < tokens.size())
    return tokens[index + 1];
  else
    return tokens.back(); // EOF token
}

void ExprParser::advance() {
  if (index < tokens.size())
    index++;
}

bool ExprParser::match(TokenType type) { return peek().type == type; }

OperatorType ExprParser::toOperatorType(TokenType type) {
  switch (type) {
  // comparison
  case TokenType::GREATERTHAN:
    return OperatorType::GREATERTHAN;
  case TokenType::LESSTHAN:
    return OperatorType::LESSTHAN;
  case TokenType::GREATEREQUAL:
    return OperatorType::GREATEREQUAL;
  case TokenType::LESSEQUAL:
    return OperatorType::LESSEQUAL;
  case TokenType::EQEQUAL:
    return OperatorType::EQEQUAL;
  case TokenType::NOTEQUAL:
    return OperatorType::NOTEQUAL;

  // additive
  case TokenType::PLUS:
    return OperatorType::PLUS;
  case TokenType::MINUS:
    return OperatorType::MINUS;

  // multiplicative
  case TokenType::STAR:
    return OperatorType::STAR;
  case TokenType::SLASH:
    return OperatorType::SLASH;
  case TokenType::DOUBLESLASH:
    return OperatorType::DOUBLESLASH;
  case TokenType::MODULO:
    return OperatorType::MODULO;

  default:
    // never supposed to happen
    throw std::runtime_error("unexpected operator");
  }
}

std::unique_ptr<ASTExprNode> ExprParser::parseExpr() {
  return parseLogicalOr();
}

std::unique_ptr<ASTExprNode> ExprParser::parseLogicalOr() {
  auto lhs = parseLogicalAnd();

  // left associative
  while (match(TokenType::OR)) {
    advance();
    auto rhs = parseLogicalAnd();
    lhs = std::make_unique<BinaryOperatorNode>(OperatorType::OR, std::move(lhs),
                                               std::move(rhs));
  }

  return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseLogicalAnd() {
  auto lhs = parseLogicalNot();

  // left associative
  while (match(TokenType::AND)) {
    advance();
    auto rhs = parseLogicalNot();
    lhs = std::make_unique<BinaryOperatorNode>(OperatorType::AND,
                                               std::move(lhs), std::move(rhs));
  }

  return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseLogicalNot() {
  // right associative
  if (match(TokenType::NOT)) {
    advance();
    auto rhs = parseLogicalNot();
    return std::make_unique<UnaryOperatorNode>(OperatorType::NOT,
                                               std::move(rhs));
  }

  return parseComparison();
}

std::unique_ptr<ASTExprNode> ExprParser::parseComparison() {
  auto lhs = parseAdditive();

  // left associative
  while (match(TokenType::GREATERTHAN) || match(TokenType::LESSTHAN) ||
         match(TokenType::GREATEREQUAL) || match(TokenType::LESSEQUAL) ||
         match(TokenType::EQEQUAL) || match(TokenType::NOTEQUAL)) {
    TokenType type = peek().type;
    advance();
    auto rhs = parseAdditive();
    lhs = std::make_unique<BinaryOperatorNode>(toOperatorType(type),
                                               std::move(lhs), std::move(rhs));
  }

  return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseAdditive() {
  auto lhs = parseMultiplicative();

  // left associative
  while (match(TokenType::PLUS) || match(TokenType::MINUS)) {
    TokenType type = peek().type;
    advance();
    auto rhs = parseMultiplicative();
    lhs = std::make_unique<BinaryOperatorNode>(toOperatorType(type),
                                               std::move(lhs), std::move(rhs));
  }

  return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseMultiplicative() {
  auto lhs = parseUnary();

  // left associative
  while (match(TokenType::STAR) || match(TokenType::SLASH) ||
         match(TokenType::DOUBLESLASH) || match(TokenType::MODULO)) {
    TokenType type = peek().type;
    advance();
    auto rhs = parseUnary();
    lhs = std::make_unique<BinaryOperatorNode>(toOperatorType(type),
                                               std::move(lhs), std::move(rhs));
  }

  return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseUnary() {
  // right associative
  if (match(TokenType::PLUS) || match(TokenType::MINUS)) {
    TokenType type = peek().type;
    advance();
    auto rhs = parseUnary();
    return std::make_unique<UnaryOperatorNode>(toOperatorType(type),
                                               std::move(rhs));
  }

  return parsePower();
}

std::unique_ptr<ASTExprNode> ExprParser::parsePower() {
  auto lhs = parsePrimary();

  // right associative
  if (match(TokenType::POWER)) {
    advance();
    auto rhs = parseUnary();
    return std::make_unique<BinaryOperatorNode>(OperatorType::POWER,
                                                std::move(lhs), std::move(rhs));
  }

  return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parsePrimary() {
  if (match(TokenType::LPAREN)) {
    advance();
    auto expr = parseExpr();

    if (!match(TokenType::RPAREN)) {
      throw std::runtime_error("expected closing parenthesis");
    }
    advance();

    return expr;
  }

  Token token = peek();
  advance();
  switch (token.type) {
  case TokenType::NUMBER: {
    double value = std::stod(token.value);
    return std::make_unique<NumberNode>(value);
  }
  case TokenType::STRING:
    return std::make_unique<StringNode>(std::move(token.value));
  case TokenType::TRUE:
    return std::make_unique<BooleanNode>(true);
  case TokenType::FALSE:
    return std::make_unique<BooleanNode>(false);
  case TokenType::NONE:
    return std::make_unique<NoneNode>();
  case TokenType::REFERENCE:
    return std::make_unique<ReferenceNode>(std::move(token.value));
  default:
    throw std::runtime_error("invalid token found");
  }
}