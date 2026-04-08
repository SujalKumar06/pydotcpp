#pragma once

#include "value.hpp"
#include "aststmt.hpp"
#include "astexpr.hpp"

#include <memory>
#include <unordered_map>
#include <string>

enum class ReturnType {
    NORMAL,
    BREAK,
    CONTINUE,
};

class Runner {
public:
    Runner();
    ReturnType runStmt(const ASTStmtNode& stmt);
    Value evalExpr(const ASTExprNode& expr);

private:
    //the global variable environment
    std::unordered_map<std::string, Value> env;

    //expr eval helpers
    Value evalBinary(const BinaryOperatorNode& expr);
    Value evalUnary(const UnaryOperatorNode& expr);
    Value evalPrimary(const ASTExprNode& expr);

    //value operation helpers
    Value compareValues(const Value& lhs, const Value& rhs, OperatorType op);
    Value arithmeticValues(const Value& lhs, const Value& rhs, OperatorType op);
};
