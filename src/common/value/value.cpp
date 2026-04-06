#include "value.hpp"

#include <stdexcept>
#include <string>
#include <type_traits>

bool isEquals(const Value& lhs, const Value& rhs) {
    return std::visit(
        [](auto&& lhs, auto&& rhs) -> bool {
            using ltype = std::decay_t<decltype(lhs)>;
            using rtype = std::decay_t<decltype(rhs)>;

            // compile-time branching with constexpr
            if constexpr (std::is_same_v<ltype, rtype>)
                return lhs == rhs;

            // True is 1.0 and False is 0.0
            else if constexpr (std::is_same_v<ltype, bool> && std::is_same_v<rtype, double>)
                return (lhs ? 1.0 : 0.0) == rhs;
            else if constexpr (std::is_same_v<ltype, double> && std::is_same_v<rtype, bool>)
                return lhs == (rhs ? 1.0 : 0.0);

            // not equals
            else
                return false;
        },
        lhs, rhs);
}

bool isTruthy(const Value& val) {
    return std::visit(
        [](auto&& val) -> bool {
            using T = std::decay_t<decltype(val)>;

            // None
            if constexpr (std::is_same_v<T, std::monostate>)
                return false;
            // number
            else if constexpr (std::is_same_v<T, double>)
                return val != 0.0;
            // bool
            else if constexpr (std::is_same_v<T, bool>)
                return val;
            // string
            else if constexpr (std::is_same_v<T, std::string>)
                return !val.empty();

            // fallback
            else
                return false;
        },
        val);
}

// helper function to deal with boolean-double operations
double toNumber(const Value& val, std::string err = "cannot perform number conversion") {
    return std::visit(
        [&err](auto&& val) -> double {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, double>)
                return val;
            else if constexpr (std::is_same_v<T, bool>)
                return (val ? 1.0 : 0.0);

            else
                throw std::runtime_error(err);
        },
        val);
}
