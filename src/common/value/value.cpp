#include "value.hpp"

#include <type_traits>
#include <string>
#include <stdexcept>

bool equals(const Value& lhs, const Value& rhs) {
    return std::visit([](auto&& lhs, auto&& rhs) -> bool {
        using ltype = std::decay_t<decltype(lhs)>;
        using rtype = std::decay_t<decltype(rhs)>;

        //compile-time branching with constexpr
        if constexpr (std::is_same<ltype, rtype>) {
            return lhs == rhs;
        }

        //True is 1.0 and False is 0.0
        if constexpr(std::is_same<ltype, bool> && std::is_same<rtype, double>) {
            return (lhs ? 1.0 : 0.0) == rhs;
        }

        if constexpr(std::is_same<ltype, double> && std::is_same<rtype, bool>) {
            return lhs == (rhs ? 1.0 : 0.0);
        }

        //not equals
        return false;
    }, lhs, rhs);
}

bool isTruthy(const Value& val) {
    return std::visit([](auto&& val) -> bool {
        using T = std::decay_t<decltype(val)>;

        //None
        if constexpr(std::is_same<T, std::monostate>) return false;
        //number
        if constexpr(std::is_same<T, double>) return val != 0.0;
        //bool
        if constexpr(std::is_same<T, bool>) return val;
        //string
        if constexpr(std::is_same<T, std::string>) return !val.empty();
    }, val);
}

//helper function to deal with boolean-double operations
double toNumber(const Value& val) {
    return std::visit([](auto&& val) -> double {
        using T = std::decay_t<decltype(val)>;

        if constexpr(std::is_same<T, double>) return val;
        if constexpr(std::is_same<T, bool>) return (val ? 1.0 : 0.0);

        throw std::runtime_error("cannot perform number conversion");
    }, val);
}
