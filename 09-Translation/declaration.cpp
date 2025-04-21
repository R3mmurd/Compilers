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
    if (this->expression != nullptr)
    {
        this->expression->destroy();
        delete this->expression;   
        this->expression = nullptr;
    }
}

ASTNodeInterface* VariableDeclaration::copy() const noexcept
{
    Expression* expr = this->expression == nullptr ? nullptr : dynamic_cast<Expression*>(this->expression->copy());
    return new VariableDeclaration{this->name, dynamic_cast<Datatype*>(this->type->copy()), expr};
}

bool VariableDeclaration::equal(ASTNodeInterface* other) const noexcept
{
    auto other_decl = dynamic_cast<VariableDeclaration*>(other);
    auto res = other_decl != nullptr && this->name == other_decl->name &&
        this->type->equal(other_decl->type);

    if (res && this->expression == nullptr)
    {
        return other_decl->expression == nullptr;
    }

    return res && this->expression->equal(other_decl->expression);
}

std::pair<bool, Datatype*> VariableDeclaration::type_check() const noexcept
{
    if (this->expression != nullptr)
    {
        auto expr_type = this->expression->type_check();
        if (expr_type.second != nullptr)
        {
            expr_type.second->destroy();
            delete expr_type.second;
        }
        return std::make_pair(expr_type.first, nullptr);
    }

    return std::make_pair(true, nullptr);
}

bool VariableDeclaration::resolve_name(SymbolTable& symbol_table) noexcept
{
    this->symbol = Symbol::build(this->type, this->name);

    bool result = symbol_table.bind(this->name, this->symbol);

    if (result && this->expression != nullptr)
    {
        if (!this->expression->resolve_name(symbol_table))
        {
            return false;
        }
    }
    
    return result;
}

std::string VariableDeclaration::to_python(const std::string& ident) const noexcept
{
    std::string result = name + " : " + type->to_python(ident);

    if (expression != nullptr)
    {
        result += " = " + expression->to_python(ident);
    }

    return result;
}

FunctionDeclaration::FunctionDeclaration(std::string_view fct_name, Datatype* datatype, const Body& fct_body) noexcept
    : Declaration{fct_name, datatype}, body{fct_body} {}

void FunctionDeclaration::destroy() noexcept
{
    this->type->destroy();
    delete this->type;
    this->type = nullptr;
    destroy_body(this->body);
}

ASTNodeInterface* FunctionDeclaration::copy() const noexcept
{
    return new FunctionDeclaration{
        this->name, dynamic_cast<Datatype*>(this->type->copy()),
        copy_body(this->body)
    };
}

bool FunctionDeclaration::equal(ASTNodeInterface* other) const noexcept
{
    auto other_decl = dynamic_cast<FunctionDeclaration*>(other);
    return other_decl != nullptr &&
        this->name == other_decl->name &&
        this->type->equal(other_decl->type) &&
        equal_body(this->body, other_decl->body);
}

bool FunctionDeclaration::resolve_name(SymbolTable& symbol_table) noexcept
{
    this->symbol = Symbol::build(this->type, this->name);
    bool result = symbol_table.bind(this->name, this->symbol);
    bool body_result = true;
    if (result && !body.empty())
    {
        symbol_table.enter_scope();
        this->type->resolve_name(symbol_table);
        body_result = resolve_name_body(this->body, symbol_table);
        symbol_table.exit_scope();
    }

    return result && body_result;
}

std::pair<bool, Datatype*> FunctionDeclaration::type_check() const noexcept
{
    return body_type_check(this->body);
}

std::string FunctionDeclaration::to_python(const std::string& ident) const noexcept
{
    FunctionDatatype* fct_type = dynamic_cast<FunctionDatatype*>(type);
    std::string result = "def " + name + "(" + param_list_to_python(fct_type->get_parameters(), ident) + ") -> " + fct_type->get_return_type()->to_python(ident) + ":\n";
    result += body_to_python(body, ident + "    ");
    return result;
}