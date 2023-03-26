#include "interpreter_test.h"
#include <iostream>

#include "environment.h"
#include "operations.h"

#include "ast_builder.h"
#include "parser.h"

namespace cx::interpreter_test {

using namespace cx;

auto mk(expression&& e) { return e; }

void CheckEquals(expression&& l, expression&& r) {
    environment env;
    auto eq = Eval(
        application{
            mk(equals_to{mk(move(l))}),
            mk(move(r))
        },
        env
    );
    if (std::holds_alternative<nothing>(eq)) {
        std::cout << "Test failed: "     << std::endl << "    " 
                  << Show(move(l)) << std::endl << "    " 
                  << Show(move(r)) << std::endl;
    }
}

void CheckNotEquals(expression&& l, expression&& r) {
    environment env;
    auto eq = Eval(
        application{
            mk(equals_to{mk(move(l))}),
            mk(move(r))
        },
        env
    );
    if (!std::holds_alternative<nothing>(eq)) {
        std::cout << "Test failed: "     << std::endl << "    " 
                  << Show(move(l)) << std::endl << "    " 
                  << Show(move(r)) << std::endl;
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
    CheckEquals(move(cx::parser::build(*cx::parser::parse_code("1 + 2"))), 
                3);
}

}