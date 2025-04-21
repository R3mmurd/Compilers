#include <datatype.hpp>

std::pair<bool, Datatype*> Datatype::type_check() const noexcept
{
    return std::make_pair(true, nullptr);
}

void BasicDatatype::destroy() noexcept {}

bool BasicDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    return true;
}

ASTNodeInterface* VoidDatatype::copy() const noexcept 
{
    return new VoidDatatype{};
}

bool VoidDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<VoidDatatype*>(other) != nullptr;
}

std::string VoidDatatype::to_python(const std::string& ident) const noexcept
{
    return "None";
}

ASTNodeInterface* BooleanDatatype::copy() const noexcept 
{
    return new BooleanDatatype{};
}

bool BooleanDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<BooleanDatatype*>(other) != nullptr;
}

std::string BooleanDatatype::to_python(const std::string& ident) const noexcept
{
    return "bool";
}

ASTNodeInterface* CharacterDatatype::copy() const noexcept 
{
    return new CharacterDatatype{};
}

bool CharacterDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<CharacterDatatype*>(other) != nullptr;
}

std::string CharacterDatatype::to_python(const std::string& ident) const noexcept
{
    return "str";
}

ASTNodeInterface* IntegerDatatype::copy() const noexcept 
{
    return new IntegerDatatype{};
}

bool IntegerDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<IntegerDatatype*>(other) != nullptr;
}

std::string IntegerDatatype::to_python(const std::string& ident) const noexcept
{
    return "int";
}

ASTNodeInterface* StringDatatype::copy() const noexcept 
{
    return new StringDatatype{};
}

bool StringDatatype::equal(ASTNodeInterface* other) const noexcept
{
    return dynamic_cast<StringDatatype*>(other) != nullptr;
}

std::string StringDatatype::to_python(const std::string& ident) const noexcept
{
    return "str";
}

ArrayDatatype::ArrayDatatype(Datatype* _inner_datatype) noexcept
    : inner_datatype{_inner_datatype} {}

void ArrayDatatype::destroy() noexcept
{
    this->inner_datatype->destroy();
    delete this->inner_datatype;
    this->inner_datatype = nullptr;
}

ASTNodeInterface* ArrayDatatype::copy() const noexcept 
{
    return new ArrayDatatype{dynamic_cast<Datatype*>(this->inner_datatype->copy())};
}

bool ArrayDatatype::equal(ASTNodeInterface* other) const noexcept
{
    auto other_array = dynamic_cast<ArrayDatatype*>(other);
    return other != nullptr && this->inner_datatype->equal(other_array->inner_datatype);
}

bool ArrayDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->inner_datatype->resolve_name(symbol_table);
}

Datatype* ArrayDatatype::get_inner_datatype() const noexcept
{
    return this->inner_datatype;
}

std::string ArrayDatatype::to_python(const std::string& ident) const noexcept
{
    return "list[" + inner_datatype->to_python(ident) + "]";
}

FunctionDatatype::FunctionDatatype(Datatype* ret_type, const ParamList& params) noexcept
    : return_type{ret_type}, parameters{params} {}

void FunctionDatatype::destroy() noexcept
{
    this->return_type->destroy();
    delete this->return_type;
    this->return_type = nullptr;
    destroy_param_list(this->parameters);
}

ASTNodeInterface* FunctionDatatype::copy() const noexcept 
{
    return new FunctionDatatype{
        dynamic_cast<Datatype*>(this->return_type->copy()),
        copy_param_list(this->parameters)
    };
}

bool FunctionDatatype::equal(ASTNodeInterface* other) const noexcept
{
    auto other_function = dynamic_cast<FunctionDatatype*>(other);
    return other_function != nullptr && this->return_type->equal(other_function->return_type) &&
        equal_param_list(this->parameters, other_function->parameters);
}

bool FunctionDatatype::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool result = this->return_type->resolve_name(symbol_table);
    return result && resolve_name_param_list(this->parameters, symbol_table);
}

Datatype* FunctionDatatype::get_return_type() const noexcept
{
    return this->return_type;
}

const ParamList& FunctionDatatype::get_parameters() const noexcept
{
    return this->parameters;
}

std::string FunctionDatatype::to_python(const std::string& ident) const noexcept
{
    return "callable";
}