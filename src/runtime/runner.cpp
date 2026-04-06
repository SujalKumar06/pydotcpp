#include "runner.hpp"

#include <cmath>
#include <stdexcept>
#include <type_traits>

Runner::Runner() {}

void Runner::runStmt(const ASTStmtNode& stmt) {}

Value Runner::evalExpr(const ASTExprNode& expr) {
    switch (expr.type) {
        case ASTExprNodeType::BINARY:
            return evalBinary(std::static_cast<const BinaryOperatorNode&>(expr));
        case ASTExprNodeType::UNARY:
            return evalUnary(std::static_cast<const UnaryOperatorNode&>(expr));

        case ASTExprNodeType::STRING:
        case ASTExprNodeType::NUMBER:
        case ASTExprNodeType::BOOLEAN:
        case ASTExprNodeType::REFERENCE:
        case ASTExprNodeType::NONE:
            return evalPrimary(expr);

        default:
            throw std::runtime_error("unexpected expr node type");
    }
}

Value Runner::evalBinary(const BinaryOperatorNode& expr) {
    Value lhs = evalExpr(*expr.lhs);

    // short-circuited or
    if (expr.op == OperatorType::OR) {
        if (isTruthy(lhs))
            return lhs;
        else
            return evalExpr(*expr.rhs);
    }

    // short-circuited and
    if (expr.op == OperatorType::AND) {
        if (!isTruthy(lhs))
            return lhs;
        else
            return evalExpr(*expr.rhs);
    }

    Value rhs = evalExpr(*expr.rhs);

    switch (expr.op) {
        case OperatorType::EQEQUAL:
            return Value(isEquals(lhs, rhs));
        case OperatorType::NOTEQUAL:
            return Value(!isEquals(lhs, rhs));

        case OperatorType::GREATERTHAN:
        case OperatorType::GREATEREQUAL:
        case OperatorType::LESSEQUAL:
        case OperatorType::LESSERTHAN:
            return compareValues(lhs, rhs, expr.op);

        case OperatorType::PLUS:
        case OperatorType::MINUS:
        case OperatorType::STAR:
        case OperatorType::SLASH:
        case OperatorType::FLOORDIV:
        case OperatorType::MODULO:
        case OperatorType::POWER:
            return arithmeticValues(lhs, rhs, expr.op);

        default:
            throw std::runtime_error("unexpected binary operator type");
    }
}

Value Runner::evalUnary(const UnaryOperatorNode& expr) {
    Value rhs = evalExpr(*expr.rhs);

    switch (expr.op) {
        case OperatorType::NOT:
            return Value(!isTruthy(rhs));
        case OperatorType::PLUS:
            return Value(toNumber(rhs));
        case OperatorType::MINUS:
            return Value(-toNumber(rhs));

        default:
            throw std::runtime_error("unexpected unary operator type");
    }
}

Value Runner::evalPrimary(const ASTExprNode& expr) {
    switch (expr.type) {
        case ASTExprNodeType::STRING:
            return Value(std::static_cast<const StringNode&>(expr).value);
        case ASTExprNodeType::NUMBER:
            return Value(std::static_cast<const NumberNode&>(expr).value);
        case ASTExprNodeType::BOOLEAN:
            return Value(std::static_cast<const BooleanNode&>(expr).value);

        case ASTExprNodeType::REFERENCE: {
            std::string name = std::static_cast<const ReferenceNode&>(expr).name;
            auto it          = env.find(name);

            if (it != env.end())
                return it->second;
            throw std::runtime_error("undefined variable encountered");
        }

        // None
        case ASTExprNodeType::NONE:
            return Value(std::monostate{});

        default:
            throw std::runtime_error("unexpected primary expr type");
    }
}

