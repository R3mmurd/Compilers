#include <datatype.hpp>

void BasicDatatype::destroy() noexcept {}

bool BasicDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    return true;
}

ASTNodeInterface* VoidDatatype::copy() const noexcept 
{
    return new VoidDatatype{};
}

ASTNodeInterface* BooleanDatatype::copy() const noexcept 
{
    return new BooleanDatatype{};
}

ASTNodeInterface* CharacterDatatype::copy() const noexcept 
{
    return new CharacterDatatype{};
}

ASTNodeInterface* IntegerDatatype::copy() const noexcept 
{
    return new IntegerDatatype{};
}

ASTNodeInterface* StringDatatype::copy() const noexcept 
{
    return new StringDatatype{};
}

ArrayDatatype::ArrayDatatype(Datatype* _inner_datatype) noexcept
    : inner_datatype{_inner_datatype} {}

void ArrayDatatype::destroy() noexcept
{
    this->inner_datatype->destroy();
    delete this->inner_datatype;
}

ASTNodeInterface* ArrayDatatype::copy() const noexcept 
{
    return new ArrayDatatype{dynamic_cast<Datatype*>(this->inner_datatype->copy())};
}

bool ArrayDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->inner_datatype->resolve_name(symbol_table);
}

FunctionDatatype::FunctionDatatype(Datatype* ret_type, const ParamList& params) noexcept
    : return_type{ret_type}, parameters{params} {}

void FunctionDatatype::destroy() noexcept
{
    this->return_type->destroy();
    delete this->return_type;

    while (!this->parameters.empty())
    {
        Param param = this->parameters.front();
        this->parameters.pop_front();
        param.second->destroy();
        delete param.second;
    }
}

ASTNodeInterface* FunctionDatatype::copy() const noexcept 
{
    return new FunctionDatatype{
        dynamic_cast<Datatype*>(this->return_type->copy()),
        copy_param_list(this->parameters)
    };
}

bool FunctionDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool result = this->return_type->resolve_name(symbol_table);
    return result && resolve_name_param_list(this->parameters, symbol_table);
}