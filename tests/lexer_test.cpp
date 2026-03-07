#include <iostream>
#include "lexer/lexer.hpp"

int passed = 0;
int failed = 0;

void test(std::string input_code, std::vector<std::pair<TokenType, std::string>> expected) {
    Lexer lexer(input_code);
    std::vector<Token> tokens = lexer.scan_Tokens();

    for (int i=0; i<expected.size(); i++) {
        if (tokens[i].type != expected[i].first || tokens[i].value != expected[i].second) {  // checks each token
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
    // numbers
    test("123",   {{TokenType::NUMBER, "123"}, {TokenType::EOF_TOKEN, ""}});
    test("12.34", {{TokenType::NUMBER, "12.34"}, {TokenType::EOF_TOKEN, ""}});
    test("1e10",  {{TokenType::NUMBER, "1e10"}, {TokenType::EOF_TOKEN, ""}});
    test(".5",    {{TokenType::NUMBER, ".5"}, {TokenType::EOF_TOKEN, ""}});

    // strings
    test("\"hello\"", {{TokenType::STRING, "\"hello\""}, {TokenType::EOF_TOKEN, ""}});

    // identifiers
    test("foo",   {{TokenType::IDENTIFIER, "foo"}, {TokenType::EOF_TOKEN, ""}});
    test("_foo",  {{TokenType::IDENTIFIER, "_foo"}, {TokenType::EOF_TOKEN, ""}});

    // keywords
    test("if",    {{TokenType::IF, "if"}, {TokenType::EOF_TOKEN, ""}});

    // operators
    test("==",    {{TokenType::EQEQUAL, "=="}, {TokenType::EOF_TOKEN, ""}});
    test("+=",    {{TokenType::PLUSEQUAL, "+="}, {TokenType::EOF_TOKEN, ""}});
    test("**",    {{TokenType::POWER, "**"}, {TokenType::EOF_TOKEN, ""}});

    // comments
    test("x # comment", {{TokenType::IDENTIFIER, "x"}, {TokenType::EOF_TOKEN, ""}});

    std::cout << "\n" << passed << " passed, " << failed << " failed\n";
    return failed > 0 ? 1 : 0;
}
