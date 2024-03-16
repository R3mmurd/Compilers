#include <datatype.hpp>

void BasicDatatype::destroy() noexcept {}

ArrayDatatype::ArrayDatatype(Datatype* _inner_datatype) noexcept
    : inner_datatype{_inner_datatype} {}

void ArrayDatatype::destroy() noexcept
{
    this->inner_datatype->destroy();
    delete this->inner_datatype;
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