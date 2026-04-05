#include "runner.hpp"

#include <stdexcept>

Runner::Runner() {}

void Runner::runStmt(const ASTStmtNode& stmt) {

}

Value Runner::evalExpr(const ASTExprNode& expr) {
    switch (expr.type) {
        case ASTExprNodeType::BINARY: return evalBinary(std::static_cast<const BinaryOperatorNode&>(expr));
        case ASTExprNodeType::UNARY: return evalUnary(std::static_cast<const UnaryOperatorNode&>(expr));

        case ASTExprNodeType::STRING:
        case ASTExprNodeType::NUMBER:
        case ASTExprNodeType::BOOLEAN:
        case ASTExprNodeType::REFERENCE:
        case ASTExprNodeType::NONE:
            return evalPrimary(expr);

        default: throw std::runtime_error("unexpected expr node type");
    }
}

Value Runner::evalBinary(const BinaryOperatorNode& expr) {
    throw std::runtime_error("not implemented yet");
}

Value Runner::evalUnary(const UnaryOperatorNode& expr) {
    Value rhs = evalExpr(*expr.rhs);

    switch (expr.op) {
        case OperatorType::NOT: return Value(!isTruthy(rhs));
        case OperatorType::PLUS: return Value(toNumber(rhs));
        case OperatorType::MINUS: return Value(-toNumber(rhs));

        default: throw std::runtime_error("unexpected unary operator type");
    }
}

Value Runner::evalPrimary(const ASTExprNode& expr) {
    switch (expr.type) {
        case ASTExprNodeType::STRING: return Value(std::static_cast<const StringNode&>(expr).value);
        case ASTExprNodeType::NUMBER: return Value(std::static_cast<const NumberNode&>(expr).value);
        case ASTExprNodeType::BOOLEAN: return Value(std::static_cast<const BooleanNode&>(expr).value);

        case ASTExprNodeType::REFERENCE: {
            std::string name = std::static_cast<const ReferenceNode&>(expr).name;
            auto it = env.find(name);

            if (it != env.end()) return it->second;
            throw std::runtime_error("undefined variable encountered");
        }

        //None
        case ASTExprNodeType::NONE: return Value(std::monostate{});

        default: throw std::runtime_error("unexpected primary expr type");
    }
}


