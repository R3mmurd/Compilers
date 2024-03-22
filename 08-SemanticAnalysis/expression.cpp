#include <datatype.hpp>
#include <expression.hpp>
#include <symbol_table.hpp>

UnaryExpression::UnaryExpression(Expression* expr) noexcept
    : expression{expr} {}

void UnaryExpression::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
}

bool UnaryExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<UnaryExpression*>(other);
    return other_expr != nullptr && this->expression->equal(other_expr->expression);
}

bool UnaryExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->expression->resolve_name(symbol_table);
}

ASTNodeInterface* NotExpression::copy() const noexcept
{
    return new NotExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool NotExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<NotExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> NotExpression::type_check() const noexcept
{
    auto bool_type = new BooleanDatatype{};
    auto expr_type = this->expression->type_check();
    bool result = expr_type.first && expr_type.second->equal(bool_type);
    delete expr_type.second;

    if (!result)
    {
        delete bool_type;
        bool_type = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, bool_type);
}

ASTNodeInterface* IncrementExpression::copy() const noexcept
{
    return new IncrementExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool IncrementExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<IncrementExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> IncrementExpression::type_check() const noexcept
{
    auto int_type = new IntegerDatatype{};
    auto expr_type = this->expression->type_check();
    bool result = expr_type.second->equal(int_type);
    delete expr_type.second;
    
    if (!result)
    {
        delete int_type;
        int_type = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, int_type);
}

ASTNodeInterface* DecrementExpression::copy() const noexcept
{
    return new DecrementExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

bool DecrementExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<DecrementExpression*>(other);
    return other_expr != nullptr && UnaryExpression::equal(other);
}

std::pair<bool, Datatype*> DecrementExpression::type_check() const noexcept
{
    auto int_type = new IntegerDatatype{};
    auto expr_type = this->expression->type_check();
    bool result = expr_type.second->equal(int_type);
    delete expr_type.second;
    
    if (!result)
    {
        delete int_type;
        int_type = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, int_type);
}

BinaryExpression::BinaryExpression(Expression* left_expr, Expression* right_expr) noexcept
    : left_expression{left_expr}, right_expression{right_expr} {}

void BinaryExpression::destroy() noexcept
{
    this->left_expression->destroy();
    delete this->left_expression;

    if (this->right_expression != nullptr)
    {
        this->right_expression->destroy();
        delete this->right_expression;
    }
}

bool BinaryExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<BinaryExpression*>(other);
    auto res = other_expr!= nullptr && this->left_expression->equal(other_expr->left_expression);

    if (res && this->right_expression == nullptr)
    {
        return other_expr->right_expression == nullptr;
    }

    return res && this->right_expression->equal(other_expr->right_expression);
}

bool BinaryExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool left_result = this->left_expression->resolve_name(symbol_table);

    if (left_result && this->right_expression != nullptr)
    {
        return this->right_expression->resolve_name(symbol_table);
    }

    return left_result;
}

Expression* BinaryExpression::get_left_expression() const noexcept
{
    return this->left_expression;
}

Expression* BinaryExpression::get_right_expression() const noexcept
{
    return this->right_expression;
}

std::pair<bool, Datatype*> BinaryExpression::boolean_operation_type_check() const noexcept
{
    auto bool_type = new BooleanDatatype{};
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();
    bool result = left_expr_type.first &&
        right_expr_type.first &&
        left_expr_type.second->equal(bool_type) &&
        right_expr_type.second->equal(bool_type);

    delete left_expr_type.second;
    delete right_expr_type.second;
    
    if (!result)
    {
        delete bool_type;
        bool_type = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, bool_type);
}

