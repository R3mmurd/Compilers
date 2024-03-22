#include <ast_node_interface.hpp>

#include <datatype.hpp>
#include <statement.hpp>
#include <symbol_table.hpp>

void destroy_body(Body& body) noexcept
{
    while (!body.empty())
    {
        Statement* statement = body.front();
        body.pop_front();
        statement->destroy();
        delete statement;
    }
}

Body copy_body(const Body& body) noexcept
{
    Body result;

    for (auto statement: body)
    {
        result.push_back(dynamic_cast<Statement*>(statement->copy()));
    }

    return result;
}

bool equal_body(const Body& body1, const Body& body2) noexcept
{
    if (body1.size() != body2.size())
    {
        return false;
    }

    return std::equal(body1.begin(), body1.end(), body2.begin());
}

bool resolve_name_body(Body& body, SymbolTable& symbol_table) noexcept
{
    for (Statement* statement : body)
    {
        if (!statement->resolve_name(symbol_table))
        {
            return false;
        }
    }

    return true;
}

void destroy_param_list(ParamList& param_list) noexcept
{
    while (!param_list.empty())
    {
        Param param = param_list.front();
        param_list.pop_front();
        param.second->destroy();
        delete param.second;
    }
}

ParamList copy_param_list(const ParamList& param_list) noexcept
{
    ParamList result;

    for (auto param: param_list)
    {
        result.push_back(std::make_pair(param.first, dynamic_cast<Datatype*>(param.second->copy())));
    }

    return result;
}

bool equal_param_list(const ParamList& param_list1, const ParamList& param_list2) noexcept
{
    if (param_list1.size() != param_list2.size())
    {
        return false;
    }

    return std::equal(param_list1.begin(), param_list1.end(), param_list2.begin(), param_list2.end(),
        [] (const Param& param1, const Param& param2) { return param1.first == param2.first && param1.second->equal(param2.second); });
}

bool resolve_name_param_list(const ParamList& param_list, SymbolTable& symbol_table) noexcept
{
    for (const Param& param : param_list)
    {
        auto symbol = Symbol::build(param.second, param.first);

        if (!symbol_table.bind(param.first, symbol))
        {
            return false;
        }
    }

    return true;
}

ASTNodeInterface::~ASTNodeInterface() noexcept {}