#pragma once

#include <ast_node_interface.hpp>

class Datatype : public ASTNodeInterface
{
public:
    template <typename Type>
    bool is() const noexcept
    {
        return dynamic_cast<const Type*>(this) != nullptr;
    }

    std::pair<bool, Datatype*> type_check() const noexcept override;
};

class BasicDatatype : public Datatype
{
public:
    void destroy() noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;
};

class VoidDatatype : public BasicDatatype
{
public:
    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;
};

class BooleanDatatype : public BasicDatatype
{
public:
    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;
};

class CharacterDatatype : public BasicDatatype
{
public:
    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;
};

class IntegerDatatype : public BasicDatatype
{
public:
    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;
};

class StringDatatype : public BasicDatatype
{
public:
    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    std::string to_python(const std::string& ident) const noexcept override;
};

class ArrayDatatype : public Datatype
{
public:
    ArrayDatatype(Datatype* _inner_datatype) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;

    Datatype* get_inner_datatype() const noexcept;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Datatype* inner_datatype;
};

class FunctionDatatype : public Datatype
{
public:
    FunctionDatatype(Datatype* ret_type, const ParamList& params) noexcept;

    void destroy() noexcept override;

    ASTNodeInterface* copy() const noexcept override;

    bool equal(ASTNodeInterface* other) const noexcept override;

    bool resolve_name(SymbolTable& symbol_table) noexcept override;
    
    Datatype* get_return_type() const noexcept;

    const ParamList& get_parameters() const noexcept;

    std::string to_python(const std::string& ident) const noexcept override;

private:
    Datatype* return_type;
    ParamList parameters;
};