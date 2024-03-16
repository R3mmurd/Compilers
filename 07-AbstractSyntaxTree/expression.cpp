#include <expression.hpp>

UnaryExpression::UnaryExpression(Expression* expr) noexcept
    : expression{expr} {}

void UnaryExpression::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
}

BinaryExpression::BinaryExpression(Expression* left_expr, Expression* right_expr) noexcept
    : left_expression{left_expr}, right_expression{right_expr} {}

void BinaryExpression::destroy() noexcept
{
    this->left_expression->destroy();
    delete this->left_expression;

    if (this->right_expression != nullptr)
    {
        this->right_expression->destroy();
        delete this->right_expression;
    }
}

void LeafExpression::destroy() noexcept {}

NameExpression::NameExpression(std::string_view _name) noexcept
    : name{_name} {}

IntExpression::IntExpression(int _value) noexcept
    : value{_value} {}

StrExpression::StrExpression(std::string_view _value) noexcept
    : value{_value} {}