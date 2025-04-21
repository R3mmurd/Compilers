#include <datatype.hpp>
#include <expression.hpp>
#include <symbol_table.hpp>

UnaryExpression::UnaryExpression(Expression* expr) noexcept
    : expression{expr} {}

void UnaryExpression::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
    this->expression = nullptr;
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

Expression* UnaryExpression::get_expression() const noexcept
{
    return this->expression;
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
    auto expr_type = this->expression->type_check();
    bool result = expr_type.first && expr_type.second->is<BooleanDatatype>();
    expr_type.second->destroy();
    delete expr_type.second;
    expr_type.second = nullptr;

    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new BooleanDatatype{});
}

std::string NotExpression::to_python(const std::string& ident) const noexcept 
{
    return "not " + expression->to_python(ident);
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
    auto expr_type = this->expression->type_check();
    bool result = expr_type.second->is<IntegerDatatype>();
    expr_type.second->destroy();
    delete expr_type.second;
    expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new IntegerDatatype{});
}

std::string IncrementExpression::to_python(const std::string& ident) const noexcept 
{
    return expression->to_python(ident) + " += 1";
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
    auto expr_type = this->expression->type_check();
    bool result = expr_type.second->is<IntegerDatatype>();
    expr_type.second->destroy();
    delete expr_type.second;
    expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new IntegerDatatype{});
}

std::string DecrementExpression::to_python(const std::string& ident) const noexcept 
{
    return expression->to_python(ident) + " -= 1";
}

BinaryExpression::BinaryExpression(Expression* left_expr, Expression* right_expr) noexcept
    : left_expression{left_expr}, right_expression{right_expr} {}

void BinaryExpression::destroy() noexcept
{
    this->left_expression->destroy();
    delete this->left_expression;
    this->left_expression = nullptr;

    if (this->right_expression != nullptr)
    {
        this->right_expression->destroy();
        delete this->right_expression;
        this->right_expression = nullptr;
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

std::pair<bool, Datatype*> BinaryExpression::boolean_operation_type_check() const noexcept
{
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();
    bool result = left_expr_type.first &&
        right_expr_type.first &&
        left_expr_type.second->is<BooleanDatatype>() &&
        right_expr_type.second->is<BooleanDatatype>();

    left_expr_type.second->destroy();
    delete left_expr_type.second;
    left_expr_type.second = nullptr;

    right_expr_type.second->destroy();
    delete right_expr_type.second;
    right_expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new BooleanDatatype{});
}

std::pair<bool, Datatype*> BinaryExpression::comparison_type_check() const noexcept
{
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();

    if (!left_expr_type.first || !right_expr_type.first)
    {
        if (left_expr_type.second != nullptr)
        {
            left_expr_type.second->destroy();
            delete left_expr_type.second;
            left_expr_type.second = nullptr;
        }

        if (right_expr_type.second != nullptr)
        {
            right_expr_type.second->destroy();
            delete right_expr_type.second;
            right_expr_type.second = nullptr;
        }

        return std::make_pair(false, nullptr);
    }

    if (left_expr_type.second->is<VoidDatatype>() || left_expr_type.second->is<ArrayDatatype>() ||
        left_expr_type.second->is<FunctionDatatype>() || right_expr_type.second->is<VoidDatatype>() ||
        right_expr_type.second->is<ArrayDatatype>() || right_expr_type.second->is<FunctionDatatype>())
    {
        left_expr_type.second->destroy();
        delete left_expr_type.second;
        left_expr_type.second = nullptr;

        right_expr_type.second->destroy();
        delete right_expr_type.second;
        right_expr_type.second = nullptr;

        return std::make_pair(false, nullptr);    
    }

    bool result = left_expr_type.second->equal(right_expr_type.second);
    
    left_expr_type.second->destroy();
    delete left_expr_type.second;
    left_expr_type.second = nullptr;

    right_expr_type.second->destroy();
    delete right_expr_type.second;
    right_expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new BooleanDatatype{});
}

std::pair<bool, Datatype*> BinaryExpression::arithmetic_operation_type_check() const noexcept
{
    auto left_expr_type = this->left_expression->type_check();
    auto right_expr_type = this->right_expression->type_check();
    bool result = left_expr_type.first &&
        right_expr_type.first &&
        left_expr_type.second->is<IntegerDatatype>() &&
        right_expr_type.second->is<IntegerDatatype>();

    left_expr_type.second->destroy();
    delete left_expr_type.second;
    left_expr_type.second = nullptr;

    right_expr_type.second->destroy();
    delete right_expr_type.second;
    right_expr_type.second = nullptr;
    
    if (!result)
    {
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, new IntegerDatatype{});
}

Expression* BinaryExpression::get_left_expression() const noexcept
{
    return this->left_expression;
}

