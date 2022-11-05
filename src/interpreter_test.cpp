#include "interpreter_test.h"
#include <iostream>

#include "types.h"
#include "operations.h"

#include "ast_builder.h"
#include "parser.h"

namespace cx::interpreter {

using namespace cx;

// template <typename... Args>
// auto mk(Args&&... args) -> decltype(std::make_unique<cx::expression>(std::forward<Args>(args)...)) {
//     return std::make_unique<cx::expression>(std::forward<Args>(args)...);
//     return cx::expression(args);
// }
auto mk(expression&& e) { return e; }

void CheckEquals(expression&& l, expression&& r) {
    environment env;
    auto eq = Eval(
        application{
            mk(equals_to{mk(std::move(l))}),
            mk(std::move(r))
        },
        env
    );
    if (std::holds_alternative<nothing>(eq)) {
        std::cout << "Test failed: "     << std::endl << "    " 
                  << Show(std::move(l)) << std::endl << "    " 
                  << Show(std::move(r)) << std::endl;
    }
}

void CheckNotEquals(expression&& l, expression&& r) {
    environment env;
    auto eq = Eval(
        application{
            mk(equals_to{mk(std::move(l))}),
            mk(std::move(r))
        },
        env
    );
    if (!std::holds_alternative<nothing>(eq)) {
        std::cout << "Test failed: "     << std::endl << "    " 
                  << Show(std::move(l)) << std::endl << "    " 
                  << Show(std::move(r)) << std::endl;
    }
}

void test() {
    std::cout << Show(identifier{"Starting tests"}) << std::endl;
    CheckEquals(
        application{
            mk(application{
                mk(addition{}), 
                mk(2)
            }),
            mk(application{
                mk(addition_with{3}),
                mk(5)
            })
        },
        10
    );
    CheckNotEquals(
        application{
            mk(application{
                mk(addition{}), 
                mk(2)
            }),
            mk(nothing{})
        },
        10
    );
    CheckEquals(std::move(cx::parser::build(*cx::parser::parse_code("1 + 2"))), 
                3);
}

}

namespace cx {

void eval(const char* code) {
    auto parsed = cx::parser::parse_code(code);
    if (!parsed)
        throw std::runtime_error("couldn't parse code");
        
    auto expr = cx::parser::build(*parsed);

    auto expr2 = expr;
    std::cout << "Got expression:" << std::endl << 
                 Show(std::move(expr2)) << std::endl;

    environment env;
    auto result = Eval(std::move(expr), env);
    std::cout << "Evaluated:" << std::endl;

    std::cout << Show(std::move(result)) << std::endl;
}

void eval_file(const char* path) {
    auto parsed = cx::parser::parse_file(path);
    if (!parsed)
        throw std::runtime_error("couldn't parse file");

    auto expr = cx::parser::build(*parsed);

    auto expr2 = expr;
    std::cout << "Got expression:" << std::endl << 
                 Show(std::move(expr2)) << std::endl;

    environment env;
    auto result = Eval(std::move(expr), env);
    std::cout << "Evaluated:" << std::endl;

    std::cout << Show(std::move(result)) << std::endl;
}

}