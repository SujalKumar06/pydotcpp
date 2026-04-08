#include <astexpr.hpp>
#include <aststmt.hpp>
#include <cmath>
#include <exprparser.hpp>
#include <iostream>
#include <lexer.hpp>
#include <sstream>
#include <stmtparser.hpp>
#include <string>
#include <vector>

//----------------------------TEST_TEMPLATE CODE-------------------------------

class TestRunner {
   private:
    int passed = 0;
    int failed = 0;
    std::vector<std::string> failure_messages;

   public:
    // Constructor prints the start header
    TestRunner() {
        std::cout
            << "============================= test session starts ==============================\n";
        std::cout << "collecting items...\n";
    }

    // Call this when a test passes
    void pass() {
        std::cout << "." << std::flush;
        passed++;
    }

    // Call this when a test fails, passing the test name/input and the error details
    void fail(const std::string& test_name, const std::string& details) {
        std::cout << "F" << std::flush;
        std::string full_message = "FAIL: \"" + test_name + "\"\n" + details;
        failure_messages.push_back(full_message);
        failed++;
    }

    // Call this at the very end of main() to print the summary and get the exit code
    int finish() {
        std::cout << "\n\n";

        if (!failure_messages.empty()) {
            std::cout << "=================================== FAILURES "
                         "===================================\n";
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

        // Return 1 if tests failed so Make registers the error, 0 if all good
        return failed > 0 ? 1 : 0;
    }
};

//----------------------------TEST_TEMPLATE CODE-------------------------------

TestRunner test_runner;

std::string convertExprASTNodeToString(ASTExprNode* node);
std::string convertStmtASTNodeToString(ASTStmtNode* node);

std::string operatorToString(OperatorType op) {
    switch (op) {
        case OperatorType::OR:
            return "or";
        case OperatorType::AND:
            return "and";
        case OperatorType::NOT:
            return "not";
        case OperatorType::GREATERTHAN:
            return ">";
        case OperatorType::GREATEREQUAL:
            return ">=";
        case OperatorType::LESSEQUAL:
            return "<=";
        case OperatorType::LESSERTHAN:
            return "<";
        case OperatorType::EQEQUAL:
            return "==";
        case OperatorType::NOTEQUAL:
            return "!=";
        case OperatorType::PLUS:
            return "+";
        case OperatorType::MINUS:
            return "-";
        case OperatorType::STAR:
            return "*";
        case OperatorType::SLASH:
            return "/";
        case OperatorType::FLOORDIV:
            return "//";
        case OperatorType::MODULO:
            return "%";
        case OperatorType::POWER:
            return "**";
        default:
            return "?";
    }
}

std::string convertExprASTNodeToString(ASTExprNode* node) {
    if (node == nullptr)
        return "nullptr";

    if (NumberNode* num = dynamic_cast<NumberNode*>(node)) {
        std::ostringstream oss;
        oss << num->value;
        return oss.str();
    }

    if (StringNode* str = dynamic_cast<StringNode*>(node)) {
        return str->value;
    }

    if (BooleanNode* bool_node = dynamic_cast<BooleanNode*>(node)) {
        if (bool_node->value)
            return "True";
        return "False";
    }

    if (NoneNode* none_node = dynamic_cast<NoneNode*>(node)) {
        return "None";
    }

    if (ReferenceNode* ref = dynamic_cast<ReferenceNode*>(node)) {
        return ref->name;
    }

    if (UnaryOperatorNode* unary = dynamic_cast<UnaryOperatorNode*>(node)) {
        return "(" + operatorToString(unary->op) + " " +
               convertExprASTNodeToString(unary->rhs.get()) + ")";
    }

    if (BinaryOperatorNode* binary = dynamic_cast<BinaryOperatorNode*>(node)) {
        return "(" + operatorToString(binary->op) + " " +
               convertExprASTNodeToString(binary->lhs.get()) + " " +
               convertExprASTNodeToString(binary->rhs.get()) + ")";
    }

    return "<UNKNOWN-EXPR-NODE>";
}

std::string convertStmtASTNodeToString(ASTStmtNode* node) {
    if (node == nullptr)
        return "nullptr";

    if (ProgramNode* prog = dynamic_cast<ProgramNode*>(node)) {
        std::string result = "(START-OF-NODES:";
        for (const auto& stmt : prog->statements) {
            result += " " + convertStmtASTNodeToString(stmt.get());
        }
        return result + " :END-OF-NODES)";
    }

    if (BlockNode* block = dynamic_cast<BlockNode*>(node)) {
        std::string result = "(START-OF-BLOCK:";
        for (const auto& stmt : block->statements) {
            result += " " + convertStmtASTNodeToString(stmt.get());
        }
        return result + " :END-OF-BLOCK)";
    }

    if (PrintStmtNode* print_stmt = dynamic_cast<PrintStmtNode*>(node)) {
        return "(PRINT: " + convertExprASTNodeToString(print_stmt->expr.get()) + " :END-OF-PRINT)";
    }

    if (VarDeclNode* var_decl = dynamic_cast<VarDeclNode*>(node)) {
        return "(VAR-DECL: VAR_NAME = " + convertExprASTNodeToString(var_decl->name.get()) +
               " VAR_VALUE = " + convertExprASTNodeToString(var_decl->value.get()) +
               " :END-OF-VAR-DECL)";
    }

    if (IfStmtNode* if_stmt = dynamic_cast<IfStmtNode*>(node)) {
        return "(IF: CONDITION = " + convertExprASTNodeToString(if_stmt->condition.get()) +
               " BLOCK = " + convertStmtASTNodeToString(if_stmt->block.get()) + " :END-OF-IF)";
    }

    if (WhileStmtNode* while_stmt = dynamic_cast<WhileStmtNode*>(node)) {
        return "(WHILE: CONDITION = " + convertExprASTNodeToString(while_stmt->condition.get()) +
               " BLOCK = " + convertStmtASTNodeToString(while_stmt->block.get()) +
               " :END-OF-WHILE)";
    }

    if (ForStmtNode* for_stmt = dynamic_cast<ForStmtNode*>(node)) {
        return "(FOR: CONDITION = " + convertExprASTNodeToString(for_stmt->condition.get()) +
               " BLOCK = " + convertStmtASTNodeToString(for_stmt->block.get()) + " :END-OF-FOR)";
    }

    return "<UNKNOWN-STMT-NODE>";
}

void test(const std::string& input_code, const std::string& expected_ast_string) {
    Lexer lexer(input_code);
    std::vector<Token> tokens = lexer.scan_Tokens();
    StmtParser parser(tokens);

    try {
        std::unique_ptr<ProgramNode> ast = parser.parseProgram();
        if (ast.get() != nullptr) {
            std::string obtained_ast_string = convertStmtASTNodeToString(ast.get());
            if (obtained_ast_string == expected_ast_string) {
                test_runner.pass();
            } else {
                std::string failDetails = "  Expected: " + expected_ast_string + "\n" +
                                          "  Got:      " + obtained_ast_string + "\n";
                test_runner.fail("TEST PROVIDED WRONG OUTPUT: " + input_code, failDetails);
            }
        } else {
            test_runner.fail("PROGRAM NODE WAS NULL: " + input_code, "NO PROGRAM NODE WAS FOUND");
        }
    } catch (const std::exception& e) {
        test_runner.fail(
            "TEST FAILED TO RUN: " + input_code,
            std::string("Exception: ") + e.what() + "\n  Expected: " + expected_ast_string + "\n");
    }
}

int main() {
    // ─── 1. Basic Variable Declarations & Literals ───────────────────────────
    test("x = 5",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = 5 :END-OF-VAR-DECL) :END-OF-NODES)");
    test("pi = 3.14",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = pi VAR_VALUE = 3.14 :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("var123 = \"hello\"",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = var123 VAR_VALUE = \"hello\" :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("flag = True",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = flag VAR_VALUE = True :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("flag = False",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = flag VAR_VALUE = False :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("empty = None",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = empty VAR_VALUE = None :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("a = b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = a VAR_VALUE = b :END-OF-VAR-DECL) :END-OF-NODES)");