Expression* BinaryExpression::get_right_expression() const noexcept
{
    return this->right_expression;
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

std::string AndExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " and " + right_expression->to_python(ident);
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

std::string OrExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " or " + right_expression->to_python(ident);
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

std::string LessExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " < " + right_expression->to_python(ident);
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

std::string LessEqExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " <= " + right_expression->to_python(ident);
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

std::string GreaterExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " > " + right_expression->to_python(ident);
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

std::string GreaterEqExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " >= " + right_expression->to_python(ident);
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

std::string EqualExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " == " + right_expression->to_python(ident);
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

std::string NotEqualExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " != " + right_expression->to_python(ident);
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

std::string AddExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " + " + right_expression->to_python(ident);
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

std::string SubExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " - " + right_expression->to_python(ident);
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

std::string MulExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " * " + right_expression->to_python(ident);
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

std::string DivExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " // " + right_expression->to_python(ident);
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

std::string ModExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " % " + right_expression->to_python(ident);
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

std::string ArgExpression::to_python(const std::string& ident) const noexcept 
{
    std::string result = left_expression->to_python(ident);

    Expression* right = right_expression;

    while (right != nullptr)
    {
        ArgExpression* arg = dynamic_cast<ArgExpression*>(right);
        result += ", " + arg->get_left_expression()->to_python(ident);
        right = arg->get_right_expression();
    }

    return result;
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
        if (left_type.second != nullptr)
        {
            left_type.second->destroy();
            delete left_type.second;
            left_type.second = nullptr;
        }
        return std::make_pair(false, nullptr);
    }

    auto fct_type = dynamic_cast<FunctionDatatype*>(left_type.second);
    auto arg_expr = dynamic_cast<ArgExpression*>(this->right_expression);

    for (const auto& param: fct_type->get_parameters())
    {
        if (arg_expr == nullptr)
        {
            left_type.second->destroy();
            delete left_type.second;
            left_type.second = nullptr;
            return std::make_pair(false, nullptr);
        }
    
        auto arg_type = arg_expr->get_left_expression()->type_check();

        if (!arg_type.second->equal(param.second))
        {
            arg_type.second->destroy();
            delete arg_type.second;
            arg_type.second = nullptr;

            left_type.second->destroy();
            delete left_type.second;
            left_type.second = nullptr;

            return std::make_pair(false, nullptr);
        }

        arg_type.second->destroy();
        delete arg_type.second;
        arg_type.second = nullptr;

        arg_expr = dynamic_cast<ArgExpression*>(arg_expr->get_right_expression());
    }

    auto result = dynamic_cast<Datatype*>(fct_type->get_return_type()->copy());

    left_type.second->destroy();
    delete left_type.second;
    left_type.second = nullptr;

    if (arg_expr != nullptr)
    {
        result->destroy();
        delete result;
        result = nullptr;
        return std::make_pair(false, nullptr);
    }

    return std::make_pair(true, result);    
}

std::string CallExpression::to_python(const std::string& ident) const noexcept 
{
    std::string result = left_expression->to_python(ident) + "(";

    if (right_expression != nullptr)
    {
        result += right_expression->to_python(ident);
    }

    result += ")";

    return result;
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
        left_type.second->destroy();
        delete left_type.second;
        left_type.second = nullptr;
        
        right_type.second->destroy();
        delete right_type.second;
        right_type.second = nullptr;

        delete right_type.second;
        return std::make_pair(false, nullptr);
    }

    auto result_type = dynamic_cast<Datatype*>(dynamic_cast<ArrayDatatype*>(left_type.second)->get_inner_datatype()->copy());

    left_type.second->destroy();
    delete left_type.second;
    left_type.second = nullptr;
    
    right_type.second->destroy();
    delete right_type.second;
    right_type.second = nullptr;

    return std::make_pair(true, result_type);
}

std::string SubscriptExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + "[" + right_expression->to_python(ident) + "]";
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

    right_type.second->destroy();
    delete right_type.second;
    right_type.second = nullptr;

    if (!result)
    {
        left_type.second->destroy();
        delete left_type.second;
        left_type.second = nullptr;
        left_type.first = false;
    }

    return left_type;
}

std::string AssignmentExpression::to_python(const std::string& ident) const noexcept 
{
    return left_expression->to_python(ident) + " = " + right_expression->to_python(ident);
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

std::string NameExpression::to_python(const std::string& ident) const noexcept 
{
    return name;
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

std::string IntExpression::to_python(const std::string& ident) const noexcept 
{
    return std::to_string(value);
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

std::string StrExpression::to_python(const std::string& ident) const noexcept 
{
    return "\"" + value + "\"";
}