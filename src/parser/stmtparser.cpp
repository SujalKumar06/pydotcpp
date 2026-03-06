#include <vector>

#include "token.hpp"
#include "stmtparser.hpp"

StmtParser::StmtParser(std::vector<Token> tokens) : tokens(std::move(tokens)), exprparser(this->tokens, index) {}