    // ─── 2. Binary Math Operations ───────────────────────────────────────────
    test("x = 1 + 2",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (+ 1 2) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = 5 - 3",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (- 5 3) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = 4 * 2",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (* 4 2) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = 10 / 2",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (/ 10 2) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = 10 // 3",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (// 10 3) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = 10 % 3",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (% 10 3) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = 2 ** 3",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (** 2 3) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");

    // ─── 3. Unary Operations ─────────────────────────────────────────────────
    test("x = -5",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (- 5) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = +10",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (+ 10) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = not True",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (not True) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = not a",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (not a) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");

    // ─── 4. Comparison & Logical Operations ──────────────────────────────────
    test("x = a > b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (> a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a < b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (< a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a >= b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (>= a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a <= b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (<= a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a == b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (== a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a != b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (!= a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a and b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (and a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a or b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (or a b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");

    // ─── 5. Complex Expressions & Precedence (PEMDAS) ────────────────────────
    test("x = 1 + 2 * 3",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (+ 1 (* 2 3)) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = (1 + 2) * 3",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (* (+ 1 2) 3) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a * b + c * d",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (+ (* a b) (* c d)) "
         ":END-OF-VAR-DECL) :END-OF-NODES)");
    test("x = a and b or c",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (or (and a b) c) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = a or b and c",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (or a (and b c)) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = not a and b",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (and (not a) b) :END-OF-VAR-DECL) "
         ":END-OF-NODES)");
    test("x = (a > b) and (c < d)",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = (and (> a b) (< c d)) "
         ":END-OF-VAR-DECL) :END-OF-NODES)");


    // ─── 6. Print Statements ─────────────────────────────────────────────────
    test("print(x)", "(START-OF-NODES: (PRINT: x :END-OF-PRINT) :END-OF-NODES)");
    test("print(100)", "(START-OF-NODES: (PRINT: 100 :END-OF-PRINT) :END-OF-NODES)");
    test("print(\"debug\")", "(START-OF-NODES: (PRINT: \"debug\" :END-OF-PRINT) :END-OF-NODES)");
    test("print(a + b)", "(START-OF-NODES: (PRINT: (+ a b) :END-OF-PRINT) :END-OF-NODES)");
    test("print(x == True)", "(START-OF-NODES: (PRINT: (== x True) :END-OF-PRINT) :END-OF-NODES)");

    // ─── 7. If Statements ────────────────────────────────────────────────────
    test("if True:\n    x = 1",
         "(START-OF-NODES: (IF: CONDITION = True BLOCK = (START-OF-BLOCK: (VAR-DECL: VAR_NAME = x "
         "VAR_VALUE = 1 :END-OF-VAR-DECL) :END-OF-BLOCK) :END-OF-IF) :END-OF-NODES)");
    test("if a > 0:\n    print(a)",
         "(START-OF-NODES: (IF: CONDITION = (> a 0) BLOCK = (START-OF-BLOCK: (PRINT: a "
         ":END-OF-PRINT) :END-OF-BLOCK) :END-OF-IF) :END-OF-NODES)");
    test("if not x:\n    y = False",
         "(START-OF-NODES: (IF: CONDITION = (not x) BLOCK = (START-OF-BLOCK: (VAR-DECL: VAR_NAME = "
         "y VAR_VALUE = False :END-OF-VAR-DECL) :END-OF-BLOCK) :END-OF-IF) :END-OF-NODES)");

    // ─── 8. While Statements ─────────────────────────────────────────────────
    test("while True:\n    print(1)",
         "(START-OF-NODES: (WHILE: CONDITION = True BLOCK = (START-OF-BLOCK: (PRINT: 1 "
         ":END-OF-PRINT) :END-OF-BLOCK) :END-OF-WHILE) :END-OF-NODES)");
    test("while x < 10:\n    x = x + 1",
         "(START-OF-NODES: (WHILE: CONDITION = (< x 10) BLOCK = (START-OF-BLOCK: (VAR-DECL: "
         "VAR_NAME = x VAR_VALUE = (+ x 1) :END-OF-VAR-DECL) :END-OF-BLOCK) :END-OF-WHILE) "
         ":END-OF-NODES)");
    test("while flag == False:\n    print(wait)",
         "(START-OF-NODES: (WHILE: CONDITION = (== flag False) BLOCK = (START-OF-BLOCK: (PRINT: "
         "wait :END-OF-PRINT) :END-OF-BLOCK) :END-OF-WHILE) :END-OF-NODES)");


    // ─── 9. Multi-line Programs ──────────────────────────────────────────────
    test("x = 1\ny = 2",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = x VAR_VALUE = 1 :END-OF-VAR-DECL) (VAR-DECL: "
         "VAR_NAME = y VAR_VALUE = 2 :END-OF-VAR-DECL) :END-OF-NODES)");

    test("a = 1\nb = a + 2\nprint(b)",
         "(START-OF-NODES: (VAR-DECL: VAR_NAME = a VAR_VALUE = 1 :END-OF-VAR-DECL) (VAR-DECL: "
         "VAR_NAME = b VAR_VALUE = (+ a 2) :END-OF-VAR-DECL) (PRINT: b :END-OF-PRINT) "
         ":END-OF-NODES)");



    // ─── 10. Multi-statement Blocks ──────────────────────────────────────────
    test("if True:\n    x = 1\n    y = 2\n    print(x)",
         "(START-OF-NODES: (IF: CONDITION = True BLOCK = (START-OF-BLOCK: (VAR-DECL: VAR_NAME = x "
         "VAR_VALUE = 1 :END-OF-VAR-DECL) (VAR-DECL: VAR_NAME = y VAR_VALUE = 2 :END-OF-VAR-DECL) "
         "(PRINT: x :END-OF-PRINT) :END-OF-BLOCK) :END-OF-IF) :END-OF-NODES)");

    test("while active:\n    print(x)\n    active = False",
         "(START-OF-NODES: (WHILE: CONDITION = active BLOCK = (START-OF-BLOCK: (PRINT: x "
         ":END-OF-PRINT) (VAR-DECL: VAR_NAME = active VAR_VALUE = False :END-OF-VAR-DECL) "
         ":END-OF-BLOCK) :END-OF-WHILE) :END-OF-NODES)");

    // ─── 11. Nested Control Flow ─────────────────────────────────────────────
    test("if a:\n    if b:\n        print(c)",
         "(START-OF-NODES: (IF: CONDITION = a BLOCK = (START-OF-BLOCK: (IF: CONDITION = b BLOCK = "
         "(START-OF-BLOCK: (PRINT: c :END-OF-PRINT) :END-OF-BLOCK) :END-OF-IF) :END-OF-BLOCK) "
         ":END-OF-IF) :END-OF-NODES)");

    test("while True:\n    if x == 0:\n        print(done)",
         "(START-OF-NODES: (WHILE: CONDITION = True BLOCK = (START-OF-BLOCK: (IF: CONDITION = (== "
         "x 0) BLOCK = (START-OF-BLOCK: (PRINT: done :END-OF-PRINT) :END-OF-BLOCK) :END-OF-IF) "
         ":END-OF-BLOCK) :END-OF-WHILE) :END-OF-NODES)");

    test("if out:\n    while something:\n        x = 1\n    print(out)",
         "(START-OF-NODES: (IF: CONDITION = out BLOCK = (START-OF-BLOCK: (WHILE: CONDITION = something "
         "BLOCK = (START-OF-BLOCK: (VAR-DECL: VAR_NAME = x VAR_VALUE = 1 :END-OF-VAR-DECL) "
         ":END-OF-BLOCK) :END-OF-WHILE) (PRINT: out :END-OF-PRINT) :END-OF-BLOCK) :END-OF-IF) "
         ":END-OF-NODES)");

    return test_runner.finish();
}