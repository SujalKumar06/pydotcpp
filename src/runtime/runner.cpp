#include "runner.hpp"

#include <cmath>
#include <iostream>
#include <stdexcept>
#include <type_traits>

Environment::Environment(std::shared_ptr<Environment> parent) : parent(std::move(parent)) {}

Value Environment::get(const std::string& name) {
    auto it = values.find(name);
    if (it != values.end())
        return it->second;

    if (parent)
        return parent->get(name);
    else
        throw std::runtime_error("undefined variable referenced");
}

void Environment::assign(const std::string& name, const Value& val) {
    values[name] = val;
}

Runner::Runner() {
    env = std::make_shared<Environment>(nullptr);
    returnval = std::monostate{};
}

ReturnType Runner::runStmt(const ASTStmtNode& stmt) {
    switch (stmt.type) {
        case ASTStmtNodeType::PROGRAM: {
            for (const auto& statementptr : static_cast<const ProgramNode&>(stmt).statements) {
                ReturnType ret = runStmt(*statementptr);

                if (ret == ReturnType::BREAK)
                    throw std::runtime_error("break outside loop");
                else if (ret == ReturnType::CONTINUE)
                    throw std::runtime_error("continue outside loop");
                else if (ret == ReturnType::RETURN)
                    throw std::runtime_error("cannot return from program body");
            }

            return ReturnType::NORMAL;
        }

        case ASTStmtNodeType::BLOCK: {
            for (const auto& statementptr : static_cast<const BlockNode&>(stmt).statements) {
                ReturnType ret = runStmt(*statementptr);

                if (ret != ReturnType::NORMAL)
                    return ret;
            }

            return ReturnType::NORMAL;
        }

        case ASTStmtNodeType::VAR_DECL: {
            const VarDeclNode& vardecl = static_cast<const VarDeclNode&>(stmt);
            std::string name           = static_cast<const ReferenceNode&>(*vardecl.name).name;
            Value rhs                  = evalExpr(*vardecl.value);
            env->assign(name, rhs);
            return ReturnType::NORMAL;
        }

        case ASTStmtNodeType::PRINT_STMT: {
            Value val = evalExpr(*static_cast<const PrintStmtNode&>(stmt).expr);
            printValue(val);
            return ReturnType::NORMAL;
        }

        case ASTStmtNodeType::WHILE_STMT: {
            const WhileStmtNode& whilestmt = static_cast<const WhileStmtNode&>(stmt);
            while (isTruthy(evalExpr(*whilestmt.condition))) {
                ReturnType ret = runStmt(*whilestmt.block);

                if (ret == ReturnType::BREAK)
                    break;
                else if (ret == ReturnType::CONTINUE)
                    continue;
            }

            return ReturnType::NORMAL;
        }

        case ASTStmtNodeType::IF_STMT: {
            const IfStmtNode& ifstmt = static_cast<const IfStmtNode&>(stmt);

            if (isTruthy(evalExpr(*ifstmt.condition)))
                return runStmt(*ifstmt.block);
            else if (ifstmt.elif_ptr)
                return runStmt(*ifstmt.elif_ptr);
            else if (ifstmt.else_ptr)
                return runStmt(*ifstmt.else_ptr);
            else
                return ReturnType::NORMAL;
        }

        case ASTStmtNodeType::ELSE_STMT:
            return runStmt(*static_cast<const ElseStmtNode&>(stmt).block);

        case ASTStmtNodeType::CONTINUE_STMT:
            return ReturnType::CONTINUE;

        case ASTStmtNodeType::BREAK_STMT:
            return ReturnType::BREAK;

        case ASTStmtNodeType::RETURN_STMT: {
            const ReturnStmtNode& returnstmt = static_cast<const ReturnStmtNode&>(stmt);
            if (returnstmt.value) returnval = evalExpr(*returnstmt.value);
            else returnval = std::monostate{};
            return ReturnType::RETURN;
        }

        case ASTStmtNodeType::EXPR_STMT: {
            evalExpr(*static_cast<const ExprStmtNode&>(stmt).expr);
            return ReturnType::NORMAL;
        }

        case ASTStmtNodeType::FUNC_DECL: {
            const FunctionDeclNode& fundeclnode = static_cast<const FunctionDeclNode&>(stmt);
            Value func = std::make_shared<Function>(
                Function{
                    fundeclnode.params,
                    fundeclnode.body,
                    env
                }
            );

            env->assign(fundeclnode.name, func);
            return ReturnType::NORMAL;
        }

        default:
            throw std::runtime_error("unexpected stmt node type");
    }
}

