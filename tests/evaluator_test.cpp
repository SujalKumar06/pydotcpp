#include <iostream>
#include <lexer.hpp>
#include <runner.hpp>
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
    TestRunner() {
        std::cout << "============================= test session starts ==============================\n";
        std::cout << "collecting items...\n";
    }

    void pass() {
        std::cout << "." << std::flush;
        passed++;
    }

    void fail(const std::string& test_name, const std::string& details) {
        std::cout << "F" << std::flush;
        std::string full_message = "FAIL: \"" + test_name + "\"\n" + details;
        failure_messages.push_back(full_message);
        failed++;
    }

    int finish() {
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
};

//----------------------------TEST_TEMPLATE CODE-------------------------------

TestRunner test_runner;

// Runs input_code through the full pipeline (lexer -> parser -> runner),
// captures stdout, and compares it against expected_output.
void test(const std::string& input_code, const std::string& expected_output) {
    // Redirect cout to a stringstream so we can capture print() output
    std::ostringstream captured;
    std::streambuf* old_buf = std::cout.rdbuf(captured.rdbuf());

    try {
        Lexer lexer(input_code);
        StmtParser parser(lexer.scan_Tokens());
        Runner runner;
        runner.runStmt(*parser.parseProgram());

        std::cout.rdbuf(old_buf);  // restore before any pass/fail output

        std::string got = captured.str();
        if (got == expected_output) {
            test_runner.pass();
        } else {
            std::string details = "  Expected: " + expected_output + "\n" +
                                  "  Got:      " + got + "\n";
            test_runner.fail(input_code, details);
        }
    } catch (const std::exception& e) {
        std::cout.rdbuf(old_buf);
        std::string details = "  Exception: " + std::string(e.what()) + "\n" +
                              "  Expected:  " + expected_output + "\n";
        test_runner.fail(input_code, details);
    }
}

// Checks that running input_code throws a runtime_error containing error_substr.
void test_error(const std::string& input_code, const std::string& error_substr) {
    std::ostringstream captured;
    std::streambuf* old_buf = std::cout.rdbuf(captured.rdbuf());

    try {
        Lexer lexer(input_code);
        StmtParser parser(lexer.scan_Tokens());
        Runner runner;
        runner.runStmt(*parser.parseProgram());

        std::cout.rdbuf(old_buf);
        test_runner.fail(input_code, "  Expected exception containing: \"" + error_substr +
                                         "\"\n  But no exception was thrown.\n");
    } catch (const std::exception& e) {
        std::cout.rdbuf(old_buf);
        std::string what = e.what();
        if (what.find(error_substr) != std::string::npos) {
            test_runner.pass();
        } else {
            test_runner.fail(input_code,
                             "  Expected exception containing: \"" + error_substr + "\"\n" +
                                 "  Got exception:               \"" + what + "\"\n");
        }
    }
}

