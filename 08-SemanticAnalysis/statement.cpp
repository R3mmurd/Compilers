#include <statement.hpp>

#include <declaration.hpp>
#include <expression.hpp>
#include <symbol_table.hpp>

DeclarationStatement::DeclarationStatement(Declaration* decl) noexcept
    : declaration{decl} {}

void DeclarationStatement::destroy() noexcept
{
    this->declaration->destroy();
    delete this->declaration;
}

ASTNodeInterface* DeclarationStatement::copy() const noexcept
{
    return new DeclarationStatement{dynamic_cast<Declaration*>(this->declaration->copy())};
}

bool DeclarationStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->declaration->resolve_name(symbol_table);
}

ExpressionStatement::ExpressionStatement(Expression* expr) noexcept
    : expression{expr} {}

void ExpressionStatement::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
}

ASTNodeInterface* ExpressionStatement::copy() const noexcept
{
    return new ExpressionStatement{dynamic_cast<Expression*>(this->expression->copy())};
}

bool ExpressionStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->expression->resolve_name(symbol_table);
}

IfElseStatement::IfElseStatement(Expression* ctrl_expr, const Body& _body, const Body& _else_body) noexcept
    : ExpressionStatement{ctrl_expr}, body{_body}, else_body{_else_body} {}

void IfElseStatement::destroy() noexcept
{
    ExpressionStatement::destroy();
    destroy_body(this->body);
    destroy_body(this->else_body);
}

ASTNodeInterface* IfElseStatement::copy() const noexcept
{
    return new IfElseStatement{
        dynamic_cast<Expression*>(this->expression->copy()),
        copy_body(this->body) ,
        copy_body(this->else_body)
    };
}

bool IfElseStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool expr_result = ExpressionStatement::resolve_name(symbol_table);

    symbol_table.enter_scope();
    bool body_result = resolve_name_body(this->body, symbol_table);
    symbol_table.exit_scope();

    symbol_table.enter_scope();
    bool else_body_result = resolve_name_body(this->else_body, symbol_table);
    symbol_table.exit_scope();

    return expr_result && body_result && else_body_result;
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

ASTNodeInterface* ForStatement::copy() const noexcept
{
    return new ForStatement{
        dynamic_cast<Expression*>(this->init_expression->copy()),
        dynamic_cast<Expression*>(this->expression->copy()),
        dynamic_cast<Expression*>(this->next_expression->copy()),
        copy_body(this->body)
    };
}

bool ForStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool partial_result = this->init_expression->resolve_name(symbol_table) &&
            ExpressionStatement::resolve_name(symbol_table) &&
            this->next_expression->resolve_name(symbol_table);

    symbol_table.enter_scope();
    bool body_result = resolve_name_body(this->body, symbol_table);
    symbol_table.exit_scope();

    return partial_result && body_result;
}

BlockStatement::BlockStatement(const Body& _body) noexcept
    : body{_body} {}

void BlockStatement::destroy() noexcept
{
    destroy_body(this->body);
}

ASTNodeInterface* BlockStatement::copy() const noexcept
{
    return new BlockStatement{copy_body(this->body)};
}

bool BlockStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    symbol_table.enter_scope();
    bool result = resolve_name_body(this->body, symbol_table);
    symbol_table.exit_scope();
    return result;
}

ASTNodeInterface* PrintStatement::copy() const noexcept
{
    return new PrintStatement{dynamic_cast<Expression*>(this->expression->copy())};
}

ASTNodeInterface* ReturnStatement::copy() const noexcept
{
    return new ReturnStatement{dynamic_cast<Expression*>(this->expression->copy())};
}