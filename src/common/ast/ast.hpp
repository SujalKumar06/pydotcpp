#pragma once

#include <string>
#include <memory>

enum class ASTNodeType {
    BINARY, //binary operators
    UNARY, //unary operators
    STRING, //string literals
    NUMBER, //number literals
    BOOLEAN, //boolean literals
    NONE, //None
    REFERENCE //variable(identifier) references
};

enum class OperatorType {
    //assignment
    ASSIGN, // =

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
    DOUBLESLASH, // //
    MODULO, // %

    //power
    POWER, // **
};

//abstract ASTNode class
class ASTNode {
public:
    ASTNode(ASTNodeType type);
    virtual ~ASTNode() = default;

    //type
    ASTNodeType type;
};

class BinaryOperatorNode : public ASTNode {
public:
    BinaryOperatorNode(OperatorType op, std::unique_ptr<ASTNode> lhs, std::unique_ptr<ASTNode> rhs);

    //operator type
    OperatorType op;
    std::unique_ptr<ASTNode> lhs;
    std::unique_ptr<ASTNode> rhs;
};

class UnaryOperatorNode : public ASTNode {
public:
    UnaryOperatorNode(OperatorType op, std::unique_ptr<ASTNode> rhs);

    //operator type
    OperatorType op;
    std::unique_ptr<ASTNode> rhs;
};

class StringNode : public ASTNode {
public:
    StringNode(std::string value);

    std::string value;
};

class NumberNode : public ASTNode {
public:
    NumberNode(double value);

    double value;
};

class BooleanNode : public ASTNode {
public:
    BooleanNode(bool value);

    bool value;
}

class NoneNode : public ASTNode {
public:
    NoneNode();
}

class ReferenceNode : public ASTNode {
public:
    ReferenceNode(std::string name);

    std::string name;
};
