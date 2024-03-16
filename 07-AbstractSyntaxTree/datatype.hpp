#pragma once

#include <ast_node_interface.hpp>

class Datatype : public ASTNodeInterface
{

};

class BasicDatatype : public Datatype
{
public:
    void destroy() noexcept override;
};

class VoidDatatype : public BasicDatatype
{

};

class BooleanDatatype : public BasicDatatype
{

};

class CharacterDatatype : public BasicDatatype
{

};

class IntegerDatatype : public BasicDatatype
{

};

class StringDatatype : public BasicDatatype
{

};

class ArrayDatatype : public BasicDatatype
{
public:
    ArrayDatatype(Datatype* _inner_datatype) noexcept;

    void destroy() noexcept override;

private:
    Datatype* inner_datatype;
};

class FunctionDatatype : public Datatype
{
public:
    FunctionDatatype(Datatype* ret_type, const ParamList& params) noexcept;

    void destroy() noexcept override;

private:
    Datatype* return_type;
    ParamList parameters;
};