#pragma once

#include <vector>
#include <memory>

#include "astexpr.hpp"

enum class ASTStmtNodeType {
    VAR_DECL, //all declaration statements of the type x = y
    PRINT_STMT, //print(something) will all go here
    IF_STMT,    //if statement. Points to condition (BINARY or UNARY or other relevant nodes) and BLOCK
    ELIF_STMT, //elif statement. Points to BLOCK
    ELSE_STMT,  //else statement. Points to BLOCK
    WHILE_STMT, //while block. Points to condition and BLOCK
    FOR_STMT, //for block. Points to condition for looping and BLOCK
    BLOCK,  //reference to a vector of ASTNodes that are in a block
    PROGRAM, //root node
    BREAK_STMT, // break
    CONTINUE_STMT, // continue
    };

//abstract ASTStmtNode class
class ASTStmtNode {
public:
    ASTStmtNode(ASTStmtNodeType type);
    virtual ~ASTStmtNode() = default;

    //type
    ASTStmtNodeType type;
};

class VarDeclNode : public ASTStmtNode {
public:
    VarDeclNode(std::unique_ptr<ASTExprNode> name, std::unique_ptr<ASTExprNode> value);

    std::unique_ptr<ASTExprNode> name; //A reference node which stores the name of the variable
    std::unique_ptr<ASTExprNode> value; //A number, string or boolean node that stores the value inside the variable
};

class PrintStmtNode : public ASTStmtNode {
public:
    PrintStmtNode(std::unique_ptr<ASTExprNode> expr);

    std::unique_ptr<ASTExprNode> expr;
};

class IfStmtNode : public ASTStmtNode {
public:
    IfStmtNode(std::unique_ptr<ASTExprNode> condition, std::unique_ptr<ASTStmtNode> block);

    std::unique_ptr<ASTExprNode> condition;
    std::unique_ptr<ASTStmtNode> block;
    std::unique_ptr<ASTStmtNode> elif_ptr;
    std::unique_ptr<ASTStmtNode> else_ptr;
};

class ElifStmtNode : public ASTStmtNode {
public:
    ElifStmtNode(std::unique_ptr<ASTExprNode> condition, std::unique_ptr<ASTStmtNode> block);

    std::unique_ptr<ASTExprNode> condition;
    std::unique_ptr<ASTStmtNode> block;
    std::unique_ptr<ASTStmtNode> elif_ptr;
    std::unique_ptr<ASTStmtNode> else_ptr;
};

class ElseStmtNode : public ASTStmtNode {
public:
    ElseStmtNode(std::unique_ptr<ASTStmtNode> block);

    std::unique_ptr<ASTStmtNode> block;
};

class WhileStmtNode : public ASTStmtNode {
public:
    WhileStmtNode(std::unique_ptr<ASTExprNode> condition, std::unique_ptr<ASTStmtNode> block);

    std::unique_ptr<ASTExprNode> condition;
    std::unique_ptr<ASTStmtNode> block;
};

class ForStmtNode : public ASTStmtNode {
public:
    ForStmtNode(std::unique_ptr<ASTExprNode> condition, std::unique_ptr<ASTStmtNode> block);

    std::unique_ptr<ASTExprNode> condition;
    std::unique_ptr<ASTStmtNode> block;
};

class BlockNode : public ASTStmtNode {
public:
    BlockNode();

    std::vector<std::unique_ptr<ASTStmtNode>> statements;
};

class ProgramNode : public ASTStmtNode {
public:
    ProgramNode();

    std::vector<std::unique_ptr<ASTStmtNode>> statements;
};

class BreakStmtNode : public ASTStmtNode{
    public:
    BreakStmtNode();
};

class ContinueStmtNode : public ASTStmtNode{
    public:
    ContinueStmtNode();
};
