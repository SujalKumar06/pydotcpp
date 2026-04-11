#include "aststmt.hpp"

ASTStmtNode::ASTStmtNode(ASTStmtNodeType type) : type(type) {}

VarDeclNode::VarDeclNode(std::unique_ptr<ASTExprNode> name, std::unique_ptr<ASTExprNode> value)
    : ASTStmtNode(ASTStmtNodeType::VAR_DECL), name(std::move(name)), value(std::move(value)) {}

PrintStmtNode::PrintStmtNode(std::unique_ptr<ASTExprNode> expr)
    : ASTStmtNode(ASTStmtNodeType::PRINT_STMT), expr(std::move(expr)) {}

IfStmtNode::IfStmtNode(std::unique_ptr<ASTExprNode> condition, std::unique_ptr<ASTStmtNode> block)
    : ASTStmtNode(ASTStmtNodeType::IF_STMT),
      condition(std::move(condition)),
      block(std::move(block)),
      elif_ptr(nullptr),
      else_ptr(nullptr) {}

ElseStmtNode::ElseStmtNode(std::unique_ptr<ASTStmtNode> block)
    : ASTStmtNode(ASTStmtNodeType::ELSE_STMT), block(std::move(block)) {}

WhileStmtNode::WhileStmtNode(std::unique_ptr<ASTExprNode> condition,
                             std::unique_ptr<ASTStmtNode> block)
    : ASTStmtNode(ASTStmtNodeType::WHILE_STMT),
      condition(std::move(condition)),
      block(std::move(block)) {}

ForStmtNode::ForStmtNode(std::unique_ptr<ASTExprNode> condition, std::unique_ptr<ASTStmtNode> block)
    : ASTStmtNode(ASTStmtNodeType::FOR_STMT),
      condition(std::move(condition)),
      block(std::move(block)) {}

BlockNode::BlockNode() : ASTStmtNode(ASTStmtNodeType::BLOCK) {}

ProgramNode::ProgramNode() : ASTStmtNode(ASTStmtNodeType::PROGRAM) {}

BreakStmtNode::BreakStmtNode() : ASTStmtNode(ASTStmtNodeType::BREAK_STMT) {}

ContinueStmtNode::ContinueStmtNode() : ASTStmtNode(ASTStmtNodeType::CONTINUE_STMT) {}
