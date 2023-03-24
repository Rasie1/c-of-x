#include "interpreter.h"
#include <iostream>

#include "environment.h"
#include "operations.h"

#include "ast_builder.h"
#include "parser.h"

namespace cx {

void eval_and_print(const std::shared_ptr<tao::pegtl::parse_tree::node>& parsed, int verbosity) {
    if (!parsed)
        throw std::runtime_error("couldn't parse file");

    auto expr = cx::parser::build(*parsed);

    if (verbosity) {
        std::cout << "Got expression:" << std::endl << 
                    Show(copy(expr)) << std::endl << std::endl;
    }

    environment env;
    if (verbosity >= 2)
        env.isTraceEnabled = true;

    auto result = SubstituteVariables(copy(expr), env);

    if (verbosity) {
        std::cout << "Environment:" << std::endl;
        for (auto [k, v]: env.variables) {
            std::cout << k << ": " << Show(std::move(v)) << std::endl;
        }

        std::cout << std::endl;
        std::cout << "Compiled:" << std::endl;
        std::cout << Show(copy(result)) << std::endl;
        std::cout << std::endl;
    }

    env.isExecuting = true;

    result = SubstituteVariables(std::move(result), env);

    if (verbosity)
        std::cout << "Executed:" << std::endl;

    std::cout << Show(std::move(result)) << std::endl;
}

void eval(const char* code, int verbosity) {
    auto parsed = cx::parser::parse_code(code);
    eval_and_print(parsed, verbosity);
}

void eval_file(const char* path, int verbosity) {
    auto parsed = cx::parser::parse_file(path);
    eval_and_print(parsed, verbosity);
}

}