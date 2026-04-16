#include <fstream>
#include <iostream>
#include <sstream>
#include <string>

#include "lexer.hpp"
#include "runner.hpp"
#include "stmtparser.hpp"

int main(int argc, char* argv[]) {
    if (argc < 2) {
        std::cerr << "Usage: " << argv[0] << " <filename>\n";
        return 1;
    }

    std::string filename = argv[1];

    std::ifstream file(filename, std::ios::in | std::ios::binary);
    if (!file) {
        std::cerr << "Error: Could not open file " << filename << '\n';
        return 1;
    }

    std::ostringstream ss;
    ss << file.rdbuf();
    std::string source = ss.str();
    Lexer lexer(source);
    StmtParser parser(lexer.scan_Tokens());

    Runner runner;
    runner.runStmt(*parser.parseProgram());
    return 0;
}
