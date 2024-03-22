#include <expression.hpp>
#include <symbol_table.hpp>

UnaryExpression::UnaryExpression(Expression* expr) noexcept
    : expression{expr} {}

void UnaryExpression::destroy() noexcept
{
    this->expression->destroy();
    delete this->expression;
}

bool UnaryExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    return this->expression->resolve_name(symbol_table);
}

ASTNodeInterface* NotExpression::copy() const noexcept
{
    return new NotExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

ASTNodeInterface* IncrementExpression::copy() const noexcept
{
    return new IncrementExpression{dynamic_cast<Expression*>(this->expression->copy())};
}

ASTNodeInterface* DecrementExpression::copy() const noexcept
{
    return new DecrementExpression{dynamic_cast<Expression*>(this->expression->copy())};
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

bool BinaryExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    bool left_result = this->left_expression->resolve_name(symbol_table);

    if (left_result && this->right_expression != nullptr)
    {
        return this->right_expression->resolve_name(symbol_table);
    }

    return left_result;
}

ASTNodeInterface* AndExpression::copy() const noexcept
{
    return new AndExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* OrExpression::copy() const noexcept
{
    return new OrExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* LessExpression::copy() const noexcept
{
    return new LessExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* LessEqExpression::copy() const noexcept
{
    return new LessEqExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* GreaterExpression::copy() const noexcept
{
    return new GreaterExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* GreaterEqExpression::copy() const noexcept
{
    return new GreaterEqExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* EqualExpression::copy() const noexcept
{
    return new EqualExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* NotEqualExpression::copy() const noexcept
{
    return new NotEqualExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* AddExpression::copy() const noexcept
{
    return new AddExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* SubExpression::copy() const noexcept
{
    return new SubExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* MulExpression::copy() const noexcept
{
    return new MulExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* DivExpression::copy() const noexcept
{
    return new DivExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* ModExpression::copy() const noexcept
{
    return new ModExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
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

ASTNodeInterface* SubscriptExpression::copy() const noexcept
{
    return new SubscriptExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
}

ASTNodeInterface* AssignmentExpression::copy() const noexcept
{
    return new AssignmentExpression{
        dynamic_cast<Expression*>(this->left_expression->copy()),
        dynamic_cast<Expression*>(this->right_expression->copy())
    };
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

bool NameExpression::resolve_name(SymbolTable& symbol_table) noexcept
{
    this->symbol = symbol_table.lookup(name);
    return this->symbol != nullptr;
}

IntExpression::IntExpression(int _value) noexcept
    : value{_value} {}

ASTNodeInterface* IntExpression::copy() const noexcept
{
    return new IntExpression{this->value};
}

StrExpression::StrExpression(std::string_view _value) noexcept
    : value{_value} {}

ASTNodeInterface* StrExpression::copy() const noexcept
{
    return new StrExpression{this->value};
}