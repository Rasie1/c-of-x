#include "eval.h"
#include "operations.h"

namespace cx {

template<class f>
void traverse(expression& e, f&& function) {
    match(e, function, [](auto&){});
    
    return match(e,
        [&function](rec<intersection_with>& e) {
            traverse(e->x, function);
        },
        [&function](rec<union_with>& e) {
            traverse(e->x, function);
        },
        [&function](rec<equals_to>& e) {
            traverse(e->x, function);
        },
        [&function](rec<closure>& e) {
            traverse(e->argument, function);
            traverse(e->body, function);
        },
        [&function](rec<addition_with>& e) {
            traverse(e->x, function);
        },
        [&function](rec<multiplication_with>& e) {
            traverse(e->x, function);
        },
        [&function](rec<subtraction_with>& e) {
            traverse(e->x, function);
        },
        [&function](rec<implication_with>& e) {
            traverse(e->x, function);
        },
        [&function](rec<application>& e) {
            traverse(e->function, function);
            traverse(e->argument, function);
        },
        [&function](rec<then>& e) {
            traverse(e->from, function);
            traverse(e->to, function);
        },
        [&function](rec<negated>& e) { 
            traverse(e->f, function);
        },
        [&function](rec<abstraction>& e) {
            traverse(e->argument, function);
            traverse(e->body, function);
        },
        [&function](rec<set>& e) { 
            for (auto& x: e->x) {
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
    auto ret = match(move(e),
        [&env](rec<application>&& e) {
            DebugPrint("apply", *e, env, 2);
            env.increaseDebugIndentation();
            defer { env.decreaseDebugIndentation(); }; 

            expression function, argument;

            {
                stash executionState(env.isExecuting, false);
                
                // function = SubstituteVariables(move(e->function), env);
                function = Eval(move(e->function), env);

                // only one variable definition is possible in each application chain
                if (auto idFunction = std::get_if<identifier>(&function); idFunction && !env.get(idFunction->name)) {
                    DebugPrint("id function in eval apply", *idFunction, env);
                    stash variables(env.variables);
                    argument = Eval(move(e->argument), env);
                    if (auto idArgument = std::get_if<identifier>(&argument)) {
                        auto newVariable = env.get(idArgument->name);
                        if (newVariable)
                            argument = application{move(*newVariable), move(argument)};
                    }
                } else if (std::get_if<rec<equals_to>>(&function)) {
                    DebugPrint("eq function in eval apply", e->argument, env);
                    stash variables(env.variables);
                    argument = Eval(move(e->argument), env);
                    // auto applied = Apply(move(function), move(e->argument), env);
                    // return applied;
                } else {
                    DebugPrint("not id function in eval apply", e->argument, env);
                    // some additional filter is needed. Env leaks here. It should go upper way i that test
                    argument = Eval(move(e->argument), env);
                    // if (std::get_if<identifier>(&argument))
                    // {
                    //     stash variables(env.variables);
                    //     return Apply(move(function), move(argument), env);
                    // }
                }
            }

            return Apply(move(function), move(argument), env);
        },
        [&env](rec<then>&& e) {
            auto from = SubstituteVariables(move(e->from), env);
            DebugPrint("then", 0, env, 2);
            return match(move(from),
                [&env](nothing&&) -> expression { 
                    env.errors.push_back("incorrect implication");
                    return nothing{}; 
                },
                [&e, &env](auto&& from) -> expression {
                    if (!env.isExecuting) {
                        stash executionState(env.isExecuting, false);
                        auto to = Eval(move(e->to), env); 
                        return then{move(from), move(to)};
                    } else {
                        auto to = Eval(move(e->to), env); 
                        return to;
                    }
                }
            );
        },
        [&env](rec<negated>&& e) -> expression { 
            auto function = Eval(move(e->f), env);
            return Negate(move(function), env); 
        },
        [&env](rec<abstraction>&& e) -> expression {
            DebugPrint("abstraction", 0, env, 2);

            stash variables(env.variables);
            stash isExecuting(env.isExecuting, false);

            auto pattern = Eval(move(e->argument), env);

            DebugPrint("new argument in abstraction", pattern, env);

            // shadowing
            // if (auto id = std::get_if<identifier>(&pattern)) {
            //     for (auto& [variable, value]: env.variables) {
            //         if (variable == id->name) {
            //             auto thisVariableInPreviousEnv = env.get(variable);
            //             if (thisVariableInPreviousEnv) {
            //                 shadowVariable(pattern, variable);
            //                 shadowVariable(e->body, variable);
            //                 variable += "'";

            //                 // if (*newlyDefinedVariable != value) {
            //                 // pattern = application{value, pattern};
            //                 // }
            //             }
            //             break;
            //         }
            //     }
            // }

            return closure{pattern, move(e->body), env}; 
        },
        [](auto&& e) -> expression { return e; }
    );
    env.decreaseDebugIndentation();
    DebugPrint("eval result", ret, env);
    return ret;
}

}