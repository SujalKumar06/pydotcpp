#include "lexer/lexer.hpp"

#include <iostream>

int passed = 0;
int failed = 0;

void test(std::string input_code, std::vector<std::pair<TokenType, std::string>> expected) {
    Lexer lexer(input_code);
    std::vector<Token> tokens = lexer.scan_Tokens();

    for (int i = 0; i < expected.size(); i++) {
        if (tokens[i].type != expected[i].first ||
            tokens[i].value != expected[i].second) {  // checks each token
            std::cout << "FAIL: \"" << input_code << "\" - token " << i << " mismatch\n";
            std::cout << "expected: " << expected[i].second << "\n";
            std::cout << "got: " << tokens[i].value << "\n";
            failed++;
            return;
        }
    }
    std::cout << "PASS: \"" << input_code << "\"\n";
    passed++;
}

int main() {
    // Test assignment and operators
    test("x = 5 + 10", {{TokenType::IDENTIFIER, "x"},
                        {TokenType::ASSIGN, "="},
                        {TokenType::NUMBER, "5"},
                        {TokenType::PLUS, "+"},
                        {TokenType::NUMBER, "10"},
                        {TokenType::NEWLINE, "\n"},
                        {TokenType::EOF_TOKEN, ""}});

    // Test parentheses and keywords
    test("if (True):", {{TokenType::IF, "if"},
                        {TokenType::LPAREN, "("},
                        {TokenType::TRUE, "True"},
                        {TokenType::RPAREN, ")"},
                        {TokenType::COLON, ":"},
                        {TokenType::NEWLINE, "\n"},
                        {TokenType::EOF_TOKEN, ""}});
    // Test Indentation and Dedentation
    test("def foo():\n    pass", {{TokenType::DEF, "def"},
                                  {TokenType::IDENTIFIER, "foo"},
                                  {TokenType::LPAREN, "("},
                                  {TokenType::RPAREN, ")"},
                                  {TokenType::COLON, ":"},
                                  {TokenType::NEWLINE, "\n"},
                                  {TokenType::INDENT, ""},
                                  {TokenType::PASS, "pass"},
                                  {TokenType::NEWLINE, "\n"},
                                  {TokenType::DEDENT, ""},
                                  {TokenType::EOF_TOKEN, ""}});
    // Small Python file test (Assignment, Control Flow, and Indentation)
    test("x = 10\nif x > 5:\n    print(x)",
         {// Line 1: x = 10
          {TokenType::IDENTIFIER, "x"},
          {TokenType::ASSIGN, "="},
          {TokenType::NUMBER, "10"},
          {TokenType::NEWLINE, "\n"},

          // Line 2: if x > 5:
          {TokenType::IF, "if"},
          {TokenType::IDENTIFIER, "x"},
          {TokenType::GREATERTHAN, ">"},
          {TokenType::NUMBER, "5"},
          {TokenType::COLON, ":"},
          {TokenType::NEWLINE, "\n"},

          // Line 3:     print(x)
          {TokenType::INDENT, ""},      // Lexer sees 4 spaces and pushes to stack
          {TokenType::PRINT, "print"},  // 'print' is defined in your keywords.hpp!
          {TokenType::LPAREN, "("},
          {TokenType::IDENTIFIER, "x"},
          {TokenType::RPAREN, ")"},

          // EOF Cleanup
          {TokenType::NEWLINE, "\n"},  // Added by your isAtEnd() block
          {TokenType::DEDENT, ""},     // Added by your isAtEnd() block clearing the indent stack
          {TokenType::EOF_TOKEN, ""}});
    // numbers
    test("123",
         {{TokenType::NUMBER, "123"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("12.34",
         {{TokenType::NUMBER, "12.34"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("1e10",
         {{TokenType::NUMBER, "1e10"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(".5", {{TokenType::NUMBER, ".5"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // strings
    test(
        "\"hello\"",
        {{TokenType::STRING, "\"hello\""}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // identifiers
    test("foo",
         {{TokenType::IDENTIFIER, "foo"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("_foo",
         {{TokenType::IDENTIFIER, "_foo"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // keywords
    test("if", {{TokenType::IF, "if"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // operators
    test("==",
         {{TokenType::EQEQUAL, "=="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("+=",
         {{TokenType::PLUSEQUAL, "+="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("**", {{TokenType::POWER, "**"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // comments
    test("x # comment",
         {{TokenType::IDENTIFIER, "x"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}
