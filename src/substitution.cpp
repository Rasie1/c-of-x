#include "substitution.h"
#include "robin_hood.h"
#include "functor.h"
#include "operations_for_datatypes.h"
#include "unapply.h"

namespace cx {

template<typename operation>
expression map_union(operation&& op, expression&& e, environment& env) {
    DebugPrint("mapping union", e, env);
    return match(move(e),
        [&](rec<application>&& lApplication) -> expression {
            auto& rUnion = lApplication->argument;
            return match(move(lApplication->function),
                [&env, &rUnion, &op](rec<union_with>&& lUnion) -> expression {
                    env.increaseDebugIndentation();
                    defer { env.decreaseDebugIndentation(); };
                    auto lCalculated = map_union(op, move(lUnion->x), env);
                    auto rCalculated = map_union(op, move(rUnion),         env);
                    return Union(move(lCalculated), move(rCalculated));
                },
                [&lApplication, &env, &op](auto&& e) -> expression {
                    DebugPrint("union variant", e, env);
                    env.increaseDebugIndentation();
                    defer { env.decreaseDebugIndentation(); };
                    auto applied = op(move(e), env);
                    DebugPrint("union variant result", applied, env);
                    return application{move(applied), move(lApplication->argument)};
                }
            );
        },
        [&](rec<equals_to>&& e) -> expression {
            return equals_to{map_union(move(op), move(e->x), env)};
        },
        [&](auto&& e) -> expression {
            DebugPrint("union variant", e, env);
            env.increaseDebugIndentation();
            defer { env.decreaseDebugIndentation(); };
            auto applied = op(move(e), env);
            DebugPrint("union variant result", applied, env);
            return applied;
        }
    );
}

inline expression SubstituteVariables(expression&& expr, environment& env, std::vector<std::string>& seen) {
    // TODO: possibly this should be BFS
    DebugPrint("substitute - evaluating", expr, env);
    env.increaseDebugIndentation();
    auto evaluated = Eval(move(expr), env);
    env.decreaseDebugIndentation();
    DebugPrint("substituting", evaluated, env);
    env.increaseDebugIndentation();
    auto fixed = match(move(evaluated),
        [&env, &seen](identifier&& e) -> expression {
            DebugPrint("substituting in id", e, env);
            seen.push_back(e.name);
            if (auto expr = env.get(e.name)) {
                DebugPrint("substituting in id, got from env", *expr, env);
                // return map_union([&seen, &e](expression&& expr, environment& env){
                //     auto newEnv = env;
                //     *newEnv.get(e.name) = move(expr);
                //     return GetElement(SubstituteVariables(move(expr), newEnv, seen), newEnv);
                // }, copy(*expr), env);
                // return GetElement(SubstituteVariables(copy(*expr), env, seen), env);
                auto element = GetElement(copy(*expr), env);
                return map_union([&seen, &e](expression&& expr, environment& env){
                    auto newEnv = env;
                    *newEnv.get(e.name) = copy(expr);
                    return SubstituteVariables(move(expr), newEnv, seen);
                }, move(element), env);
                // return element;
            }
            return e;
        },
        [&env, &seen](rec<equals_to>&& function) -> expression {
            function->x = SubstituteVariables(move(function->x), env, seen);
            return function;
        },
        [&env, &seen](rec<negated>&& function) -> expression {
            function->f = SubstituteVariables(move(function->f), env, seen);
            return function;
        },
        [&env, &seen](rec<intersection_with>&& function) -> expression {
            function->x = SubstituteVariables(move(function->x), env, seen);
            return function;
        },
        [&env, &seen](rec<union_with>&& function) -> expression {
            function->x = SubstituteVariables(move(function->x), env, seen);
            return function;
        },
        [&env, &seen](rec<addition_with>&& function) -> expression {
            function->x = SubstituteVariables(move(function->x), env, seen);
            return function;
        },
        [&env, &seen](rec<subtraction_with>&& function) -> expression {
            function->x = SubstituteVariables(move(function->x), env, seen);
            return function;
        },
        [&env, &seen](rec<multiplication_with>&& function) -> expression {
            function->x = SubstituteVariables(move(function->x), env, seen);
            return function;
        },
        [&env, &seen](rec<application>&& e) -> expression {
            e->function = SubstituteVariables(move(e->function), env, seen);
            e->argument = SubstituteVariables(move(e->argument), env, seen);
            return e;
        },
        [&env, &seen](rec<then>&& e) -> expression {
            e->from = SubstituteVariables(move(e->from), env, seen);
            e->to = SubstituteVariables(move(e->to), env, seen);

            if (!env.isExecuting) {
                return then{move(e->from), move(e->to)};
            } else {
                return e->to;
            }
        },
        [](auto&& e) -> expression { return e; }
    );
    env.decreaseDebugIndentation();
    DebugPrint("substitute - evaluating at the end", fixed, env);
    env.increaseDebugIndentation();
    auto ret = Eval(move(fixed), env);
    env.decreaseDebugIndentation();
    return ret;
}

expression SubstituteVariables(expression&& expr, environment& env) {
    std::vector<std::string> seen;
    return SubstituteVariables(move(expr), env, seen);
}

std::pair<expression, std::optional<std::string>> FixWithVariable(expression&& expr, environment& env) {
    std::vector<std::string> seen;
    std::optional<std::string> variable;
    auto fixed = SubstituteVariables(move(expr), env, seen);
    if (!seen.empty())
        variable = seen.back();
    return {move(fixed), variable};
}

std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env) {
    if (auto id = std::get_if<identifier>(&argument)) {
        DebugPrint("extending", argument, env, 2);
        DebugPrint("new component", function, env, 2);
        return env.add(id->name, move(function)) == environment::extension_result::Added
             ? std::optional<std::string>(id->name)
             : std::nullopt;
    }
    return std::nullopt;
}

}