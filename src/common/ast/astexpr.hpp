#pragma once

#include <string>
#include <memory>

enum class ASTExprNodeType {
    BINARY, //binary operators
    UNARY, //unary operators
    STRING, //string literals
    NUMBER, //number literals
    BOOLEAN, //boolean literals
    NONE, //None
    REFERENCE //variable(identifier) references
};

enum class OperatorType {
    //logical
    OR, // or
    AND, // and
    NOT, // not

    //comparisons
    GREATERTHAN, // >
    GREATEREQUAL, // >=
    LESSEQUAL, // <=
    LESSERTHAN, // <
    EQEQUAL, // ==
    NOTEQUAL, // !=

    //additive/unary
    PLUS, // +
    MINUS, // -

    //multiplicative
    STAR, // *
    SLASH, // /
    FLOORDIV, // //
    MODULO, // %

    //power
    POWER, // **
};

//abstract ASTExprNode class
class ASTExprNode {
public:
    ASTExprNode(ASTExprNodeType type);
    virtual ~ASTExprNode() = default;

    //type
    ASTExprNodeType type;
};

class BinaryOperatorNode : public ASTExprNode {
public:
    BinaryOperatorNode(OperatorType op, std::unique_ptr<ASTExprNode> lhs, std::unique_ptr<ASTExprNode> rhs);

    //operator type
    OperatorType op;
    std::unique_ptr<ASTExprNode> lhs;
    std::unique_ptr<ASTExprNode> rhs;
};

class UnaryOperatorNode : public ASTExprNode {
public:
    UnaryOperatorNode(OperatorType op, std::unique_ptr<ASTExprNode> rhs);

    //operator type
    OperatorType op;
    std::unique_ptr<ASTExprNode> rhs;
};

class StringNode : public ASTExprNode {
public:
    StringNode(std::string value);

    std::string value;
};

class NumberNode : public ASTExprNode {
public:
    NumberNode(double value);

    double value;
};

class BooleanNode : public ASTExprNode {
public:
    BooleanNode(bool value);

    bool value;
};

class NoneNode : public ASTExprNode {
public:
    NoneNode();
};

class ReferenceNode : public ASTExprNode {
public:
    ReferenceNode(std::string name);

    std::string name;
};