Value compareValues(const Value& lhs, const Value& rhs, OperatorType op) {
    double lhs_num = toNumber(lhs, "comparison between incompatible types");
    double rhs_num = toNumber(rhs, "comparison between incompatible types");

    switch (op) {
        case OperatorType::GREATERTHAN:
            return Value(lhs_num > rhs_num);
        case OperatorType::GREATEREQUAL:
            return Value(lhs_num >= rhs_num);
        case OperatorType::LESSEQUAL:
            return Value(lhs_num <= lhs_num);
        case OperatorType::LESSERTHAN:
            return Value(lhs_num < rhs_num);

        default:
            throw std::runtime_error("unexpected comparison operator type");
    }
}

Value arithmeticValues(const Value& lhs, const Value& rhs, OperatorType op) {
    switch (op) {
        case OperatorType::PLUS:
            return std::visit(
                [&lhs, &rhs](auto&& lval, auto&& rval) {
                    using ltype = std::decay_t<decltype(lval)>;
                    using rtype = std::decay_t<decltype(rval)>;

                    if constexpr (std::is_same_v(ltype, std::string) &&
                                  std::is_same_v(rtype, std::string))
                        return Value(lval + rval);
                    else {
                        double lnum = toNumber(lhs, "addition between incompatible types");
                        double rnum = toNumber(rhs, "addition between incompatible types");
                        return Value(lnum + rnum);
                    }
                },
                lhs, rhs);

        case OperatorType::STAR:
            return std::visit(
                [](auto&& lval, auto&& rval) {
                    using ltype = std::decay_t<decltype(lval)>;
                    using rtype = std::decay_t<decltype(rval)>;

                    double rnum = toNumber(rhs, "multiplication between incompatible types");

                    if constexpr (std::is_same_v(ltype, std::string)) {
                        // arbitrary epsilon for considering a double as an integer
                        if (std::abs(rnum - std::round(rnum)) > 1e-9 || std::round(rnum) < 0)
                            throw std::runtime_error(
                                "strings can only be multiplied by non-negative integral values");

                        std::string output;
                        while (double i = std::round(rnum); i > 0; i--)
                            output += lval;
                        return Value(output);
                    } else {
                        double lnum = toNumber(lhs, "addition between incompatible types");
                        return Value(lnum * rnum);
                    }
                },
                lhs, rhs);

        case OperatorType::MINUS: {
            double lnum = toNumber(lhs, "subtraction between incompatible types");
            double rnum = toNumber(rhs, "subtraction between incompatible types");
            return Value(lnum - rnum);
        }

        case OperatorType::SLASH: {
            double lnum = toNumber(lhs, "division between incompatible types");
            double rnum = toNumber(rhs, "division between incompatible types");

            if (rnum == 0.0)
                throw std::runtime_error("division by zero");
            return Value(lnum / rnum);
        }

        case OperatorType::FLOORDIV: {
            double lnum = toNumber(lhs, "floor division between incompatible types");
            double rnum = toNumber(rhs, "floor division between incompatible types");

            if (rnum == 0.0)
                throw std::runtime_error("division by zero");
            return Value(std::floor(lnum / rnum));
        }

        case OperatorType::MODULO: {
            double lnum = toNumber(lhs, "modulo between incompatible types");
            double rnum = toNumber(rhs, "modulo between incompatible types");

            if (rnum == 0.0)
                throw std::runtime_error("division by zero");

            double mod = std::fmod(lnum, rnum);
            if ((mod != 0) && ((rnum > 0 && mod < 0) || (rnum < 0 && mod > 0)))
                mod += rnum;

            return Value(mod);
        }

        case OperatorType::POWER: {
            double lnum = toNumber(lhs, "power between incompatible types");
            double rnum = toNumber(rhs, "power between incompatible types");

            double power = std::pow(lnum, rnum);
            if (std::isnan(power))
                throw std::runtime_error("complex/imaginary results are not supported");

            return Value(power);
        }

        default:
            throw std::runtime_error("unexpected arithmetic operator type");
    }
}
