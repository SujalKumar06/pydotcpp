#include "ast.hpp"

ASTNode::ASTNode(ASTNodeType type)
    : type(type) {}

BinaryOperatorNode::BinaryOperatorNode(
    OperatorType op,
    std::unique_ptr<ASTNode> lhs,
    std::unique_ptr<ASTNode> rhs)
    : ASTNode(ASTNodeType::BINARY), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

UnaryOperatorNode::UnaryOperatorNode(
    OperatorType op,
    std::unique_ptr<ASTNode> rhs)
    : ASTNode(ASTNodeType::UNARY), op(op), rhs(std::move(rhs)) {}

StringNode::StringNode(std::string value)
    : ASTNode(ASTNodeType::STRING), value(std::move(value)) {}

NumberNode::NumberNode(double value)
    : ASTNode(ASTNodeType::NUMBER), value(value) {}

ReferenceNode::ReferenceNode(std::string name)
    : ASTNode(ASTNodeType::REFERENCE), name(std::move(name)) {}
