#include <string>
#include <token.hpp>

Token::Token(TokenType type, std::string val, int l, int c)
    : type(type), value(val), line(l), column(c) {}
