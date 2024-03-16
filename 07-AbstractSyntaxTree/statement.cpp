#include <statement.hpp>

#include <declaration.hpp>
#include <expression.hpp>

DeclarationStatement::DeclarationStatement(Declaration* decl) noexcept
    : declaration{decl} {}

void DeclarationStatement::destroy() noexcept
{
    this->declaration->destroy();
    delete this->declaration;
}

ExpressionStatement::ExpressionStatement(Expression* expr) noexcept
    : expression{expr} {}

void ExpressionStatement::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
}

IfElseStatement::IfElseStatement(Expression* ctrl_expr, const Body& _body, const Body& _else_body) noexcept
    : ExpressionStatement{ctrl_expr}, body{_body}, else_body{_else_body} {}

void IfElseStatement::destroy() noexcept
{
    ExpressionStatement::destroy();
    destroy_body(this->body);
    destroy_body(this->else_body);
}

ForStatement::ForStatement(Expression* init_expr, Expression* ctrl_expr, Expression* next_expr, const Body& _body) noexcept
    : ExpressionStatement{ctrl_expr}, init_expression{init_expr}, next_expression{next_expr}, body{_body} {}

void ForStatement::destroy() noexcept
{
    ExpressionStatement::destroy();
    this->init_expression->destroy();
    delete this->init_expression;
    this->next_expression->destroy();
    delete this->next_expression;
    destroy_body(this->body);
}
