#include "exprparser.hpp"
#include "astexpr.hpp"

#include <string>

std::unordered_map<OperatorType, std::string> repr = {

}

std::string represent(const ASTExprNode& node) {
    switch (node.type) {
        case ASTExprNodeType::BINARY:
            auto bin = static_cast<const BinaryOperatorNode&>(node);
            return std::string('(') + represent(node.lhs) + std::string(' ') + std::string(' ') + represent(node.rhs) + std::string(')');
    }
}
