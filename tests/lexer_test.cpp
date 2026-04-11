#include "lexer.hpp"

#include <iostream>
#include <vector>
#include <string>

int passed = 0;
int failed = 0;
std::vector<std::string> failure_messages;

void test(std::string input_code, std::vector<std::pair<TokenType, std::string>> expected) {
    Lexer lexer(input_code);
    std::vector<Token> tokens = lexer.scan_Tokens();

    for (int i = 0; i < expected.size(); i++) {
        if (i >= tokens.size() ||
            tokens[i].type != expected[i].first ||
            tokens[i].value != expected[i].second) {

            std::cout << "F" << std::flush;

            std::string error = "FAIL: \"" + input_code + "\"\n";
            error += "  Token " + std::to_string(i) + " mismatch\n";
            error += "  Expected: " + expected[i].second + "\n";
            if (i < tokens.size()) {
                error += "  Got:      " + tokens[i].value + "\n";
            } else {
                error += "  Got:      <missing token>\n";
            }

            failure_messages.push_back(error);
            failed++;
            return;
        }
    }

    std::cout << "." << std::flush;
    passed++;
}

int main() {
    std::cout << "============================= test session starts ==============================\n";
    std::cout << "collecting items...\n";

    // ─── Keywords ────────────────────────────────────────────────────────────────

    test("if",       {{TokenType::IF,       "if"},       {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("elif",     {{TokenType::ELIF,     "elif"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("else",     {{TokenType::ELSE,     "else"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("while",    {{TokenType::WHILE,    "while"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("break",    {{TokenType::BREAK,    "break"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("continue", {{TokenType::CONTINUE, "continue"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("pass",     {{TokenType::PASS,     "pass"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("and",      {{TokenType::AND,      "and"},      {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("or",       {{TokenType::OR,       "or"},       {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("not",      {{TokenType::NOT,      "not"},      {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("in",       {{TokenType::IN,       "in"},       {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("is",       {{TokenType::IS,       "is"},       {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("True",     {{TokenType::TRUE,     "True"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("False",    {{TokenType::FALSE,    "False"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("None",     {{TokenType::NONE,     "None"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("print",    {{TokenType::PRINT,    "print"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    test("iffy",      {{TokenType::IDENTIFIER, "iffy"},      {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("whileTrue", {{TokenType::IDENTIFIER, "whileTrue"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("breakdown", {{TokenType::IDENTIFIER, "breakdown"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("andor",     {{TokenType::IDENTIFIER, "andor"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("Truthy",    {{TokenType::IDENTIFIER, "Truthy"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("None2",     {{TokenType::IDENTIFIER, "None2"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Identifiers ─────────────────────────────────────────────────────────────────

    test("x",         {{TokenType::IDENTIFIER, "x"},         {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("myVar",     {{TokenType::IDENTIFIER, "myVar"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("_private",  {{TokenType::IDENTIFIER, "_private"},  {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("__dunder",  {{TokenType::IDENTIFIER, "__dunder"},  {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("var123",    {{TokenType::IDENTIFIER, "var123"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("CamelCase", {{TokenType::IDENTIFIER, "CamelCase"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("ALL_CAPS",  {{TokenType::IDENTIFIER, "ALL_CAPS"},  {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Numbers ─────────────────────────────────────────────────────────────────

    test("0",       {{TokenType::NUMBER, "0"},       {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("1",       {{TokenType::NUMBER, "1"},       {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("42",      {{TokenType::NUMBER, "42"},      {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("1000",    {{TokenType::NUMBER, "1000"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("3.14",    {{TokenType::NUMBER, "3.14"},    {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("0.5",     {{TokenType::NUMBER, "0.5"},     {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("100.001", {{TokenType::NUMBER, "100.001"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Strings ─────────────────────────────────────────────────────────────────

    test(R"("")",           {{TokenType::STRING, "\"\""},           {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"('')",           {{TokenType::STRING, "''"},             {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"("hello")",      {{TokenType::STRING, "\"hello\""},      {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"('hello')",      {{TokenType::STRING, "'hello'"},        {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"("hello world")",{{TokenType::STRING, "\"hello world\""},{TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"("123")",        {{TokenType::STRING, "\"123\""},        {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"("it's")",       {{TokenType::STRING, "\"it's\""},       {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"('say "hi"')",   {{TokenType::STRING, "'say \"hi\"'"},   {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(R"("!@#$%")",      {{TokenType::STRING, "\"!@#$%\""},      {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Operators — single ──────────────────────────────────────────────────────

    test("+", {{TokenType::PLUS,        "+"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("-", {{TokenType::MINUS,       "-"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("*", {{TokenType::STAR,        "*"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("/", {{TokenType::SLASH,       "/"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("%", {{TokenType::MODULO,      "%"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("=", {{TokenType::ASSIGN,      "="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(">", {{TokenType::GREATERTHAN, ">"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("<", {{TokenType::LESSERTHAN,  "<"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("&", {{TokenType::AMPERSAND,   "&"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("|", {{TokenType::PIPE,        "|"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Operators — compound ────────────────────────────────────────────────────

    test("//", {{TokenType::FLOORDIV,     "//"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("**", {{TokenType::POWER,        "**"}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test(">=", {{TokenType::GREATEREQUAL, ">="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("<=", {{TokenType::LESSEQUAL,    "<="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("==", {{TokenType::EQEQUAL,      "=="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("!=", {{TokenType::NOTEQUAL,     "!="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("+=", {{TokenType::PLUSEQUAL,    "+="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("-=", {{TokenType::MINUSEQUAL,   "-="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("*=", {{TokenType::STAREQUAL,    "*="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});
    test("/=", {{TokenType::SLASHEQUAL,   "/="}, {TokenType::NEWLINE, "\n"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Comments ────────────────────────────────────────────────────────────────

    test("# this is a comment", {
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x # inline comment", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Expressions ─────────────────────────────────────────────────────────────

    test("x + y", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS,       "+"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("a * b + c", {
        {TokenType::IDENTIFIER, "a"},
        {TokenType::STAR,       "*"},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::PLUS,       "+"},
        {TokenType::IDENTIFIER, "c"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x ** 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::POWER,      "**"},
        {TokenType::NUMBER,     "2"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("10 // 3", {
        {TokenType::NUMBER,    "10"},
        {TokenType::FLOORDIV,  "//"},
        {TokenType::NUMBER,    "3"},
        {TokenType::NEWLINE,   "\n"},
        {TokenType::EOF_TOKEN, ""}});

    test("x % 2 == 0", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::MODULO,     "%"},
        {TokenType::NUMBER,     "2"},
        {TokenType::EQEQUAL,    "=="},
        {TokenType::NUMBER,     "0"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("not x and y or z", {
        {TokenType::NOT,        "not"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::AND,        "and"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::OR,         "or"},
        {TokenType::IDENTIFIER, "z"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x is None", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::IS,         "is"},
        {TokenType::NONE,       "None"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x in y", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::IN,         "in"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("(x + y)", {
        {TokenType::LPAREN,     "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS,       "+"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::RPAREN,     ")"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Assignment ──────────────────────────────────────────────────────────────

    test("x = 5", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::NUMBER,     "5"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x = \"hello\"", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::STRING,     "\"hello\""},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x = True", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::TRUE,       "True"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x += 1", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUSEQUAL,  "+="},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x -= 1", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::MINUSEQUAL, "-="},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x *= 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::STAREQUAL,  "*="},
        {TokenType::NUMBER,     "2"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x /= 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::SLASHEQUAL, "/="},
        {TokenType::NUMBER,     "2"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Print ───────────────────────────────────────────────────────────────────

    test("print(x)", {
        {TokenType::PRINT,      "print"},
        {TokenType::LPAREN,     "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::RPAREN,     ")"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("print(\"hello\", x)", {
        {TokenType::PRINT,      "print"},
        {TokenType::LPAREN,     "("},
        {TokenType::STRING,     "\"hello\""},
        {TokenType::COMMA,      ","},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::RPAREN,     ")"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Newline ─────────────────────────────────────────────────────────────────

    test("x\ny", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x = 1\ny = 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::ASSIGN,     "="},
        {TokenType::NUMBER,     "2"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── if / elif / else ────────────────────────────────────────────────────────

    test("if x:\n    pass", {
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    test("if x:\n    pass\nelse:\n    pass", {
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::ELSE,       "else"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    test("if x:\n    pass\nelif y:\n    pass\nelse:\n    pass", {
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::ELIF,       "elif"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::ELSE,       "else"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    test("if x > 0:\n    y = 1", {
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::GREATERTHAN,">"},
        {TokenType::NUMBER,     "0"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::ASSIGN,     "="},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    test("if x:\n    if y:\n        pass", {
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── while / break / continue ────────────────────────────────────────────────

    test("while True:\n    pass", {
        {TokenType::WHILE,    "while"},
        {TokenType::TRUE,     "True"},
        {TokenType::COLON,    ":"},
        {TokenType::NEWLINE,  "\n"},
        {TokenType::INDENT,   ""},
        {TokenType::PASS,     "pass"},
        {TokenType::NEWLINE,  "\n"},
        {TokenType::DEDENT,   ""},
        {TokenType::EOF_TOKEN,""}});

    test("while True:\n    break", {
        {TokenType::WHILE,    "while"},
        {TokenType::TRUE,     "True"},
        {TokenType::COLON,    ":"},
        {TokenType::NEWLINE,  "\n"},
        {TokenType::INDENT,   ""},
        {TokenType::BREAK,    "break"},
        {TokenType::NEWLINE,  "\n"},
        {TokenType::DEDENT,   ""},
        {TokenType::EOF_TOKEN,""}});

    test("while True:\n    continue", {
        {TokenType::WHILE,    "while"},
        {TokenType::TRUE,     "True"},
        {TokenType::COLON,    ":"},
        {TokenType::NEWLINE,  "\n"},
        {TokenType::INDENT,   ""},
        {TokenType::CONTINUE, "continue"},
        {TokenType::NEWLINE,  "\n"},
        {TokenType::DEDENT,   ""},
        {TokenType::EOF_TOKEN,""}});

    test("while x < 10:\n    x += 1", {
        {TokenType::WHILE,      "while"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::LESSERTHAN, "<"},
        {TokenType::NUMBER,     "10"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUSEQUAL,  "+="},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    test("while True:\n    if x == 0:\n        break\n    continue", {
        {TokenType::WHILE,      "while"},
        {TokenType::TRUE,       "True"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::EQEQUAL,    "=="},
        {TokenType::NUMBER,     "0"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::BREAK,      "break"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::CONTINUE,   "continue"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Indentation ─────────────────────────────────────────────────────────────

    test("if x:\n    if y:\n        pass\nx", {
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PASS,       "pass"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::DEDENT,     ""},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Mixed: realistic snippets ───────────────────────────────────────────────

    test("if x % 2 == 0:\n    x = x // 2\nelse:\n    x = x * 3 + 1", {
        {TokenType::IF,         "if"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::MODULO,     "%"},
        {TokenType::NUMBER,     "2"},
        {TokenType::EQEQUAL,    "=="},
        {TokenType::NUMBER,     "0"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::FLOORDIV,   "//"},
        {TokenType::NUMBER,     "2"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::ELSE,       "else"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::STAR,       "*"},
        {TokenType::NUMBER,     "3"},
        {TokenType::PLUS,       "+"},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    test("while x > 0:\n    print(x)\n    x -= 1", {
        {TokenType::WHILE,      "while"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::GREATERTHAN,">"},
        {TokenType::NUMBER,     "0"},
        {TokenType::COLON,      ":"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::INDENT,     ""},
        {TokenType::PRINT,      "print"},
        {TokenType::LPAREN,     "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::RPAREN,     ")"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::MINUSEQUAL, "-="},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::DEDENT,     ""},
        {TokenType::EOF_TOKEN,  ""}});

    test("print(\"hello\\n\", x)", {
        {TokenType::PRINT,      "print"},
        {TokenType::LPAREN,     "("},
        {TokenType::STRING,     "\"hello\\n\""},
        {TokenType::COMMA,      ","},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::RPAREN,     ")"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::EOF_TOKEN,  ""}});

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
