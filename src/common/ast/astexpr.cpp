#include "astexpr.hpp"

ASTExprNode::ASTExprNode(ASTExprNodeType type) : type(type) {}

BinaryOperatorNode::BinaryOperatorNode(OperatorType op, std::unique_ptr<ASTExprNode> lhs,
                                       std::unique_ptr<ASTExprNode> rhs)
    : ASTExprNode(ASTExprNodeType::BINARY), op(op), lhs(std::move(lhs)), rhs(std::move(rhs)) {}

UnaryOperatorNode::UnaryOperatorNode(OperatorType op, std::unique_ptr<ASTExprNode> rhs)
    : ASTExprNode(ASTExprNodeType::UNARY), op(op), rhs(std::move(rhs)) {}

CallNode::CallNode(std::unique_ptr<ASTExprNode> callee,
                   std::vector<std::unique_ptr<ASTExprNode>> args)
    : ASTExprNode(ASTExprNodeType::CALL), callee(std::move(callee)), args(std::move(args)) {}

StringNode::StringNode(std::string value)
    : ASTExprNode(ASTExprNodeType::STRING), value(std::move(value)) {}

DoubleNode::DoubleNode(double value) : ASTExprNode(ASTExprNodeType::DOUBLE), value(value) {}

IntegerNode::IntegerNode(long long value) : ASTExprNode(ASTExprNodeType::INTEGER), value(value) {}

BooleanNode::BooleanNode(bool value) : ASTExprNode(ASTExprNodeType::BOOLEAN), value(value) {}

ListNode::ListNode(std::vector<std::unique_ptr<ASTExprNode>> elements)
    : ASTExprNode(ASTExprNodeType::LIST), elements(std::move(elements)) {}

NoneNode::NoneNode() : ASTExprNode(ASTExprNodeType::NONE) {}

ReferenceNode::ReferenceNode(std::string name)
    : ASTExprNode(ASTExprNodeType::REFERENCE), name(std::move(name)) {}

IndexNode::IndexNode(std::unique_ptr<ASTExprNode> lhs, std::unique_ptr<ASTExprNode> index)
    : ASTExprNode(ASTExprNodeType::INDEX), lhs(std::move(lhs)), index(std::move(index)) {}
