#include "exprparser.hpp"
#include "lexer.hpp"

#include <string>
#include <iostream>

int passed = 0;
int failed = 0;
std::vector<std::string> failure_messages;

std::unordered_map<OperatorType, std::string> repr = {
    {OperatorType::OR, std::string("or")}, // or
    {OperatorType::AND, std::string("and")}, // and
    {OperatorType::NOT, std::string("not")}, // not

    //comparisons
    {OperatorType::GREATERTHAN, std::string(">")}, // >
    {OperatorType::GREATEREQUAL, std::string(">=")}, // >=
    {OperatorType::LESSEQUAL, std::string("<=")}, // <=
    {OperatorType::LESSERTHAN, std::string("<")}, // <
    {OperatorType::EQEQUAL, std::string("==")}, // ==
    {OperatorType::NOTEQUAL, std::string("!=")}, // !=

    //additive/unary
    {OperatorType::PLUS, std::string("+")}, // +
    {OperatorType::MINUS, std::string("-")}, // -

    //multiplicative
    {OperatorType::STAR, std::string("*")}, // *
    {OperatorType::SLASH, std::string("/")}, // /
    {OperatorType::FLOORDIV, std::string("//")}, // //
    {OperatorType::MODULO, std::string("%")}, // %

    //power
    {OperatorType::POWER, std::string("**")}, // **
};

std::string represent(const ASTExprNode& node) {
    switch (node.type) {
        case ASTExprNodeType::BINARY: {
            const BinaryOperatorNode& bin = static_cast<const BinaryOperatorNode&>(node);
            return std::string("(") + represent(*bin.lhs) + std::string(" ") + repr.at(bin.op) + std::string(" ") + represent(*bin.rhs) + std::string(")");
        }

        case ASTExprNodeType::UNARY: {
            const UnaryOperatorNode& un = static_cast<const UnaryOperatorNode&>(node);
            return std::string("(") + repr.at(un.op) + std::string(" ") + represent(*un.rhs) + std::string(")");
        }

        case ASTExprNodeType::STRING: {
            const StringNode& str = static_cast<const StringNode&>(node);
            return str.value;
        }

        case ASTExprNodeType::NUMBER: {
            const NumberNode& num = static_cast<const NumberNode&>(node);
            return std::to_string(num.value);
        }

        case ASTExprNodeType::BOOLEAN: {
            const BooleanNode& boole = static_cast<const BooleanNode&>(node);
            return (boole.value ? std::string("True") : std::string("False"));
        }

        case ASTExprNodeType::NONE: {
            return std::string("None");
        }

        case ASTExprNodeType::REFERENCE: {
            const ReferenceNode& ref = static_cast<const ReferenceNode&>(node);
            return ref.name;
        }

        default:
            //not supposed to happen
            throw std::runtime_error("unexpected token");
    }
}

void test(std::string input_code, std::string expected) {
    Lexer lexer(input_code);
    std::vector<Token> tokens = lexer.scan_Tokens();
    int index = 0;
    ExprParser exprparser(tokens, index);

    std::string output = represent(*exprparser.parseExpr());

    if (output == expected) {
        std::cout << "." << std::flush;
        passed++;
        return;
    }

    std::string error = "Expected: " + expected + " but got: " + output;
    failure_messages.push_back(error);
    failed++;
}

int main() {
    std::cout << "============================= test session starts ==============================\n";
    std::cout << "collecting items...\n";

    test("1+1", "(1.000000 + 1.000000)");

    test("2*3", "(2.000000 * 3.000000)");

    test("4-5", "(4.000000 - 5.000000)");

    test("6/2", "(6.000000 / 2.000000)");

    test("1+2*3", "(1.000000 + (2.000000 * 3.000000))");

    test("(1+2)*3", "((1.000000 + 2.000000) * 3.000000)");

    test("-5", "(- 5.000000)");

    test("not True", "(not True)");

    test("1==2", "(1.000000 == 2.000000)");

    test("a+3", "(a + 3.000000)");

    std::cout << "\n\n";
    if (!failure_messages.empty()) {
        std::cout << "=================================== FAILURES ===================================\n";
        for (const auto& msg : failure_messages) {
            std::cout << msg << "\n";
        }
    }

    std::cout << "========================= ";
    if (failed > 0) {
        std::cout << failed << " failed, " << passed << " passed";
    } else {
        std::cout << passed << " passed";
    }
    std::cout << " =========================\n";

    return failed > 0 ? 1 : 0;
}

