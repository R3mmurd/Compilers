#pragma once

#include <memory>
#include <string>
#include <string_view>

#include <ast_node_interface.hpp>

class Declaration : public ASTNodeInterface
{
public:
    Declaration(std::string_view decl_name, Datatype* datatype) noexcept;

protected:
    std::string name;
    Datatype* type;
    std::shared_ptr<Symbol> symbol{nullptr};
};

class VariableDeclaration : public Declaration
{
public:
    VariableDeclaration(std::string_view var_name, Datatype* datatype, Expression* expr = nullptr) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;
    
    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Expression* expression;
};

class FunctionDeclaration : public Declaration
{
public:
    FunctionDeclaration(std::string_view fct_name, Datatype* datatype, const Body& fct_body = Body{}) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::pair<bool, Datatype*> type_check() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Body body;
};