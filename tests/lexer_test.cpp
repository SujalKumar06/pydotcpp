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
    // ─── Keywords ────────────────────────────────────────────────────────────────

    test("if",       {{TokenType::IF,       "if"},       {TokenType::EOF_TOKEN, ""}});
    test("elif",     {{TokenType::ELIF,     "elif"},     {TokenType::EOF_TOKEN, ""}});
    test("else",     {{TokenType::ELSE,     "else"},     {TokenType::EOF_TOKEN, ""}});
    test("while",    {{TokenType::WHILE,    "while"},    {TokenType::EOF_TOKEN, ""}});
    test("break",    {{TokenType::BREAK,    "break"},    {TokenType::EOF_TOKEN, ""}});
    test("continue", {{TokenType::CONTINUE, "continue"}, {TokenType::EOF_TOKEN, ""}});
    test("pass",     {{TokenType::PASS,     "pass"},     {TokenType::EOF_TOKEN, ""}});
    test("and",      {{TokenType::AND,      "and"},      {TokenType::EOF_TOKEN, ""}});
    test("or",       {{TokenType::OR,       "or"},       {TokenType::EOF_TOKEN, ""}});
    test("not",      {{TokenType::NOT,      "not"},      {TokenType::EOF_TOKEN, ""}});
    test("in",       {{TokenType::IN,       "in"},       {TokenType::EOF_TOKEN, ""}});
    test("is",       {{TokenType::IS,       "is"},       {TokenType::EOF_TOKEN, ""}});
    test("True",     {{TokenType::TRUE,     "True"},     {TokenType::EOF_TOKEN, ""}});
    test("False",    {{TokenType::FALSE,    "False"},    {TokenType::EOF_TOKEN, ""}});
    test("None",     {{TokenType::NONE,     "None"},     {TokenType::EOF_TOKEN, ""}});
    test("print",    {{TokenType::PRINT,    "print"},    {TokenType::EOF_TOKEN, ""}});

    // Keywords must not match as part of a longer identifier
    test("iffy",      {{TokenType::IDENTIFIER, "iffy"},      {TokenType::EOF_TOKEN, ""}});
    test("whileTrue", {{TokenType::IDENTIFIER, "whileTrue"}, {TokenType::EOF_TOKEN, ""}});
    test("breakdown", {{TokenType::IDENTIFIER, "breakdown"}, {TokenType::EOF_TOKEN, ""}});
    test("andor",     {{TokenType::IDENTIFIER, "andor"},     {TokenType::EOF_TOKEN, ""}});
    test("Truthy",    {{TokenType::IDENTIFIER, "Truthy"},    {TokenType::EOF_TOKEN, ""}});
    test("None2",     {{TokenType::IDENTIFIER, "None2"},     {TokenType::EOF_TOKEN, ""}});

    // ─── Identifiers ─────────────────────────────────────────────────────────────

    test("x",         {{TokenType::IDENTIFIER, "x"},         {TokenType::EOF_TOKEN, ""}});
    test("myVar",     {{TokenType::IDENTIFIER, "myVar"},     {TokenType::EOF_TOKEN, ""}});
    test("_private",  {{TokenType::IDENTIFIER, "_private"},  {TokenType::EOF_TOKEN, ""}});
    test("__dunder",  {{TokenType::IDENTIFIER, "__dunder"},  {TokenType::EOF_TOKEN, ""}});
    test("var123",    {{TokenType::IDENTIFIER, "var123"},    {TokenType::EOF_TOKEN, ""}});
    test("CamelCase", {{TokenType::IDENTIFIER, "CamelCase"}, {TokenType::EOF_TOKEN, ""}});
    test("ALL_CAPS",  {{TokenType::IDENTIFIER, "ALL_CAPS"},  {TokenType::EOF_TOKEN, ""}});

    // ─── Numbers ─────────────────────────────────────────────────────────────────

    test("0",       {{TokenType::NUMBER, "0"},       {TokenType::EOF_TOKEN, ""}});
    test("1",       {{TokenType::NUMBER, "1"},       {TokenType::EOF_TOKEN, ""}});
    test("42",      {{TokenType::NUMBER, "42"},      {TokenType::EOF_TOKEN, ""}});
    test("1000",    {{TokenType::NUMBER, "1000"},    {TokenType::EOF_TOKEN, ""}});
    test("3.14",    {{TokenType::NUMBER, "3.14"},    {TokenType::EOF_TOKEN, ""}});
    test("0.5",     {{TokenType::NUMBER, "0.5"},     {TokenType::EOF_TOKEN, ""}});
    test("100.001", {{TokenType::NUMBER, "100.001"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Strings ─────────────────────────────────────────────────────────────────

    test(R"("")",           {{TokenType::STRING, "\"\""},           {TokenType::EOF_TOKEN, ""}});
    test(R"('')",           {{TokenType::STRING, "''"},             {TokenType::EOF_TOKEN, ""}});
    test(R"("hello")",      {{TokenType::STRING, "\"hello\""},      {TokenType::EOF_TOKEN, ""}});
    test(R"('hello')",      {{TokenType::STRING, "'hello'"},        {TokenType::EOF_TOKEN, ""}});
    test(R"("hello world")",{{TokenType::STRING, "\"hello world\""},{TokenType::EOF_TOKEN, ""}});
    test(R"("123")",        {{TokenType::STRING, "\"123\""},        {TokenType::EOF_TOKEN, ""}});
    test(R"("it's")",       {{TokenType::STRING, "\"it's\""},       {TokenType::EOF_TOKEN, ""}});
    test(R"('say "hi"')",   {{TokenType::STRING, "'say \"hi\"'"},   {TokenType::EOF_TOKEN, ""}});
    test(R"("!@#$%")",      {{TokenType::STRING, "\"!@#$%\""},      {TokenType::EOF_TOKEN, ""}});

    // ─── Operators — single ──────────────────────────────────────────────────────

    test("+", {{TokenType::PLUS,        "+"}, {TokenType::EOF_TOKEN, ""}});
    test("-", {{TokenType::MINUS,       "-"}, {TokenType::EOF_TOKEN, ""}});
    test("*", {{TokenType::STAR,        "*"}, {TokenType::EOF_TOKEN, ""}});
    test("/", {{TokenType::SLASH,       "/"}, {TokenType::EOF_TOKEN, ""}});
    test("%", {{TokenType::MODULO,      "%"}, {TokenType::EOF_TOKEN, ""}});
    test("=", {{TokenType::ASSIGN,      "="}, {TokenType::EOF_TOKEN, ""}});
    test(">", {{TokenType::GREATERTHAN, ">"}, {TokenType::EOF_TOKEN, ""}});
    test("<", {{TokenType::LESSERTHAN,  "<"}, {TokenType::EOF_TOKEN, ""}});
    test("&", {{TokenType::AMPERSAND,   "&"}, {TokenType::EOF_TOKEN, ""}});
    test("|", {{TokenType::PIPE,        "|"}, {TokenType::EOF_TOKEN, ""}});

    // ─── Operators — compound (must not be split into two single tokens) ──────────

    test("//", {{TokenType::FLOORDIV,     "//"}, {TokenType::EOF_TOKEN, ""}});
    test("**", {{TokenType::POWER,        "**"}, {TokenType::EOF_TOKEN, ""}});
    test(">=", {{TokenType::GREATEREQUAL, ">="}, {TokenType::EOF_TOKEN, ""}});
    test("<=", {{TokenType::LESSEQUAL,    "<="}, {TokenType::EOF_TOKEN, ""}});
    test("==", {{TokenType::EQEQUAL,      "=="}, {TokenType::EOF_TOKEN, ""}});
    test("!=", {{TokenType::NOTEQUAL,     "!="}, {TokenType::EOF_TOKEN, ""}});
    test("+=", {{TokenType::PLUSEQUAL,    "+="}, {TokenType::EOF_TOKEN, ""}});
    test("-=", {{TokenType::MINUSEQUAL,   "-="}, {TokenType::EOF_TOKEN, ""}});
    test("*=", {{TokenType::STAREQUAL,    "*="}, {TokenType::EOF_TOKEN, ""}});
    test("/=", {{TokenType::SLASHEQUAL,   "/="}, {TokenType::EOF_TOKEN, ""}});

    // ─── Comments ────────────────────────────────────────────────────────────────

    test("# this is a comment", {
        {TokenType::EOF_TOKEN, ""}});

    test("x # inline comment", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Expressions ─────────────────────────────────────────────────────────────

    test("x + y", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS,       "+"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::EOF_TOKEN,  ""}});

    test("a * b + c", {
        {TokenType::IDENTIFIER, "a"},
        {TokenType::STAR,       "*"},
        {TokenType::IDENTIFIER, "b"},
        {TokenType::PLUS,       "+"},
        {TokenType::IDENTIFIER, "c"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x ** 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::POWER,      "**"},
        {TokenType::NUMBER,     "2"},
        {TokenType::EOF_TOKEN,  ""}});

    test("10 // 3", {
        {TokenType::NUMBER,    "10"},
        {TokenType::FLOORDIV,  "//"},
        {TokenType::NUMBER,    "3"},
        {TokenType::EOF_TOKEN, ""}});

    test("x % 2 == 0", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::MODULO,     "%"},
        {TokenType::NUMBER,     "2"},
        {TokenType::EQEQUAL,    "=="},
        {TokenType::NUMBER,     "0"},
        {TokenType::EOF_TOKEN,  ""}});

    test("not x and y or z", {
        {TokenType::NOT,        "not"},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::AND,        "and"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::OR,         "or"},
        {TokenType::IDENTIFIER, "z"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x is None", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::IS,         "is"},
        {TokenType::NONE,       "None"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x in y", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::IN,         "in"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::EOF_TOKEN,  ""}});

    test("(x + y)", {
        {TokenType::LPAREN,     "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUS,       "+"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::RPAREN,     ")"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Assignment ──────────────────────────────────────────────────────────────

    test("x = 5", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::NUMBER,     "5"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x = \"hello\"", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::STRING,     "\"hello\""},
        {TokenType::EOF_TOKEN,  ""}});

    test("x = True", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::TRUE,       "True"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x += 1", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::PLUSEQUAL,  "+="},
        {TokenType::NUMBER,     "1"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x -= 1", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::MINUSEQUAL, "-="},
        {TokenType::NUMBER,     "1"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x *= 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::STAREQUAL,  "*="},
        {TokenType::NUMBER,     "2"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x /= 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::SLASHEQUAL, "/="},
        {TokenType::NUMBER,     "2"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Print ───────────────────────────────────────────────────────────────────

    test("print(x)", {
        {TokenType::PRINT,      "print"},
        {TokenType::LPAREN,     "("},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::RPAREN,     ")"},
        {TokenType::EOF_TOKEN,  ""}});

    test("print(\"hello\", x)", {
        {TokenType::PRINT,      "print"},
        {TokenType::LPAREN,     "("},
        {TokenType::STRING,     "\"hello\""},
        {TokenType::COMMA,      ","},
        {TokenType::IDENTIFIER, "x"},
        {TokenType::RPAREN,     ")"},
        {TokenType::EOF_TOKEN,  ""}});

    // ─── Newline ─────────────────────────────────────────────────────────────────

    test("x\ny", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::EOF_TOKEN,  ""}});

    test("x = 1\ny = 2", {
        {TokenType::IDENTIFIER, "x"},
        {TokenType::ASSIGN,     "="},
        {TokenType::NUMBER,     "1"},
        {TokenType::NEWLINE,    "\n"},
        {TokenType::IDENTIFIER, "y"},
        {TokenType::ASSIGN,     "="},
        {TokenType::NUMBER,     "2"},
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
        {TokenType::NEWLINE,    "\n"},
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

    // ─── Indentation — multiple DEDENTs on a single line ─────────────────────────

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
}
