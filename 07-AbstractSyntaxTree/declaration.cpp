#include <declaration.hpp>

#include <expression.hpp>
#include <statement.hpp>

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

FunctionDeclaration::FunctionDeclaration(std::string_view fct_name, Datatype* datatype, const Body& fct_body) noexcept
    : Declaration{fct_name, datatype}, body{fct_body} {}

void FunctionDeclaration::destroy() noexcept
{
    destroy_body(this->body);
}