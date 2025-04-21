#pragma once

#include <ast_node_interface.hpp>

class Statement : public ASTNodeInterface
{
    // Empty
};

class DeclarationStatement : public Statement
{
public:
    DeclarationStatement(Declaration* decl) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Declaration* declaration;
};

class ExpressionStatement : public Statement
{
public:
    ExpressionStatement(Expression* expr) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;

protected:
    Expression* expression;
};

class IfElseStatement : public ExpressionStatement
{
public:
    IfElseStatement(Expression* ctrl_expr, const Body& _body, const Body& _else_body = Body{}) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Body body;
    Body else_body;
};

class ForStatement : public ExpressionStatement
{
public:
    ForStatement(Expression* init_expr, Expression* ctrl_expr, Expression* next_expr, const Body& _body) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Expression* init_expression;
    Expression* next_expression;
    Body body;
};

class BlockStatement : public Statement
{
public:
    BlockStatement(const Body& _body) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Body body;
};

class PrintStatement : public ExpressionStatement
{
public:
    using ExpressionStatement::ExpressionStatement;
    
    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;
};

class ReturnStatement : public ExpressionStatement
{
public:
    using ExpressionStatement::ExpressionStatement;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;
};