std::pair<bool, Datatype*> BinaryExpression::comparison_type_check() const noexcept
{
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();

    if (!left_expr_type.first || !right_expr_type.first)
    {
        return std::make_pair(false, nullptr);
    }

    if (left_expr_type.second->is<VoidDatatype>() || left_expr_type.second->is<ArrayDatatype>() ||
        left_expr_type.second->is<FunctionDatatype>() || right_expr_type.second->is<VoidDatatype>() ||
        right_expr_type.second->is<ArrayDatatype>() || right_expr_type.second->is<FunctionDatatype>())
    {
        delete left_expr_type.second;
        delete right_expr_type.second;
        return std::make_pair(false, nullptr);    
    }

    bool result = left_expr_type.second->equal(right_expr_type.second);
    delete left_expr_type.second;
    delete right_expr_type.second;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new BooleanDatatype{});
}

std::pair<bool, Datatype*> BinaryExpression::arithmetic_operation_type_check() const noexcept
{
    auto int_type = new IntegerDatatype{};
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();
    bool result = left_expr_type.first &&
        right_expr_type.first &&
        left_expr_type.second->equal(int_type) &&
        right_expr_type.second->equal(int_type);
    delete left_expr_type.second;
    delete right_expr_type.second;
    
    if (!result)
    {
        delete int_type;
        int_type = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, int_type);
}

