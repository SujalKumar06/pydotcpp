#include "aststmt.hpp"

ASTStmtNode::ASTStmtNode(ASTStmtNodeType type) : type(type) {}

AssignNode::AssignNode(std::unique_ptr<ASTExprNode> lhs, std::unique_ptr<ASTExprNode> value)
    : ASTStmtNode(ASTStmtNodeType::ASSIGN), lhs(std::move(lhs)), value(std::move(value)) {}

CompoundNode::CompoundNode(std::unique_ptr<ASTExprNode> lhs, std::unique_ptr<ASTExprNode> value,
                           OperatorType op)
    : ASTStmtNode(ASTStmtNodeType::COMPOUND),
      lhs(std::move(lhs)),
      value(std::move(value)),
      op(op) {}

FunctionDeclNode::FunctionDeclNode(std::string name, std::vector<std::string> params,
                                   std::unique_ptr<ASTStmtNode> body)
    : ASTStmtNode(ASTStmtNodeType::FUNC_DECL),
      name(std::move(name)),
      params(std::move(params)),
      body(std::move(body)) {}

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

ReturnStmtNode::ReturnStmtNode(std::unique_ptr<ASTExprNode> value)
    : ASTStmtNode(ASTStmtNodeType::RETURN_STMT), value(std::move(value)) {}

ExprStmtNode::ExprStmtNode(std::unique_ptr<ASTExprNode> expr)
    : ASTStmtNode(ASTStmtNodeType::EXPR_STMT), expr(std::move(expr)) {}
