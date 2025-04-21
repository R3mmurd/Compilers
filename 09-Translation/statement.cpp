#include <statement.hpp>

#include <datatype.hpp>
#include <declaration.hpp>
#include <expression.hpp>
#include <symbol_table.hpp>

DeclarationStatement::DeclarationStatement(Declaration* decl) noexcept
    : declaration{decl} {}

void DeclarationStatement::destroy() noexcept
{
    this->declaration->destroy();
    delete this->declaration;
    this->declaration = nullptr;
}

ASTNodeInterface* DeclarationStatement::copy() const noexcept
{
    return new DeclarationStatement{dynamic_cast<Declaration*>(this->declaration->copy())};
}

bool DeclarationStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<DeclarationStatement*>(other);
    return other_stmt != nullptr && this->declaration->equal(other_stmt->declaration);
}

std::pair<bool, Datatype*> DeclarationStatement::type_check() const noexcept
{
    return this->declaration->type_check();
}

bool DeclarationStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->declaration->resolve_name(symbol_table);
}

std::string DeclarationStatement::to_python(const std::string& ident) const noexcept
{
    return ident + declaration->to_python(ident);
}

ExpressionStatement::ExpressionStatement(Expression* expr) noexcept
    : expression{expr} {}

void ExpressionStatement::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
    this->expression = nullptr;
}

ASTNodeInterface* ExpressionStatement::copy() const noexcept
{
    return new ExpressionStatement{dynamic_cast<Expression*>(this->expression->copy())};
}

bool ExpressionStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<ExpressionStatement*>(other);
    return other_stmt != nullptr && this->expression->equal(other_stmt->expression);
}

std::pair<bool, Datatype*> ExpressionStatement::type_check() const noexcept
{
    auto expr_type = this->expression->type_check();

    if (expr_type.second != nullptr)
    {
        delete expr_type.second;
        expr_type.second = nullptr;
    }
    return expr_type;
}

bool ExpressionStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->expression->resolve_name(symbol_table);
}

std::string ExpressionStatement::to_python(const std::string& ident) const noexcept
{
    return ident + expression->to_python(ident);
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

bool IfElseStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<IfElseStatement*>(other);
    return other_stmt != nullptr && ExpressionStatement::equal(other) &&
        equal_body(this->body, other_stmt->body) && equal_body(this->else_body, other_stmt->else_body);
}

std::pair<bool, Datatype*> IfElseStatement::type_check() const noexcept
{
    auto result = ExpressionStatement::type_check().first &&
        body_type_check(this->body).first && 
        body_type_check(this->else_body).first;
    return std::make_pair(result, nullptr);
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

std::string IfElseStatement::to_python(const std::string& ident) const noexcept
{
    std::string result = ident + "if " + expression->to_python(ident) + ":\n";
    result += body_to_python(body, ident + "    ");

    if (!else_body.empty())
    {
        result += ident + "else:";
        result += body_to_python(else_body, ident + "    ");
    }

    return result;
}

ForStatement::ForStatement(Expression* init_expr, Expression* ctrl_expr, Expression* next_expr, const Body& _body) noexcept
    : ExpressionStatement{ctrl_expr}, init_expression{init_expr}, next_expression{next_expr}, body{_body} {}

void ForStatement::destroy() noexcept
{
    ExpressionStatement::destroy();
    this->init_expression->destroy();
    delete this->init_expression;
    this->init_expression = nullptr;
    this->next_expression->destroy();
    delete this->next_expression;
    this->next_expression = nullptr;
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

bool ForStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<ForStatement*>(other);
    return other_stmt != nullptr && 
        this->init_expression->equal(other_stmt->init_expression) &&
        ExpressionStatement::equal(other) &&
        this->next_expression->equal(other_stmt->next_expression) &&
        equal_body(this->body, other_stmt->body);
}

std::pair<bool, Datatype*> ForStatement::type_check() const noexcept
{
    auto init_result = this->init_expression->type_check();
    
    if (init_result.second != nullptr)
    {
        delete init_result.second;
        init_result.second = nullptr;
    }

    auto next_result = this->next_expression->type_check();
    
    if (next_result.second != nullptr)
    {
        delete next_result.second;
        next_result.second = nullptr;
    }

    auto result = init_result.first && ExpressionStatement::type_check().first && next_result.first;

    return std::make_pair(result, nullptr);
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

std::string ForStatement::to_python(const std::string& ident) const noexcept
{
    std::string result = ident + init_expression->to_python(ident) + "\n";

    result += ident + "while " + expression->to_python(ident) + ":\n";

    result += body_to_python(body, ident + "    ");

    result += ident + "    " + next_expression->to_python(ident);

    return result;
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

bool BlockStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<BlockStatement*>(other);
    return other_stmt != nullptr && equal_body(this->body, other_stmt->body);
}

std::pair<bool, Datatype*> BlockStatement::type_check() const noexcept
{
    return body_type_check(this->body);
}

bool BlockStatement::resolve_name(SymbolTable& symbol_table) noexcept
{
    symbol_table.enter_scope();
    bool result = resolve_name_body(this->body, symbol_table);
    symbol_table.exit_scope();
    return result;
}

std::string BlockStatement::to_python(const std::string& ident) const noexcept
{
    return body_to_python(body, ident + "    ");
}

ASTNodeInterface* PrintStatement::copy() const noexcept
{
    return new PrintStatement{dynamic_cast<Expression*>(this->expression->copy())};
}

bool PrintStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<PrintStatement*>(other);
    return other_stmt != nullptr && ExpressionStatement::equal(other);
}

std::string PrintStatement::to_python(const std::string& ident) const noexcept
{
    return ident + "print(" + expression->to_python(ident) + ")";
}

ASTNodeInterface* ReturnStatement::copy() const noexcept
{
    return new ReturnStatement{dynamic_cast<Expression*>(this->expression->copy())};
}

bool ReturnStatement::equal(ASTNodeInterface* other) const noexcept
{
    auto other_stmt = dynamic_cast<ReturnStatement*>(other);
    return other_stmt != nullptr && ExpressionStatement::equal(other);
}

std::string ReturnStatement::to_python(const std::string& ident) const noexcept
{
    return ident + "return " + expression->to_python(ident);
}