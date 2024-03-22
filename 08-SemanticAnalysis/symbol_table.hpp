#pragma once

#include <memory>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>

class Datatype;

struct Symbol
{
    Datatype* type;
    std::string name;
    int which;

    static std::shared_ptr<Symbol> build(Datatype* type, std::string_view name) noexcept;
};

class SymbolTable
{
public:
    using TableType = std::unordered_map<std::string, std::shared_ptr<Symbol>>;
    using TableStack = std::vector<TableType>;

    SymbolTable() = default;

    void enter_scope() noexcept;

    bool exit_scope() noexcept;

    TableType::size_type scope_level() const noexcept;

    bool bind(const std::string& name, std::shared_ptr<Symbol> symbol) noexcept;

    std::shared_ptr<Symbol> lookup(const std::string& name) noexcept;

    std::shared_ptr<Symbol> current_scope_lookup(const std::string& name) noexcept;

private:
    static std::shared_ptr<Symbol> find_in_scope(const std::string& name, const TableType& scope) noexcept;

    TableStack scopes;
};