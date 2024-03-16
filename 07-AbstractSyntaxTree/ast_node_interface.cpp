#include <ast_node_interface.hpp>

#include <statement.hpp>

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

ASTNodeInterface::~ASTNodeInterface() noexcept {}