#pragma once

#include <memory>

#include <ast_node_interface.hpp>

class SymbolTable;

class Expression : public ASTNodeInterface
{

protected:
    std::shared_ptr<Symbol> symbol{nullptr};
};

class UnaryExpression : public Expression
{
public:
    UnaryExpression(Expression* expr) noexcept;

    void destroy() noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

protected:
    Expression* expression;
};

class NotExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class IncrementExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class DecrementExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class BinaryExpression : public Expression
{
public:
    BinaryExpression(Expression* left_expr, Expression* right_expr) noexcept;

    void destroy() noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

protected:
    Expression* left_expression;
    Expression* right_expression;
};

class AndExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class OrExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class LessExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class LessEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class GreaterExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class GreaterEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class EqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class NotEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class AddExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class SubExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class MulExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class DivExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class ModExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class ArgExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class CallExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class SubscriptExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class AssignmentExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;

    ASTNodeInterface* copy() const noexcept override;
};

class LeafExpression : public Expression
{
public:
    void destroy() noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;
};

class NameExpression : public LeafExpression
{
public:
    NameExpression(std::string_view _name) noexcept;

    ASTNodeInterface* copy() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

private:
    std::string name;
};

class IntExpression : public LeafExpression
{
public:
    IntExpression(int _value) noexcept;

    ASTNodeInterface* copy() const noexcept override;

private:
    int value;
};

class StrExpression : public LeafExpression
{
public:
    StrExpression(std::string_view _value) noexcept;

    ASTNodeInterface* copy() const noexcept override;

private:
    std::string value;
};