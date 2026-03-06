#pragma once

#include <string>
#include <memory>
#include <vector>

enum class ASTNodeType {
    BINARY, //binary operators
    UNARY, //unary operators
    STRING, //string literals
    NUMBER, //number literals
    BOOLEAN, //boolean literals
    NONE, //None
    REFERENCE, //variable(identifier) references
    VAR_DECL, //all declaration statements of the type x = y
    PRINT_STMT, //print(something) will all go here
    IF_STMT,    //if statement. Points to condition (BINARY or UNARY or other relevant nodes) and BLOCK
    ELIF_STMT, //elif statement. Points to BLOCK
    ELSE_STMT,  //else statement. Points to BLOCK
    WHILE_STMT, //while block. Points to condition and BLOCK
    FOR_STMT, //for block. Points to condition for looping and BLOCK
    BLOCK,  //reference to a vector of ASTNodes that are in a block
    PROGRAM //root node
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
};

class NoneNode : public ASTNode {
public:
    NoneNode();
};

class ReferenceNode : public ASTNode {
public:
    ReferenceNode(std::string name);

    std::string name;
};

class VarDeclNode : public ASTNode {
public:
    VarDeclNode(std::unique_ptr<ASTNode> name, std::unique_ptr<ASTNode> value);

    std::unique_ptr<ASTNode> name; //A reference node which stores the name of the variable
    std::unique_ptr<ASTNode> value; //A number, string or boolean node that stores the value inside the variable
};

class PrintStmtNode : public ASTNode {
public:
    PrintStmtNode(std::unique_ptr<ASTNode> expr);

    std::unique_ptr<ASTNode> expr;
};

class IfStmtNode : public ASTNode {
public:
    IfStmtNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> block);

    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> block;
    std::unique_ptr<ASTNode> elif_ptr;
    std::unique_ptr<ASTNode> else_ptr;
};

class ElifStmtNode : public ASTNode {
public:
    ElifStmtNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> block);

    std::unique_ptr<ASTNode> condition;
    std::unique_ptr<ASTNode> block;
    std::unique_ptr<ASTNode> elif_ptr;
    std::unique_ptr<ASTNode> else_ptr;
};

class ElseStmtNode : public ASTNode {
public:
    ElseStmtNode(std::unique_ptr<ASTNode> block);

    std::unique_ptr<ASTNode> block;
};

class WhileStmtNode : public ASTNode {
public:
    WhileStmtNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> block);

    std::unique_ptr<ASTNode> block;
    std::unique_ptr<ASTNode> condition;
};

class ForStmtNode : public ASTNode {
public:
    ForStmtNode(std::unique_ptr<ASTNode> condition, std::unique_ptr<ASTNode> block);

    std::unique_ptr<ASTNode> block;
    std::unique_ptr<ASTNode> condition;
};

class BlockNode : public ASTNode {
public:
    BlockNode() : ASTNode(ASTNodeType::BLOCK) {}

    std::vector<std::unique_ptr<ASTNode>> statements;
};

class ProgramNode : public ASTNode {
public:
    virtual ~ProgramNode() noexcept override = default;

    std::vector<std::unique_ptr<ASTNode>> statements;

    ProgramNode() : ASTNode(ASTNodeType::PROGRAM) {}
};