#pragma once

#include <variant>
#include <string>

using Value = std::variant<
    double,
    std::string,
    bool,
    std::monostate
>;

//helpers related to Value
bool equals(const Value& lhs, const Value& rhs);
bool isTruthy(const Value& val);
double toNumber(const Value& val);
