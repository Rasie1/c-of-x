#include "compiler.h"
#include <iostream>

namespace cx {

struct runtime_expression {
    void* data;
    int type;
    std::vector<std::unique_ptr<runtime_expression>> refinements;
    std::vector<std::unique_ptr<runtime_expression>> containedValues;
};

inline expression Normalize(expression&& e) {
    return e;
}

std::string Compile(expression&& e) {
    // auto normalized = Normalize(std::move(e));
    
    // DebugPrint("fix - evaluating", expr, env);
    // env.increaseDebugIndentation();
    // auto evaluated = Eval(std::move(expr), env);
    // env.decreaseDebugIndentation();
    // DebugPrint("fixing", evaluated, env);
    // env.increaseDebugIndentation();
    // auto fixed = std::visit(overload{
    //     [&env, &seen](identifier&& e) -> expression {
    //         DebugPrint("fixing in id", e, env);
    //         if (seen.contains(e.name))
    //             return any{};
    //         seen.insert(e.name);
    //         if (auto expr = env.get(e.name)) {
    //             auto copy = *expr;
    //             DebugPrint("fixing in id, got from env", *expr, env);
    //             return GetElement(SubstituteVariables(std::move(copy), env, seen));
    //         }
    //         return error{std::string("undefined variable \"") + e.name + "\""};
    //     },
    //     [&env, &seen](rec<equals_to>&& function) -> expression {
    //         function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
    //         return function;
    //     },
    //     [&env, &seen](rec<negated>&& function) -> expression {
    //         function.get().f = SubstituteVariables(std::move(function.get().f), env, seen);
    //         return function;
    //     },
    //     [&env, &seen](rec<intersection_with>&& function) -> expression {
    //         function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
    //         return function;
    //     },
    //     [&env, &seen](rec<application>&& e) -> expression {
    //         e.get().function = SubstituteVariables(std::move(e.get().function), env, seen);
    //         e.get().argument = SubstituteVariables(std::move(e.get().argument), env, seen);
    //         return e;
    //     },
    //     [&env, &seen](rec<then>&& e) -> expression {
    //         e.get().from = SubstituteVariables(std::move(e.get().from), env, seen);
    //         e.get().to = SubstituteVariables(std::move(e.get().to), env, seen);
    //         return e;
    //     },
    //     [](auto&& e) -> expression { return e; }
    // }, std::move(evaluated));
    // env.decreaseDebugIndentation();
    // DebugPrint("fix - evaluating at the end", fixed, env);
    // env.increaseDebugIndentation();
    // auto ret = Eval(std::move(fixed), env);
    // env.decreaseDebugIndentation();
    // return ret;



    return "";
}

}