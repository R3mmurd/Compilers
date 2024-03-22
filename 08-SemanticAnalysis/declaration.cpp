#include <declaration.hpp>

#include <datatype.hpp>
#include <expression.hpp>
#include <statement.hpp>
#include <symbol_table.hpp>

Declaration::Declaration(std::string_view decl_name, Datatype* datatype) noexcept
    : name{decl_name}, type{datatype} {}

VariableDeclaration::VariableDeclaration(std::string_view var_name, Datatype* datatype, Expression* expr) noexcept
    : Declaration{var_name, datatype}, expression{expr} {}

void VariableDeclaration::destroy() noexcept
{
    if (this->expression == nullptr)
    {
        return;
    }

    this->expression->destroy();
    delete this->expression;
}

ASTNodeInterface* VariableDeclaration::copy() const noexcept
{
    return new VariableDeclaration{this->name, dynamic_cast<Datatype*>(this->type->copy())};
}

bool VariableDeclaration::resolve_name(SymbolTable& symbol_table) noexcept
{
    this->symbol = Symbol::build(this->type, this->name);

    if (this->expression != nullptr)
    {
        this->expression->resolve_name(symbol_table);
    }
    
    return symbol_table.bind(this->name, this->symbol);
}

FunctionDeclaration::FunctionDeclaration(std::string_view fct_name, Datatype* datatype, const Body& fct_body) noexcept
    : Declaration{fct_name, datatype}, body{fct_body} {}

void FunctionDeclaration::destroy() noexcept
{
    destroy_body(this->body);
}

bool FunctionDeclaration::resolve_name(SymbolTable& symbol_table) noexcept
{
    this->symbol = Symbol::build(this->type, this->name);
    bool result = symbol_table.bind(this->name, this->symbol);

    if (result && !body.empty())
    {
        symbol_table.enter_scope();
        this->type->resolve_name(symbol_table);
        resolve_name_body(this->body, symbol_table);
        symbol_table.exit_scope();
    }

    return result;
}

ASTNodeInterface* FunctionDeclaration::copy() const noexcept
{
    return new FunctionDeclaration{
        this->name, dynamic_cast<Datatype*>(this->type->copy()),
        copy_body(this->body)
    };
}
