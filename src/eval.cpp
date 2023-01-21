#include "eval.h"
#include "operations.h"

namespace cx {

expression Eval(expression&& e, 
                environment& env) {
    DebugPrint("eval", e, env);
    env.increaseDebugIndentation();
    auto ret = std::visit(overload{
        [&env](rec<application>&& e) {
            // auto envCopy = env;
            auto function = Eval(std::move(e.get().function), env);
            auto argument = Eval(std::move(e.get().argument), env);
            // auto function = std::move(e.get().function);
            // auto argument = std::move(e.get().argument);
            return Apply(std::move(function), std::move(argument), env);
        },
        [&env](rec<then>&& e) {
            auto from = Eval(std::move(e.get().from), env);
            DebugPrint("then", 0, env, 2);
            return std::visit(overload{
                [](error&& x) -> expression { return x;},
                [](nothing&&) -> expression { return error{}; },
                [&e, &env](auto&&) -> expression { return Eval(std::move(e.get().to), env); }
            }, std::move(from));
        },
        [&env](rec<negated>&& e) -> expression { 
            auto function = Eval(std::move(e.get().f), env);
            return Negate(std::move(function), env); 
        },
        [&env](rec<abstraction>&& e) -> expression {
            DebugPrint("-- Constructing closure", 0, env, 2);
            for (auto& [k, v]: env.variables) {
                DebugPrint(k, v, env);
            }
            return closure{std::move(e.get().argument), std::move(e.get().body), env}; 
        },
        [](auto&& e) -> expression { return e; }
    }, std::move(e));
    env.decreaseDebugIndentation();
    DebugPrint("eval result", ret, env);
    return ret;
}

}