ASTNodeInterface* AndExpression::copy() const noexcept
{
    return new AndExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool AndExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<AndExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> AndExpression::type_check() const noexcept
{
    return this->boolean_operation_type_check();
}

ASTNodeInterface* OrExpression::copy() const noexcept
{
    return new OrExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool OrExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<OrExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> OrExpression::type_check() const noexcept
{
    return this->boolean_operation_type_check();
}

ASTNodeInterface* LessExpression::copy() const noexcept
{
    return new LessExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool LessExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<LessExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> LessExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* LessEqExpression::copy() const noexcept
{
    return new LessEqExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool LessEqExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<LessEqExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> LessEqExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* GreaterExpression::copy() const noexcept
{
    return new GreaterExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool GreaterExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<GreaterExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> GreaterExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* GreaterEqExpression::copy() const noexcept
{
    return new GreaterEqExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool GreaterEqExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<GreaterEqExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> GreaterEqExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* EqualExpression::copy() const noexcept
{
    return new EqualExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool EqualExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<EqualExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> EqualExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* NotEqualExpression::copy() const noexcept
{
    return new NotEqualExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool NotEqualExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<NotEqualExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> NotEqualExpression::type_check() const noexcept
{
    return this->comparison_type_check();    
}

ASTNodeInterface* AddExpression::copy() const noexcept
{
    return new AddExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool AddExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<AddExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> AddExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* SubExpression::copy() const noexcept
{
    return new SubExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool SubExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<SubExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> SubExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* MulExpression::copy() const noexcept
{
    return new MulExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool MulExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<MulExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> MulExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* DivExpression::copy() const noexcept
{
    return new DivExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

std::pair<bool, Datatype*> DivExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

bool DivExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<DivExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

ASTNodeInterface* ModExpression::copy() const noexcept
{
    return new ModExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool ModExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ModExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> ModExpression::type_check() const noexcept
{
    return this->arithmetic_operation_type_check();    
}

ASTNodeInterface* ArgExpression::copy() const noexcept
{
    Expression* left_copy = dynamic_cast<Expression*>(this->left_expression->copy());
    Expression* right_copy = nullptr;

    if (this->right_expression != nullptr)
    {
        right_copy = dynamic_cast<Expression*>(this->right_expression->copy());
    }

    return new ArgExpression{left_copy, right_copy};
}

bool ArgExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<ArgExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> ArgExpression::type_check() const noexcept
{
    return this->left_expression->type_check();
}

ASTNodeInterface* CallExpression::copy() const noexcept
{
    Expression* left_copy = dynamic_cast<Expression*>(this->left_expression->copy());
    Expression* right_copy = nullptr;

    if (this->right_expression != nullptr)
    {
        right_copy = dynamic_cast<Expression*>(this->right_expression->copy());
    }

    return new CallExpression{left_copy, right_copy};
}

bool CallExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<CallExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> CallExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();

    if (!left_type.first || !left_type.second->is<FunctionDatatype>())
    {
        delete left_type.second;
        return std::make_pair(false, nullptr);
    }

    auto fct_type = dynamic_cast<FunctionDatatype*>(left_type.second);

    auto arg_expr = dynamic_cast<ArgExpression*>(this->right_expression);

    for (const auto& param: fct_type->get_parameters())
    {
        if (arg_expr == nullptr)
        {
            delete left_type.second;
            return std::make_pair(false, nullptr);
        }
    
        auto arg_type = arg_expr->get_left_expression()->type_check();

        if (!arg_type.second->equal(param.second))
        {
            delete arg_type.second;
            delete left_type.second;
            return std::make_pair(false, nullptr);
        }

        delete arg_type.second;

        arg_expr = dynamic_cast<ArgExpression*>(arg_expr->get_right_expression());
    }

    auto result = dynamic_cast<Datatype*>(fct_type->get_return_type()->copy());

    delete fct_type;

    if (arg_expr != nullptr)
    {
        delete result;
        result = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, result);    
}

ASTNodeInterface* SubscriptExpression::copy() const noexcept
{
    return new SubscriptExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool SubscriptExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<SubscriptExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other);
}

std::pair<bool, Datatype*> SubscriptExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();

    if (!left_type.second->is<ArrayDatatype>() || !right_type.second->is<IntegerDatatype>())
    {
        delete left_type.second;
        delete right_type.second;
        return std::make_pair(false, nullptr);
    }

    auto result_type = dynamic_cast<Datatype*>(dynamic_cast<ArrayDatatype*>(left_type.second)->get_inner_datatype()->copy());

    delete left_type.second;
    delete right_type.second;

    return std::make_pair(true, result_type);
}

ASTNodeInterface* AssignmentExpression::copy() const noexcept
{
    return new AssignmentExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

bool AssignmentExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<AssignmentExpression*>(other);
    return other_expr != nullptr && BinaryExpression::equal(other_expr);
}

std::pair<bool, Datatype*> AssignmentExpression::type_check() const noexcept
{
    auto left_type = this->left_expression->type_check();
    auto right_type = this->right_expression->type_check();
    bool result = left_type.first &&
        right_type.first &&
        left_type.second->equal(right_type.second);

    delete right_type.second;

    if (!result)
    {
        delete left_type.second;
        left_type.second = nullptr;
        left_type.first = false;
    }

    return left_type;
}

void LeafExpression::destroy() noexcept {}

bool LeafExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    return true;
}

NameExpression::NameExpression(std::string_view _name) noexcept
    : name{_name} {}

ASTNodeInterface* NameExpression::copy() const noexcept
{
    return new NameExpression{this->name};
}

bool NameExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<NameExpression*>(other);
    return other_expr != nullptr && this->name == other_expr->name;
}

bool NameExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    this->symbol = symbol_table.lookup(this->name);
    return this->symbol != nullptr;
}

std::pair<bool, Datatype*> NameExpression::type_check() const noexcept
{
    if (symbol == nullptr)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, dynamic_cast<Datatype*>(this->symbol->type->copy()));    
}

IntExpression::IntExpression(int _value) noexcept
    : value{_value} {}

ASTNodeInterface* IntExpression::copy() const noexcept
{
    return new IntExpression{this->value};
}

bool IntExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<IntExpression*>(other);
    return other_expr != nullptr && this->value == other_expr->value;
}

std::pair<bool, Datatype*> IntExpression::type_check() const noexcept
{
    return std::make_pair(true, new IntegerDatatype{});
}

StrExpression::StrExpression(std::string_view _value) noexcept
    : value{_value} {}

ASTNodeInterface* StrExpression::copy() const noexcept
{
    return new StrExpression{this->value};
}

bool StrExpression::equal(ASTNodeInterface* other) const noexcept
{
    auto other_expr = dynamic_cast<StrExpression*>(other);
    return other_expr != nullptr && this->value == other_expr->value;
}

std::pair<bool, Datatype*> StrExpression::type_check() const noexcept
{
    return std::make_pair(true, new StringDatatype{});    
}