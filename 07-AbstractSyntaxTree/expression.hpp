#pragma once

#include <ast_node_interface.hpp>

class Expression : public ASTNodeInterface
{
    // Empty
};

class UnaryExpression : public Expression
{
public:
    UnaryExpression(Expression* expr) noexcept;

    void destroy() noexcept override;

private:
    Expression* expression;
};

class NotExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;
};

class IncrementExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;
};

class DecrementExpression : public UnaryExpression
{
public:
    using UnaryExpression::UnaryExpression;
};

class BinaryExpression : public Expression
{
public:
    BinaryExpression(Expression* left_expr, Expression* right_expr) noexcept;

    void destroy() noexcept override;

protected:
    Expression* left_expression;
    Expression* right_expression;
};

class AndExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class OrExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class LessExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class LessEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class GreaterExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class GreaterEqExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class EqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class NotEqualExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class AddExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class SubExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class MulExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class DivExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class ModExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class ArgExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class CallExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class SubscriptExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class AssignmentExpression : public BinaryExpression
{
public:
    using BinaryExpression::BinaryExpression;
};

class LeafExpression : public Expression
{
public:
    void destroy() noexcept override;
};

class NameExpression : public LeafExpression
{
public:
    NameExpression(std::string_view _name) noexcept;

private:
    std::string name;
};

class IntExpression : public LeafExpression
{
public:
    IntExpression(int _value) noexcept;

private:
    int value;
};

class StrExpression : public LeafExpression
{
public:
    StrExpression(std::string_view _value) noexcept;

private:
    std::string value;
};