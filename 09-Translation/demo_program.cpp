/*
    Compilers: Semantic Analysis

    This program creates the AST for the following program


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

    int main()
    {
        int result = accum_from_zero_to(10);
        print result; // Hypotetical print statement
        return 0;
    }
*/

#include <iostream>
#include <fstream>

#include <datatype.hpp>
#include <declaration.hpp>
#include <expression.hpp>
#include <statement.hpp>
#include <symbol_table.hpp>

int main()
{
    auto function_declaration = new FunctionDeclaration{
        "accum_from_zero_to",                             // --> Function name
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

    auto main_function = new FunctionDeclaration{
        "main",                                           // --> Function name
        new FunctionDatatype{                             // --> Function type
            new IntegerDatatype{},
            ParamList{}
        },
        Body{                                             // --> Function body
            new DeclarationStatement{                     // --> int i;
                new VariableDeclaration{                  // int result = accum_from_zero_to(10);
                    "result",
                    new IntegerDatatype{},
                    new CallExpression{
                        new NameExpression{"accum_from_zero_to"},
                        new ArgExpression{new IntExpression{10}, nullptr}
                    }
                }
            },
            new PrintStatement{new NameExpression{"result"}}, // --> print result;
            new ReturnStatement{new IntExpression{0}}         // --> return 0;
        }
    };

    auto program = Body{
        new DeclarationStatement{function_declaration},
        new DeclarationStatement{main_function},
        new ExpressionStatement{
            new CallExpression{
                new NameExpression{"main"},
                nullptr
            }
        }
    };

    bool resolve_name_result = false;
    {
        SymbolTable symbol_table;
        resolve_name_result = resolve_name_body(program, symbol_table);
    }

    std::cout << std::boolalpha << "Program name resolution: " << resolve_name_result << std::endl;

    auto program_copy = copy_body(program);

    bool program_equal_result = equal_body(program, program_copy);

    std::cout << std::boolalpha << "Program equality: " << program_equal_result << std::endl;

    auto program_type = body_type_check(program);

    std::cout << std::boolalpha << "Program type check: " << program_type.first << std::endl;

    if (program_type.second != nullptr)
    {
        program_type.second->destroy();
        delete program_type.second;
        program_type.second = nullptr;
    }


    std::ofstream output{"demo_program.py"};
    output << body_to_python(program, "");
    output.close();

    destroy_body(program);
    destroy_body(program_copy);

    return EXIT_SUCCESS;
}