Value Runner::evalExpr(const ASTExprNode& expr) {
    switch (expr.type) {
        case ASTExprNodeType::BINARY:
            return evalBinary(static_cast<const BinaryOperatorNode&>(expr));
        case ASTExprNodeType::UNARY:
            return evalUnary(static_cast<const UnaryOperatorNode&>(expr));

        case ASTExprNodeType::CALL:
            return evalCall(static_cast<const CallNode&> (expr));

        case ASTExprNodeType::STRING:
        case ASTExprNodeType::DOUBLE:
        case ASTExprNodeType::INTEGER:
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
            return isEquals(lhs, rhs);
        case OperatorType::NOTEQUAL:
            return !isEquals(lhs, rhs);

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
            return !isTruthy(rhs);
        case OperatorType::PLUS: {
            if (std::holds_alternative<double>(rhs))
                return toDouble(rhs);
            return toInteger(rhs);
        }
        case OperatorType::MINUS: {
            if (std::holds_alternative<double>(rhs))
                return -toDouble(rhs);
            return -toInteger(rhs);
        }
        default:
            throw std::runtime_error("unexpected unary operator type");
    }
}

Value Runner::evalCall(const CallNode& expr) {
    Value callee = evalExpr(*expr.callee);
    if (!std::holds_alternative<std::shared_ptr<Function>>(callee)) throw std::runtime_error("non-callable object being called");

    std::shared_ptr<Function> func = std::get<std::shared_ptr<Function>>(callee);
    if (func->params.size() != expr.args.size()) throw std::runtime_error("unexpected number of arguments passed into function");

    auto newenv = std::make_shared<Environment>(func->closure);
    for (int i = 0; i < func->params.size(); i++) {
        newenv->assign(func->params[i], evalExpr(*expr.args[i]));
    }

    auto prev = env;
    env = newenv;

    ReturnType ret = runStmt(*func->body);
    env = prev;

    if (ret == ReturnType::BREAK) throw std::runtime_error("break outside loop");
    else if (ret == ReturnType::CONTINUE) throw std::runtime_error("continue outside loop");
    else if (ret == ReturnType::NORMAL) returnval = std::monostate{};

    Value evaled = returnval;
    returnval = std::monostate{};

    return evaled;
}

Value Runner::evalPrimary(const ASTExprNode& expr) {
    switch (expr.type) {
        case ASTExprNodeType::STRING:
            return static_cast<const StringNode&>(expr).value;
        case ASTExprNodeType::DOUBLE:
            return static_cast<const DoubleNode&>(expr).value;
        case ASTExprNodeType::INTEGER:
            return static_cast<const IntegerNode&>(expr).value;
        case ASTExprNodeType::BOOLEAN:
            return static_cast<const BooleanNode&>(expr).value;

        case ASTExprNodeType::REFERENCE: {
            std::string name = static_cast<const ReferenceNode&>(expr).name;
            return env->get(name);
        }

        // None
        case ASTExprNodeType::NONE:
            return std::monostate{};

        default:
            throw std::runtime_error("unexpected primary expr type");
    }
}

Value Runner::compareValues(const Value& lhs, const Value& rhs, OperatorType op) {
    auto cmp = [&](auto lval, auto rval) -> Value {
        switch (op) {
            case OperatorType::GREATERTHAN:
                return lval > rval;
            case OperatorType::GREATEREQUAL:
                return lval >= rval;
            case OperatorType::LESSEQUAL:
                return lval <= rval;
            case OperatorType::LESSERTHAN:
                return lval < rval;

            default:
                throw std::runtime_error("unexpected comparison operator type");
        }
    };

    if (std::holds_alternative<double>(lhs) || std::holds_alternative<double>(rhs)) {
        return cmp(toDouble(lhs, "comparison between incompatible types"),
                   toDouble(rhs, "comparison between incompatible types"));
    }

    return cmp(toInteger(lhs, "comparison between incompatible types"),
               toInteger(rhs, "comparison between incompatible types"));
}

