#pragma once

#include <ast_node_interface.hpp>

class Datatype : public ASTNodeInterface
{

};

class BasicDatatype : public Datatype
{
public:
    void destroy() noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;
};

class VoidDatatype : public BasicDatatype
{
    ASTNodeInterface* copy() const noexcept override;
};

class BooleanDatatype : public BasicDatatype
{
    ASTNodeInterface* copy() const noexcept override;
};

class CharacterDatatype : public BasicDatatype
{
    ASTNodeInterface* copy() const noexcept override;
};

class IntegerDatatype : public BasicDatatype
{
    ASTNodeInterface* copy() const noexcept override;
};

class StringDatatype : public BasicDatatype
{
    ASTNodeInterface* copy() const noexcept override;
};

class ArrayDatatype : public Datatype
{
public:
    ArrayDatatype(Datatype* _inner_datatype) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

private:
    Datatype* inner_datatype;
};

class FunctionDatatype : public Datatype
{
public:
    FunctionDatatype(Datatype* ret_type, const ParamList& params) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

private:
    Datatype* return_type;
    ParamList parameters;
};