int main() {
    // ─── 1. Print literals ───────────────────────────────────────────────────
    test("print(42)",          "42\n");
    test("print(0)",           "0\n");
    test("print(3.14)",        "3.14\n");
    test("print(True)",        "True\n");
    test("print(False)",       "False\n");
    test("print(None)",        "None\n");
    test("print(\"hello\")",   "hello\n");
    test("print('world')",     "world\n");
    test("print(\"\")",        "\n");

    // ─── 2. Variables & assignment ───────────────────────────────────────────
    test("x = 10\nprint(x)",           "10\n");
    test("x = \"hi\"\nprint(x)",       "hi\n");
    test("x = True\nprint(x)",         "True\n");
    test("x = None\nprint(x)",         "None\n");
    test("x = 1\ny = 2\nprint(x)\nprint(y)", "1\n2\n");
    test("a = 5\nb = a\nprint(b)",     "5\n");

    // ─── 3. Arithmetic ───────────────────────────────────────────────────────
    test("print(1 + 2)",      "3\n");
    test("print(10 - 3)",     "7\n");
    test("print(4 * 5)",      "20\n");
    test("print(10 / 4)",     "2.5\n");
    test("print(10 // 3)",    "3\n");
    test("print(10 % 3)",     "1\n");
    test("print(2 ** 8)",     "256\n");
    test("print(1 + 2 * 3)",  "7\n");
    test("print((1 + 2) * 3)", "9\n");
    test("print(-5)",         "-5\n");
    test("print(- -3)",       "3\n");

    // ─── 4. Compound assignment ──────────────────────────────────────────────
    test("x = 10\nx += 5\nprint(x)",  "15\n");
    test("x = 10\nx -= 3\nprint(x)",  "7\n");
    test("x = 4\nx *= 3\nprint(x)",   "12\n");
    test("x = 10\nx /= 4\nprint(x)",  "2.5\n");
    test("x = 10\nx //= 3\nprint(x)", "3\n");
    test("x = 10\nx %= 3\nprint(x)",  "1\n");
    test("x = 2\nx **= 4\nprint(x)",  "16\n");

    // ─── 5. Comparisons ──────────────────────────────────────────────────────
    test("print(1 == 1)",   "True\n");
    test("print(1 == 2)",   "False\n");
    test("print(1 != 2)",   "True\n");
    test("print(1 != 1)",   "False\n");
    test("print(3 > 2)",    "True\n");
    test("print(2 > 3)",    "False\n");
    test("print(2 >= 2)",   "True\n");
    test("print(1 >= 2)",   "False\n");
    test("print(1 < 2)",    "True\n");
    test("print(2 < 1)",    "False\n");
    test("print(2 <= 2)",   "True\n");
    test("print(3 <= 2)",   "False\n");

    // ─── 6. Logical operators ────────────────────────────────────────────────
    test("print(True and True)",   "True\n");
    test("print(True and False)",  "False\n");
    test("print(False and True)",  "False\n");
    test("print(False or True)",   "True\n");
    test("print(False or False)",  "False\n");
    test("print(not True)",        "False\n");
    test("print(not False)",       "True\n");
    test("print(not 0)",           "True\n");
    test("print(not 1)",           "False\n");

    // ─── 7. String operations ────────────────────────────────────────────────
    test("print(\"hello\" + \" world\")", "hello world\n");
    test("print(\"ha\" * 3)",             "hahaha\n");
    test("print(3 * \"ab\")",             "ababab\n");
    test("x = \"abc\"\nprint(x[0])",      "a\n");
    test("x = \"abc\"\nprint(x[2])",      "c\n");
    test("x = \"abc\"\nprint(x[-1])",     "c\n");
    test("print(\"hello\" == \"hello\")", "True\n");
    test("print(\"hello\" == \"world\")", "False\n");

    // ─── 8. Escape sequences in strings ─────────────────────────────────────
    test("print(\"line1\\nline2\")", "line1\nline2\n");
    test("print(\"a\\tb\")",         "a\tb\n");

    // ─── 9. If / elif / else ─────────────────────────────────────────────────
    test("if True:\n    print(1)", "1\n");
    test("if False:\n    print(1)", "");
    test("if True:\n    print(1)\nelse:\n    print(2)", "1\n");
    test("if False:\n    print(1)\nelse:\n    print(2)", "2\n");

    test("x = 5\nif x > 3:\n    print(\"big\")\nelse:\n    print(\"small\")", "big\n");
    test("x = 1\nif x > 3:\n    print(\"big\")\nelse:\n    print(\"small\")", "small\n");

    test("x = 5\n"
         "if x > 10:\n    print(\"high\")\n"
         "elif x > 3:\n    print(\"mid\")\n"
         "else:\n    print(\"low\")",
         "mid\n");

    test("x = 1\n"
         "if x > 10:\n    print(\"high\")\n"
         "elif x > 3:\n    print(\"mid\")\n"
         "else:\n    print(\"low\")",
         "low\n");

    // ─── 10. While loops ─────────────────────────────────────────────────────
    test("x = 0\nwhile x < 3:\n    print(x)\n    x += 1", "0\n1\n2\n");

    test("x = 5\nwhile x > 0:\n    x -= 1\nprint(x)", "0\n");

    test("x = 0\nwhile x < 5:\n    x += 1\n    if x == 3:\n        break\nprint(x)", "3\n");

    test("x = 0\n"
         "while x < 5:\n"
         "    x += 1\n"
         "    if x == 3:\n"
         "        continue\n"
         "    print(x)",
         "1\n2\n4\n5\n");

    // ─── 11. Nested control flow ─────────────────────────────────────────────
    test("x = 0\n"
         "while x < 3:\n"
         "    if x == 1:\n"
         "        print(\"one\")\n"
         "    else:\n"
         "        print(\"other\")\n"
         "    x += 1",
         "other\none\nother\n");

    test("i = 0\n"
         "while i < 3:\n"
         "    j = 0\n"
         "    while j < 3:\n"
         "        if i == j:\n"
         "            print(i)\n"
         "        j += 1\n"
         "    i += 1",
         "0\n1\n2\n");

    // ─── 12. Multiple print statements ──────────────────────────────────────
    test("print(1)\nprint(2)\nprint(3)", "1\n2\n3\n");
    test("x = 10\ny = 20\nprint(x + y)", "30\n");

    // ─── 13. Functions ───────────────────────────────────────────────────────
    test("def add(a, b):\n    return a + b\nprint(add(3, 4))", "7\n");

    test("def square(x):\n    return x * x\nprint(square(5))", "25\n");

    test("def greet(name):\n    print(\"hello\")\ngreet(\"world\")", "hello\n");

    test("def factorial(n):\n"
         "    if n <= 1:\n"
         "        return 1\n"
         "    return n * factorial(n - 1)\n"
         "print(factorial(5))",
         "120\n");

    test("def counter(n):\n"
         "    x = 0\n"
         "    while x < n:\n"
         "        print(x)\n"
         "        x += 1\n"
         "counter(3)",
         "0\n1\n2\n");

    // ─── 14. Lists ───────────────────────────────────────────────────────────
    test("x = [1, 2, 3]\nprint(x[0])", "1\n");
    test("x = [1, 2, 3]\nprint(x[2])", "3\n");
    test("x = [1, 2, 3]\nprint(x[-1])", "3\n");
    test("x = [1, 2, 3]\nx[0] = 99\nprint(x[0])", "99\n");
    test("a = [1, 2]\nb = [3, 4]\nprint((a + b)[2])", "3\n");

    // ─── 15. Runtime error cases ─────────────────────────────────────────────
    test_error("print(x)", "undefined variable");
    test_error("x = 1 / 0", "division by zero");
    test_error("x = 1 // 0", "division by zero");
    test_error("x = [1, 2]\nprint(x[5])", "out of range");
    test_error("break", "break outside loop");
    test_error("continue", "continue outside loop");

    return test_runner.finish();
}
