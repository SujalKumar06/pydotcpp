#pragma once

#include <variant>
#include <string>

//runtime value representation
using Value = std::variant<
    long long,
    double,
    std::string,
    bool,
    std::monostate
>;

//helpers related to Value
bool isEquals(const Value& lhs, const Value& rhs);
bool isTruthy(const Value& val);
long long toInteger(const Value& val, std::string err = "cannot perform double conversion");
double toDouble(const Value& val, std::string err = "cannot perform integer conversion");

//safe ops
long long binexp(long long base, long long exp);
long long safeAdd(long long a, long long b);
long long safeSub(long long a, long long b);
long long safeMul(long long a, long long b);
long long safeDiv(long long a, long long b);
long long safeMod(long long a, long long b);
