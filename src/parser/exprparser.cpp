#include "exprparser.hpp"

#include <cmath>
#include <stdexcept>

ExprParser::ExprParser(std::vector<Token>& tokens, int& index) : tokens(tokens), index(index) {}

Token ExprParser::peek() {
    if (index < tokens.size())
        return tokens[index];
    else
        return tokens.back();  // EOF token
}

Token ExprParser::peekNext() {
    if (index + 1 < tokens.size())
        return tokens[index + 1];
    else
        return tokens.back();  // EOF token
}

void ExprParser::advance() {
    if (index < tokens.size())
        index++;
}

bool ExprParser::match(TokenType type) {
    return peek().type == type;
}

OperatorType ExprParser::toOperatorType(TokenType type) {
    switch (type) {
        // comparison
        case TokenType::GREATERTHAN:
            return OperatorType::GREATERTHAN;
        case TokenType::LESSERTHAN:
            return OperatorType::LESSERTHAN;
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
        case TokenType::PLUSEQUAL:
            return OperatorType::PLUS;
        case TokenType::MINUS:
        case TokenType::MINUSEQUAL:
            return OperatorType::MINUS;

        // multiplicative
        case TokenType::STAR:
        case TokenType::STAREQUAL:
            return OperatorType::STAR;
        case TokenType::SLASH:
        case TokenType::SLASHEQUAL:
            return OperatorType::SLASH;
        case TokenType::FLOORDIV:
        case TokenType::FLOOREQUAL:
            return OperatorType::FLOORDIV;
        case TokenType::MODULO:
        case TokenType::MODULOEQUAL:
            return OperatorType::MODULO;

        // power
        case TokenType::POWER:
        case TokenType::POWEREQUAL:
            return OperatorType::POWER;

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
        lhs =
            std::make_unique<BinaryOperatorNode>(OperatorType::AND, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseLogicalAnd() {
    auto lhs = parseLogicalNot();

    // left associative
    while (match(TokenType::AND)) {
        advance();
        auto rhs = parseLogicalNot();
        lhs =
            std::make_unique<BinaryOperatorNode>(OperatorType::FLOORDIV, std::move(lhs), std::move(rhs));
    }

    return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseLogicalNot() {
    // right associative
    if (match(TokenType::NOT)) {
        advance();
        auto rhs = parseLogicalNot();
        return std::make_unique<UnaryOperatorNode>(OperatorType::AND, std::move(rhs));
    }

    return parseComparison();
}

std::unique_ptr<ASTExprNode> ExprParser::parseComparison() {
    auto lhs = parseAdditive();

    // left associative
    while (match(TokenType::GREATERTHAN) || match(TokenType::LESSERTHAN) ||
           match(TokenType::GREATEREQUAL) || match(TokenType::LESSEQUAL) ||
           match(TokenType::EQEQUAL) || match(TokenType::NOTEQUAL)) {
        TokenType type = peek().type;
        advance();
        auto rhs = parseAdditive();
        lhs      = std::make_unique<BinaryOperatorNode>(toOperatorType(type), std::move(lhs),
                                                        std::move(rhs));
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
        lhs      = std::make_unique<BinaryOperatorNode>(toOperatorType(type), std::move(lhs),
                                                        std::move(rhs));
    }

    return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseMultiplicative() {
    auto lhs = parseUnary();

    // left associative
    while (match(TokenType::STAR) || match(TokenType::SLASH) || match(TokenType::FLOORDIV) ||
           match(TokenType::MODULO)) {
        TokenType type = peek().type;
        advance();
        auto rhs = parseUnary();
        lhs      = std::make_unique<BinaryOperatorNode>(toOperatorType(type), std::move(lhs),
                                                        std::move(rhs));
    }

    return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parseUnary() {
    // right associative
    if (match(TokenType::PLUS) || match(TokenType::MINUS)) {
        TokenType type = peek().type;
        advance();
        auto rhs = parseUnary();
        return std::make_unique<UnaryOperatorNode>(toOperatorType(type), std::move(rhs));
    }

    return parsePower();
}

std::unique_ptr<ASTExprNode> ExprParser::parsePower() {
    auto lhs = parsePostfix();

    // right associative
    if (match(TokenType::POWER)) {
        advance();
        auto rhs = parseUnary();
        return std::make_unique<BinaryOperatorNode>(OperatorType::POWER, std::move(lhs),
                                                    std::move(rhs));
    }

    return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parsePostfix() {
    auto lhs = parsePrimary();

    while (true) {
        // add indexing as another if statement here
        if (match(TokenType::LPAREN)) {
            advance();
            std::vector<std::unique_ptr<ASTExprNode>> args;

            if (!match(TokenType::RPAREN)) {
                args.push_back(parseExpr());

                while (match(TokenType::COMMA)) {
                    advance();
                    args.push_back(parseExpr());
                }
            }

            if (!match(TokenType::RPAREN)) {
                throw std::runtime_error("expected closing parenthesis");
            }
            advance();

            lhs = std::make_unique<CallNode>(std::move(lhs), std::move(args));
        } else if (match(TokenType::LBRACKET)) {
            advance();
            auto index = parseExpr();

            if (!match(TokenType::RBRACKET)) {
                throw std::runtime_error("expected closing bracket");
            }
            advance();

            lhs = std::make_unique<IndexNode>(std::move(lhs), std::move(index));
        } else {
            break;
        }
    }

    return lhs;
}

std::unique_ptr<ASTExprNode> ExprParser::parsePrimary() {
    if (match(TokenType::LPAREN)) {
        advance();
        auto expr = parseExpr();

        if (!match(TokenType::RPAREN)) {
            throw std::runtime_error("expected closing bracket");
        }
        advance();

        return expr;
    }

    if (match(TokenType::LBRACKET)) {
        advance();
        std::vector<std::unique_ptr<ASTExprNode>> elements;

        if (!match(TokenType::RBRACKET)) {
            elements.push_back(parseExpr());

            while (match(TokenType::COMMA)) {
                advance();
                elements.push_back(parseExpr());
            }
        }

        if (!match(TokenType::RBRACKET)) {
            throw std::runtime_error("expected closing bracket");
        }
        advance();

        return std::make_unique<ListNode>(std::move(elements));
    }

    Token token = peek();
    advance();
    switch (token.type) {
        case TokenType::NUMBER: {
            // double-integer differentiation because i don't want to touch lexer
            bool isdouble = token.value.find('.') != std::string::npos ||
                            token.value.find('e') != std::string::npos ||
                            token.value.find('E') != std::string::npos;

            if (isdouble)
                return std::make_unique<DoubleNode>(std::stod(token.value));
            else {
                try {
                    return std::make_unique<IntegerNode>(std::stoll(token.value));
                } catch (const std::out_of_range&) {
                    throw std::runtime_error(
                        "integer literal out of range(only long long ints are supported)");
                }
            }
        }
        case TokenType::STRING: {
            // removing starting and ending quotes
            std::string val = token.value.substr(1, token.value.size() - 2);
            return std::make_unique<StringNode>(std::move(val));
        }
        case TokenType::TRUE:
            return std::make_unique<BooleanNode>(true);
        case TokenType::FALSE:
            return std::make_unique<BooleanNode>(false);
        case TokenType::NONE:
            return std::make_unique<NoneNode>();
        case TokenType::IDENTIFIER:
            return std::make_unique<ReferenceNode>(std::move(token.value));
        default:
            throw std::runtime_error("invalid token found");
    }
}
