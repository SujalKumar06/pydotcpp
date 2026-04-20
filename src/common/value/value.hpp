#pragma once

#include "aststmt.hpp"

#include <variant>
#include <string>
#include <memory>
#include <vector>

//function
struct Function;
struct List;
class Environment; //defined in runner.hpp

//runtime value representation
using Value = std::variant<
    long long,
    double,
    std::string,
    bool,
    std::monostate,
    std::shared_ptr<Function>,
    std::shared_ptr<List>
>;

struct Function {
    std::vector<std::string> params;
    std::shared_ptr<ASTStmtNode> body;
    std::shared_ptr<Environment> closure;
};

struct List {
    std::vector<Value> elements;
};

//helpers related to Value
bool isEquals(const Value& lhs, const Value& rhs);
bool isTruthy(const Value& val);
long long toInteger(const Value& val, std::string err = "cannot perform integer conversion");
double toDouble(const Value& val, std::string err = "cannot perform double conversion");

//safe ops
long long binexp(long long base, long long exp);
long long safeAdd(long long a, long long b);
long long safeSub(long long a, long long b);
long long safeMul(long long a, long long b);
long long safeDiv(long long a, long long b);
long long safeMod(long long a, long long b);
