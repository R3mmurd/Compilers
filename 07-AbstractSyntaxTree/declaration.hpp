#pragma once

#include <string>
#include <string_view>

#include <ast_node_interface.hpp>

class Declaration : public ASTNodeInterface
{
public:
    Declaration(std::string_view decl_name, Datatype* datatype) noexcept;

private:
    std::string name;
    Datatype* type;
};

class VariableDeclaration : public Declaration
{
public:
    VariableDeclaration(std::string_view var_name, Datatype* datatype, Expression* expr = nullptr) noexcept;

    void destroy() noexcept override;

private:
    Expression* expression;
};

class FunctionDeclaration : public Declaration
{
public:
    FunctionDeclaration(std::string_view fct_name, Datatype* datatype, const Body& fct_body = Body{}) noexcept;

    void destroy() noexcept override;

private:
    Body body;
};