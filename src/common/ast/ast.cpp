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

BooleanNode::BooleanNode(bool value)
    : ASTNode(ASTNodeType::BOOLEAN), value(value) {}

NoneNode::NoneNode()
    : ASTNode(ASTNodeType::NONE) {}

ReferenceNode::ReferenceNode(std::string name)
    : ASTNode(ASTNodeType::REFERENCE), name(std::move(name)) {}

VarDeclNode::VarDeclNode(std::unique_ptr<ASTNode> name, std::unique_ptr<ASTNode> value)
    : ASTNode(ASTNodeType::VAR_DECL), name(std::move(name)), value(std::move(value)) {}

PrintStmtNode::PrintStmtNode(std::unique_ptr<ASTNode> expr)
    : ASTNode(ASTNodeType::PRINT_STMT), expr(std::move(expr)) {}

IfStmtNode::IfStmtNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> block)
    : ASTNode(ASTNodeType::IF_STMT), condition(std::move(condition)), block(std::move(block)), elif_ptr(nullptr), else_ptr(nullptr) {}

ElifStmtNode::ElifStmtNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> block)
    : ASTNode(ASTNodeType::ELIF_STMT), condition(std::move(condition)), block(std::move(block)), elif_ptr(nullptr), else_ptr(nullptr) {}

ElseStmtNode::ElseStmtNode(std::unique_ptr<ASTNode> block)
    : ASTNode(ASTNodeType::ELSE_STMT), block(std::move(block)) {}

BlockNode::BlockNode()
    : ASTNode(ASTNodeType::BLOCK) {}

ProgramNode::ProgramNode()
    : ASTNode(ASTNodeType::PROGRAM) {}
