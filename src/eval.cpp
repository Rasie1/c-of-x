#include "eval.h"
#include "operations.h"

namespace cx {

template<class f>
void traverse(expression& e, f&& function) {
    match(e, function, [](auto&){});
    
    return match(e,
        [&function](rec<intersection_with>& e) {
            traverse(e.get().x, function);
        },
        [&function](rec<union_with>& e) {
            traverse(e.get().x, function);
        },
        [&function](rec<equals_to>& e) {
            traverse(e.get().x, function);
        },
        [&function](rec<closure>& e) {
            traverse(e.get().argument, function);
            traverse(e.get().body, function);
        },
        [&function](rec<addition_with>& e) {
            traverse(e.get().x, function);
        },
        [&function](rec<multiplication_with>& e) {
            traverse(e.get().x, function);
        },
        [&function](rec<subtraction_with>& e) {
            traverse(e.get().x, function);
        },
        [&function](rec<implication_with>& e) {
            traverse(e.get().x, function);
        },
        [&function](rec<application>& e) {
            traverse(e.get().function, function);
            traverse(e.get().argument, function);
        },
        [&function](rec<then>& e) {
            traverse(e.get().from, function);
            traverse(e.get().to, function);
        },
        [&function](rec<negated>& e) { 
            traverse(e.get().f, function);
        },
        [&function](rec<abstraction>& e) {
            traverse(e.get().argument, function);
            traverse(e.get().body, function);
        },
        [&function](rec<set>& e) { 
            for (auto& x: e.get().x) {
                traverse(x, function);
            }
        },
        [](auto&){}
    );
}

void shadowVariables(expression& e) {
    traverse(e, [](identifier& id){
        id.name += '\'';
    });
}

void shadowVariable(expression& e, const std::string& name) {
    traverse(e, [&name](identifier& id){
        if (name == id.name)
            id.name += '\'';
    });
}

expression Eval(expression&& e, 
                environment& env) {
    DebugPrint("eval", e, env);
    env.increaseDebugIndentation();
    auto ret = match(std::move(e),
        [&env](rec<application>&& e) {
            DebugPrint("apply", e.get(), env, 2);
            env.increaseDebugIndentation();
            defer { env.decreaseDebugIndentation(); }; 

            expression function, argument;

            {
                stash executionState(env.isExecuting, false);
                
                function = Eval(std::move(e.get().function), env);
                argument = Eval(std::move(e.get().argument), env);
            }

            auto applied = Apply(std::move(function), std::move(argument), env);

            return applied;
        },
        [&env](rec<then>&& e) {
            auto from = Eval(std::move(e.get().from), env);
            DebugPrint("then", 0, env, 2);
            return match(std::move(from),
                [&env](nothing&&) -> expression { 
                    env.errors.push_back("incorrect implication");
                    return nothing{}; 
                },
                [&e, &env](auto&& from) -> expression {
                    // bool wasExecuting = env.isExecuting;
                    // stash executionState(env.isExecuting, false);
                    // auto to = Eval(std::move(e.get().to), env); 
                    // if (!wasExecuting) {
                    //     return then{std::move(from), std::move(to)};
                    // } else {
                    //     return to;
                    // }
                    if (!env.isExecuting) {
                        stash executionState(env.isExecuting, false);
                        auto to = Eval(std::move(e.get().to), env); 
                        return then{std::move(from), std::move(to)};
                    } else {
                        auto to = Eval(std::move(e.get().to), env); 
                        return to;
                    }
                }
            );
        },
        [&env](rec<negated>&& e) -> expression { 
            auto function = Eval(std::move(e.get().f), env);
            return Negate(std::move(function), env); 
        },
        [&env](rec<abstraction>&& e) -> expression {
            DebugPrint("-- Constructing closure", 0, env, 2);
            for (auto& [k, v]: env.variables) 
                DebugPrint(k, v, env);

            auto newEnv = env;
            newEnv.isExecuting = false;

            auto pattern = Eval(std::move(e.get().argument), newEnv);

            if (auto id = std::get_if<identifier>(&pattern)) {
                for (auto& [variable, value]: newEnv.variables) {
                    if (variable == id->name) {
                        shadowVariable(pattern, variable);
                        shadowVariable(e.get().body, variable);
                        break;
                    }
                }
            }

            return closure{pattern, std::move(e.get().body), newEnv}; 
        },
        [](auto&& e) -> expression { return e; }
    );
    env.decreaseDebugIndentation();
    DebugPrint("eval result", ret, env);
    return ret;
}

}