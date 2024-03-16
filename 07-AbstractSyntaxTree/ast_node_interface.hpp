#pragma once

#include <forward_list>
#include <string>
#include <string_view>

class Declaration;
class Expression;
class Statement;
class Datatype;
class Parameter;

using Body = std::forward_list<Statement*>;

using Param = std::pair<std::string, Datatype*>;

using ParamList = std::forward_list<Param>;

void destroy_body(Body& body) noexcept;

class ASTNodeInterface
{
public:
    virtual ~ASTNodeInterface() noexcept;

    virtual void destroy() noexcept = 0;
};