Value Runner::arithmeticValues(const Value& lhs, const Value& rhs, OperatorType op) {
    switch (op) {
        case OperatorType::PLUS: {
            // string+string
            if (std::holds_alternative<std::string>(lhs) &&
                std::holds_alternative<std::string>(rhs)) {
                return std::get<std::string>(lhs) + std::get<std::string>(rhs);
            }

            std::string err = "addition between incompatible types";

            // double
            if (std::holds_alternative<double>(lhs) || std::holds_alternative<double>(rhs))
                return toDouble(lhs, err) + toDouble(rhs, err);
            // integer
            return safeAdd(toInteger(lhs, err), toInteger(rhs, err));
        }

        case OperatorType::STAR: {
            auto mulstr = [](std::string str, long long num) -> std::string {
                std::string output;
                for (long long i = 0; i < num; i++)
                    output += str;
                return output;
            };

            // string*num
            if (std::holds_alternative<std::string>(lhs)) {
                long long num = toInteger(rhs, "multiplication between incompatible types");
                if (num < 0)
                    throw std::runtime_error(
                        "strings can only be multiplied by non-negative integral values");

                std::string str = std::get<std::string>(lhs);
                return mulstr(str, num);
            }

            // num*string
            if (std::holds_alternative<std::string>(rhs)) {
                long long num = toInteger(lhs, "multiplication between incompatible types");
                if (num < 0)
                    throw std::runtime_error(
                        "strings can only be multiplied by non-negative integral values");

                std::string str = std::get<std::string>(rhs);
                return mulstr(str, num);
            }

            std::string err = "multiplication between incompatible types";

            // double
            if (std::holds_alternative<double>(lhs) || std::holds_alternative<double>(rhs))
                return toDouble(lhs, err) * toDouble(rhs, err);
            // integer
            return safeMul(toInteger(lhs, err), toInteger(rhs, err));
        }

        case OperatorType::MINUS: {
            std::string err = "subtraction between incompatible types";

            // double
            if (std::holds_alternative<double>(lhs) || std::holds_alternative<double>(rhs))
                return toDouble(lhs, err) - toDouble(rhs, err);
            // integer
            return safeSub(toInteger(lhs, err), toInteger(rhs, err));
        }

        case OperatorType::SLASH: {
            std::string err = "division between incompatible types";

            // always double
            double ldouble = toDouble(lhs, err);
            double rdouble = toDouble(rhs, err);

            if (rdouble == 0.0)
                throw std::runtime_error("division by zero");
            return ldouble / rdouble;
        }

        case OperatorType::FLOORDIV: {
            std::string err = "floor division between incompatible types";

            // double
            if (std::holds_alternative<double>(lhs) || std::holds_alternative<double>(rhs)) {
                double ldouble = toDouble(lhs, err);
                double rdouble = toDouble(rhs, err);

                if (rdouble == 0.0)
                    throw std::runtime_error("division by zero");
                return std::floor(ldouble / rdouble);
            }

            // integer
            return safeDiv(toInteger(lhs, err), toInteger(rhs, err));
        }

        case OperatorType::MODULO: {
            std::string err = "modulo between incompatible types";

            // double
            if (std::holds_alternative<double>(lhs) || std::holds_alternative<double>(rhs)) {
                double ldouble = toDouble(lhs, err);
                double rdouble = toDouble(rhs, err);

                if (rdouble == 0.0)
                    throw std::runtime_error("modulo by zero");

                double mod = std::fmod(ldouble, rdouble);
                if ((mod != 0) && ((rdouble > 0 && mod < 0) || (rdouble < 0 && mod > 0)))
                    mod += rdouble;
                return mod;
            }

            // integer
            return safeMod(toInteger(lhs, err), toInteger(rhs, err));
        }

        case OperatorType::POWER: {
            std::string err = "power between incompatible types";

            // double
            if (std::holds_alternative<double>(lhs) || std::holds_alternative<double>(rhs)) {
                double ldouble = toDouble(lhs, err);
                double rdouble = toDouble(rhs, err);
                double power   = std::pow(ldouble, rdouble);
                if (std::isnan(power))
                    throw std::runtime_error("complex/imaginary results are not supported");

                return power;
            }

            // integer
            long long lint = toInteger(lhs, err);
            long long rint = toInteger(rhs, err);

            // negative exponent
            if (rint < 0) {
                double power = std::pow(static_cast<double>(lint), static_cast<double>(rint));
                if (std::isnan(power))
                    throw std::runtime_error("complex/imaginary results are not supported");

                return power;
            }

            return binexp(lint, rint);
        }

        default:
            throw std::runtime_error("unexpected arithmetic operator type");
    }
}

void Runner::printValue(const Value& val) {
    // printing to stdout
    std::visit(
        [this](auto&& v) -> void {
            using T = std::decay_t<decltype(v)>;

            if constexpr (std::is_same_v<T, std::monostate>)
                std::cout << "None" << '\n';
            else if constexpr (std::is_same_v<T, bool>)
                std::cout << (v ? "True" : "False") << '\n';
            else if constexpr (std::is_same_v<T, std::string>)
                printString(v);
            else
                std::cout << v << '\n';
        },
        val);
}

void Runner::printString(const std::string& str) {
    std::string result;

    for (size_t i = 0; i < str.size(); i++) {
        if (str[i] == '\\' && i + 1 < str.size()) {
            // handle escape sequences
            switch (str[i + 1]) {
                case 'n':
                    result += '\n';
                    break;
                case 't':
                    result += '\t';
                    break;
                case 'r':
                    result += '\r';
                    break;
                case '\\':
                    result += '\\';
                    break;
                case '"':
                    result += '"';
                    break;
                case '\'':
                    result += '\'';
                    break;
                case 'b':
                    result += '\b';
                    break;
                case 'f':
                    result += '\f';
                    break;
                default:
                    // unknown escape, so just print
                    result += '\\' + str[i + 1];
                    break;
            }
            i++;  // skip next character
        } else {
            result += str[i];
        }
    }

    std::cout << result << '\n';
}
