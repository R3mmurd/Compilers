/*
    Compilers: Abstract Syntax Tree (AST)

    This program creates the AST for the following function in C

    int accum_from_zero_to(int x)
    {
        int i;
        int total = 0;

        for (i = 0; i < x; i++)
        {
            total = total + i;
        }

        return total;
    }

    The following function is another syntax of a hypothetical programming
    language. This function would be solved by the same AST

    function accum_from_zero(x: integer) : integer
    begin
        var i : integer;
        var total : integer = 0;

        for (i := 0 ... i < x ... inc i)
        begin
            total := total + i;
        end;

        return total;
    end;
*/

#include <iostream>

#include <datatype.hpp>
#include <declaration.hpp>
#include <expression.hpp>
#include <statement.hpp>
#include <symbol_table.hpp>

int main()
{
    auto function_declaration = new FunctionDeclaration{
        "accum_from_zero_to",                                        // --> Function name
        new FunctionDatatype{                             // --> Function type
            new IntegerDatatype{},
            ParamList{Param{"x", new IntegerDatatype{}}}
        },
        Body{                                             // --> Function body
            new DeclarationStatement{                     // --> int i;
                new VariableDeclaration{
                    "i",
                    new IntegerDatatype{}
                }
            },
            new DeclarationStatement{                     // --> int total = 0;
                new VariableDeclaration{
                    "total",
                    new IntegerDatatype{},
                    new IntExpression{0}
                }
            },
            new ForStatement{                             // --> for(...)
                new AssignmentExpression{                 // --> i = 0
                    new NameExpression{"i"},
                    new IntExpression{0}
                },
                new LessExpression{                       // --> i < 10
                    new NameExpression{"i"},
                    new NameExpression{"x"}
                },
                new IncrementExpression{                  // --> i++
                    new NameExpression{"i"}
                },
                Body{
                    new ExpressionStatement{
                        new AssignmentExpression{         // --> total = total + i;
                            new NameExpression{"total"},
                            new AddExpression{
                                new NameExpression{"total"},
                                new NameExpression{"i"}
                            }
                        }
                    },
                }
            },
            new ReturnStatement{                           // --> return total;
                new NameExpression{"total"}
            }
        }
    };

    SymbolTable symbol_table;
    symbol_table.enter_scope();

    bool result = function_declaration->resolve_name(symbol_table);

    symbol_table.exit_scope();

    std::cout << std::boolalpha << "Result of name resolution: " << result << std::endl;

    auto function_declaration_copy = function_declaration->copy();

    result = function_declaration->equal(function_declaration_copy);

    std::cout << std::boolalpha << "Function declaration equality: " << result << std::endl;

    std::cout << std::boolalpha << "Function declaration type check: " << function_declaration->type_check().first << std::endl;


    function_declaration->destroy();
    delete function_declaration;
    function_declaration_copy->destroy();
    delete function_declaration_copy;

    auto void_type = new VoidDatatype{};
    auto int_type1 = new IntegerDatatype{};
    auto int_type2 = new IntegerDatatype{};

    std::cout << std::boolalpha << "void == int: " << void_type->equal(int_type1) << std::endl;
    std::cout << std::boolalpha << "int == int: " << int_type1->equal(int_type2) << std::endl;

    delete void_type;
    delete int_type1;
    delete int_type2;

    return EXIT_SUCCESS;
}