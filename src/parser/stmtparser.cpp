#include "stmtparser.hpp"

#include <vector>

#include "token.hpp"

StmtParser::StmtParser(std::vector<Token> tokens)
    : tokens(std::move(tokens)), exprparser(this->tokens, index) {}
