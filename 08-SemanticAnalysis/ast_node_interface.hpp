#pragma once

#include <list>
#include <string>
#include <string_view>

class Declaration;
class Expression;
class Statement;
class Datatype;
class Parameter;
struct Symbol;
class SymbolTable;

using Body = std::list<Statement*>;

using Param = std::pair<std::string, Datatype*>;

using ParamList = std::list<Param>;

void destroy_body(Body& body) noexcept;

Body copy_body(const Body& body) noexcept;

bool equal_body(const Body& body1, const Body& body2) noexcept;

std::pair<bool, Datatype*> body_type_check(const Body& body) noexcept;

bool resolve_name_body(Body& body, SymbolTable& symbol_table) noexcept;

void destroy_param_list(ParamList& param_list) noexcept;

ParamList copy_param_list(const ParamList& param_list) noexcept;

bool equal_param_list(const ParamList& param_list1, const ParamList& param_list2) noexcept;

std::pair<bool, Datatype*> param_list_type_check(const ParamList& param_list) noexcept;

bool resolve_name_param_list(const ParamList& param_list, SymbolTable& symbol_table) noexcept;

class ASTNodeInterface
{
public:
    virtual ~ASTNodeInterface() noexcept;

    virtual void destroy() noexcept = 0;

    virtual ASTNodeInterface* copy() const noexcept = 0;

    virtual bool equal(ASTNodeInterface* other) const noexcept = 0;

    virtual std::pair<bool, Datatype*> type_check() const noexcept = 0;

    virtual bool resolve_name(SymbolTable& symbol_table) noexcept = 0;
};