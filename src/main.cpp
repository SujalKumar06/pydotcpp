#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>

#include "lexer.hpp"
#include "stmtparser.hpp"
#include "runner.hpp"

// Core function to execute a string of source code
void run(const std::string& source_code, Runner& runner) {
    try {
        // 1. Lexical Analysis
        Lexer lexer(source_code);
        std::vector<Token> tokens = lexer.scan_Tokens();

        // 2. Parsing
        StmtParser parser(tokens);
        auto ast = parser.parseProgram();

        // 3. Execution
        if (ast) {
            runner.runStmt(*ast);
        }
    } catch (const std::exception& e) {
        std::cerr << "Runtime Error: " << e.what() << '\n';
    }
}

// Executes a source file
void runFile(const std::string& path) {
    std::ifstream file(path);
    if (!file.is_open()) {
        std::cerr << "Could not open file: " << path << '\n';
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();
    std::string source_code = buffer.str();

    Runner runner;
    run(source_code, runner);
}

// Starts an interactive Read-Eval-Print Loop (REPL)
void runPrompt() {
    Runner runner; // Instantiated here so the environment (variables) persists across inputs
    std::string line;

    std::cout << "Python-style Interpreter [C++]\n";
    std::cout << "Type 'exit()' to quit.\n";

    while (true) {
        std::cout << ">>> ";
        if (!std::getline(std::cin, line)) {
            break; // Handle EOF (Ctrl+D)
        }
        if (line == "exit()") {
            break;
        }
        
        // Passing a newline ensures the Lexer properly processes statements 
        // that rely on NEWLINE tokens.
        run(line + "\n", runner);
    }
}

int main(int argc, char* argv[]) {
    if (argc > 2) {
        std::cout << "Usage: executable [script.py]\n";
        return 1;
    } else if (argc == 2) {
        runFile(argv[1]);
    } else {
        runPrompt();
    }

    return 0;
}