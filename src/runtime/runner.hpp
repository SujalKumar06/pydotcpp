#pragma once

#include "value.hpp"
#include "aststmt.hpp"
#include "astexpr.hpp"

#include <memory>
#include <unordered_map>
#include <string>

//to deal with call stack unwinding for break, continue
enum class ReturnType {
    NORMAL,
    BREAK,
    CONTINUE,
    RETURN,
};

class Environment {
public:
    Environment(std::shared_ptr<Environment> parent);
    Value get(const std::string& name);
    void assign(const std::string& name, const Value& val);

private:
    std::unordered_map<std::string, Value> values;
    std::shared_ptr<Environment> parent;
};

class Runner {
public:
    Runner();
    ReturnType runStmt(const ASTStmtNode& stmt);
    Value evalExpr(const ASTExprNode& expr);

private:
    //the active variable environment
    std::shared_ptr<Environment> env;

    //current return value
    Value returnval;

    //expr eval helpers
    Value evalBinary(const BinaryOperatorNode& expr);
    Value evalUnary(const UnaryOperatorNode& expr);
    Value evalCall(const CallNode& expr);
    Value evalPrimary(const ASTExprNode& expr);

    //value operation helpers
    Value compareValues(const Value& lhs, const Value& rhs, OperatorType op);
    Value arithmeticValues(const Value& lhs, const Value& rhs, OperatorType op);

    //stmt eval helpers
    void printValue(const Value& str);
    void printString(const std::string& str);
};
