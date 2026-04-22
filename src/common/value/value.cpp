#include "value.hpp"

#include <climits>
#include <stdexcept>
#include <string>
#include <type_traits>

bool isEquals(const Value& lhs, const Value& rhs) {
    return std::visit(
        [](auto&& lhs, auto&& rhs) -> bool {
            using ltype = std::decay_t<decltype(lhs)>;
            using rtype = std::decay_t<decltype(rhs)>;

            // compile-time branching with constexpr
            if constexpr (std::is_same_v<ltype, std::shared_ptr<List>> &&
                          std::is_same_v<rtype, std::shared_ptr<List>>) {
                if (lhs == rhs)
                    return true;

                if (lhs->elements.size() != rhs->elements.size())
                    return false;

                for (int i = 0; i < lhs->elements.size(); i++) {
                    if (!isEquals(lhs->elements[i], rhs->elements[i]))
                        return false;
                }

                return true;
            }

            else if constexpr (std::is_same_v<ltype, rtype>)
                return lhs == rhs;

            else if constexpr (std::is_arithmetic_v<ltype> && std::is_arithmetic_v<rtype>) {
                // if lhs or rhs is double, compare as double
                auto lldcomp = [](double d, long long ll) -> bool {
                    // < -2**63 or >= 2**63 (based on long long limits)
                    if (d < -9223372036854775808.0 || d >= 9223372036854775808.0)
                        return false;

                    long long dconv = static_cast<long long>(d);
                    if (static_cast<double>(dconv) != d)
                        return false;

                    return ll == dconv;
                };

                if constexpr (std::is_same_v<ltype, double>) {
                    double doubleval = static_cast<double>(lhs);
                    long long llval  = static_cast<long long>(rhs);
                    return lldcomp(doubleval, llval);
                } else if constexpr (std::is_same_v<rtype, double>) {
                    double doubleval = static_cast<double>(rhs);
                    long long llval  = static_cast<long long>(lhs);
                    return lldcomp(doubleval, llval);
                }
                // else compare as integers
                return static_cast<long long>(lhs) == static_cast<long long>(rhs);
            }

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
            // double
            else if constexpr (std::is_same_v<T, double>)
                return val != 0.0;
            // integer
            else if constexpr (std::is_same_v<T, long long>)
                return val != 0LL;
            // bool
            else if constexpr (std::is_same_v<T, bool>)
                return val;
            // string
            else if constexpr (std::is_same_v<T, std::string>)
                return !val.empty();
            // list
            else if constexpr (std::is_same_v<T, std::shared_ptr<List>>)
                return !val->elements.empty();
            // fallback(everything's truthy unless it's falsy)
            else
                return true;
        },
        val);
}

// helper function to deal with double conversion operations
double toDouble(const Value& val, std::string err) {
    return std::visit(
        [&err](auto&& val) -> double {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, double>)
                return val;
            else if constexpr (std::is_same_v<T, bool>)
                return (val ? 1.0 : 0.0);
            else if constexpr (std::is_same_v<T, long long>)
                return static_cast<double>(val);
            else
                throw std::runtime_error(err);
        },
        val);
}

// helper function to deal with integer conversion operations
long long toInteger(const Value& val, std::string err) {
    return std::visit(
        [&err](auto&& val) -> long long {
            using T = std::decay_t<decltype(val)>;

            if constexpr (std::is_same_v<T, long long>)
                return val;
            else if constexpr (std::is_same_v<T, bool>)
                return (val ? 1LL : 0LL);
            else
                throw std::runtime_error(err);
        },
        val);
}

// safe ops

// binary exponentiation
long long binexp(long long base, long long exp) {
    if (exp < 0)
        throw std::runtime_error("negative exponent not supported for integer pow");

    long long result = 1;
    long long curr   = base;
    long long e      = exp;

    while (e > 0) {
        if (e & 1)
            result = safeMul(result, curr);

        e >>= 1;
        if (e > 0)
            curr = safeMul(curr, curr);
    }
    return result;
}

long long safeAdd(long long a, long long b) {
    if ((b > 0 && a > LLONG_MAX - b) || (b < 0 && a < LLONG_MIN - b)) {
        throw std::runtime_error("integer overflow in addition");
    }
    return a + b;
}

long long safeSub(long long a, long long b) {
    if ((b < 0 && a > LLONG_MAX + b) || (b > 0 && a < LLONG_MIN + b)) {
        throw std::runtime_error("integer overflow in subtraction");
    }
    return a - b;
}

long long safeMul(long long a, long long b) {
    if (a == 0 || b == 0)
        return 0;

    if (a == -1 && b == LLONG_MIN)
        throw std::runtime_error("integer overflow in multiplication");
    if (b == -1 && a == LLONG_MIN)
        throw std::runtime_error("integer overflow in multiplication");

    if (a > 0) {
        if (b > 0) {
            if (a > LLONG_MAX / b)
                throw std::runtime_error("integer overflow in multiplication");
        } else {
            if (b < LLONG_MIN / a)
                throw std::runtime_error("integer overflow in multiplication");
        }
    } else {
        if (b > 0) {
            if (a < LLONG_MIN / b)
                throw std::runtime_error("integer overflow in multiplication");
        } else {
            if (a != 0 && b < LLONG_MAX / a)
                throw std::runtime_error("integer overflow in multiplication");
        }
    }

    return a * b;
}

long long safeDiv(long long a, long long b) {
    if (b == 0)
        throw std::runtime_error("division by zero");

    if (a == LLONG_MIN && b == -1)
        throw std::runtime_error("integer overflow in division");

    return a / b;
}

long long safeMod(long long a, long long b) {
    if (b == 0)
        throw std::runtime_error("modulo by zero");

    if (a == LLONG_MIN && b == -1)
        return 0;

    long long r = a % b;

    if (r != 0 && ((r > 0 && b < 0) || (r < 0 && b > 0))) {
        r += b;
    }

    return r;
}
