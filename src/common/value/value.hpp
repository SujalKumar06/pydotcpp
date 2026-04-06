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
bool isEquals(const Value& lhs, const Value& rhs);
bool isTruthy(const Value& val);
double toNumber(const Value& val, std::string err = "cannot perform number conversion");
