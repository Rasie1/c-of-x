#include "operations.h"
#include "robin_hood.h"
#include "functor.h"
#include "operations_for_datatypes.h"
#include "unapply.h"

namespace cx {

expression GetElement(expression&& set, environment& env) {
    return match(std::move(set),
        [](rec<equals_to>&& e) { return e.get().x; },
        [&env](int&& e) -> expression {
            env.errors.push_back(Show(std::move(e)) + " doesn't contain values");
            return nothing{};
        },
        [&env](std::string&& e) -> expression {
            env.errors.push_back(Show(std::move(e)) + " doesn't contain values");
            return nothing{};
        },
        [](auto&& e) -> expression { return application{std::move(e), any{}}; }
    );
}

inline expression SubstituteVariables(expression&& expr, environment& env, std::vector<std::string>& seen) {
    // TODO: possibly this should be BFS
    DebugPrint("substitute - evaluating", expr, env);
    env.increaseDebugIndentation();
    auto evaluated = Eval(std::move(expr), env);
    env.decreaseDebugIndentation();
    DebugPrint("substituting", evaluated, env);
    env.increaseDebugIndentation();
    auto fixed = match(std::move(evaluated),
        [&env, &seen](identifier&& e) -> expression {
            DebugPrint("substituting in id", e, env);
            seen.push_back(e.name);
            if (auto expr = env.get(e.name)) {
                DebugPrint("substituting in id, got from env", *expr, env);
                return GetElement(SubstituteVariables(copy(*expr), env, seen), env);
            }
            return e;
        },
        [&env, &seen](rec<equals_to>&& function) -> expression {
            function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<negated>&& function) -> expression {
            function.get().f = SubstituteVariables(std::move(function.get().f), env, seen);
            return function;
        },
        [&env, &seen](rec<intersection_with>&& function) -> expression {
            function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<union_with>&& function) -> expression {
            function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<addition_with>&& function) -> expression {
            function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<subtraction_with>&& function) -> expression {
            function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<multiplication_with>&& function) -> expression {
            function.get().x = SubstituteVariables(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<application>&& e) -> expression {
            e.get().function = SubstituteVariables(std::move(e.get().function), env, seen);
            e.get().argument = SubstituteVariables(std::move(e.get().argument), env, seen);
            return e;
        },
        [&env, &seen](rec<then>&& e) -> expression {
            e.get().from = SubstituteVariables(std::move(e.get().from), env, seen);
            e.get().to = SubstituteVariables(std::move(e.get().to), env, seen);

            if (!env.isExecuting) {
                return then{std::move(e.get().from), std::move(e.get().to)};
            } else {
                return e.get().to;
            }
        },
        [](auto&& e) -> expression { return e; }
    );
    env.decreaseDebugIndentation();
    DebugPrint("substitute - evaluating at the end", fixed, env);
    env.increaseDebugIndentation();
    auto ret = Eval(std::move(fixed), env);
    env.decreaseDebugIndentation();
    return ret;
}

expression SubstituteVariables(expression&& expr, environment& env) {
    std::vector<std::string> seen;
    return SubstituteVariables(std::move(expr), env, seen);
}

std::pair<expression, std::optional<std::string>> FixWithVariable(expression&& expr, environment& env) {
    std::vector<std::string> seen;
    std::optional<std::string> variable;
    auto fixed = SubstituteVariables(std::move(expr), env, seen);
    if (!seen.empty())
        variable = seen.back();
    return {std::move(fixed), variable};
}

expression Negate(expression&& f, environment& env) {
    DebugPrint("negating", f, env, 2);
    auto evaluated = Eval(std::move(f), env);
    return match(std::move(evaluated),
        // [](int&&) -> expression { return nothing{}; },
        // [](std::string&&) -> expression { return nothing{}; },
        [](any&&) -> expression { return nothing{}; },
        // [](unit&&) -> expression { return nothing{}; },
        [](nothing&&) -> expression { return any{}; },
        [](identifier&& e) -> expression { return negated{std::move(e)}; },
        [](rec<negated>&& e) -> expression { return e.get().f; },
        unmapped<equals_to, negated>(env),
        // [](auto&& e) -> expression { return make_operation<intersection_with>(any{}, negated{std::move(e)}); }
        [](auto&& e) -> expression { return negated{std::move(e)}; }
    );
}

std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env) {
    if (auto id = std::get_if<identifier>(&argument)) {
        DebugPrint("extending", argument, env, 2);
        DebugPrint("new component", function, env, 2);
        return env.add(id->name, std::move(function)) == environment::extension_result::Added
             ? std::optional<std::string>(id->name)
             : std::nullopt;
    }
    return std::nullopt;
}

std::optional<expression> IntersectFind(
    expression&& l,
    expression&& r,
    environment& env) {
    DebugPrint("intersect-find", l, env);
    if (l == r)
        return l;
    DebugPrint("intersect-find-2", r, env);
    // return make_operation<intersection_with>(std::move(l), std::move(r));
    auto intersected = Intersect(std::move(l), std::move(r), env);
    DebugPrint("intersect-find return", intersected, env);
    return match(std::move(intersected),
        [](nothing&&) -> std::optional<expression> { return {}; },
        [&](rec<equals_to>&& e) -> std::optional<expression> {
            // todo: make it fully recursive
            // DebugPrint("intersect-find eq", e, env);
            return match(std::move(e.get().x),
                [](nothing&&) -> std::optional<expression> { return {}; },
                [](auto&& e) -> std::optional<expression> { return {equals_to{std::move(e)}}; }
            );
        },
        [](auto&& e) -> std::optional<expression> { return { e }; }
    );
}

expression Intersect(expression&& l,
                     expression&& r,
                     environment& env) {
    DebugPrint("intersect1", l, env);
    DebugPrint("intersect2", r, env);
    if (l == r)
        return r;

    // todo: perhaps it's an extra eval here, but it seems to work with it
    l = Eval(std::move(l), env);
    r = Eval(std::move(r), env);
    if (l == r)
        return r;
    {
        DebugPrint("intersect unapply l", l, env);
        env.increaseDebugIndentation();
        defer { env.decreaseDebugIndentation(); };
        if (Unapply(copy(l), copy(r), env))
            return r;
        env.errors.clear();
    }
    {
        DebugPrint("intersect unapply r", r, env);
        env.increaseDebugIndentation();
        defer { env.decreaseDebugIndentation(); };
        if (Unapply(copy(r), copy(l), env))
            return l;
        env.errors.clear();
    }
    auto result = match(std::move(l),
        intersect_for_datatype<int>{r, env},
        intersect_for_datatype<basic_type<int>>{r, env},
        intersect_for_datatype<std::string>{r, env},
        intersect_for_datatype<basic_type<std::string>>{r, env},
        [&r, &env](rec<equals_to>&& l) -> expression {
            return match(std::move(r),
                [&l, &env](rec<equals_to>&& r) -> expression { return equals_to{Intersect(std::move(l.get().x), std::move(r.get().x), env)}; },
                [&l](any&&) -> expression { return l; },
                [&l](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(l), std::move(v)); },
                [&env](auto&& e) -> expression { 
                    env.errors.push_back("can't intersect with " + Show(std::move(e)));
                    return nothing{}; 
                }
            );
        },
        [&r, &env](rec<intersection_with>&& l) -> expression {
            return match(std::move(r),
                [&l, &env](rec<intersection_with>&& r) -> expression { 
                    return intersection_with{Intersect(std::move(l.get().x), std::move(r.get().x), env)}; 
                },
                [&l](any&&) -> expression { return l; },
                [&l](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(l), std::move(v)); },
                [&env](auto&& e) -> expression { 
                    env.errors.push_back("can't intersect with " + Show(std::move(e)));
                    return nothing{}; 
                }
            );
        },
        [&r, &env](rec<application>&& lApplication) -> expression {
            auto rCopy = r;
            auto lCopy = lApplication.get();
            auto mapped = map_union_l{rCopy, env, Intersect}.operator()<true>(std::move(lApplication.get()));
            return mapped;
        },
        [&r, &env](rec<closure>&& l) -> expression {
            return make_operation<intersection_with>(std::move(l), std::move(r));
        },
        [&r](any&&) -> expression { return r; },
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
        [](nothing&& e) -> expression { return e; },
        [&env](auto&& e) -> expression {
            env.errors.push_back(std::string("can't intersect ") + Show(e));
            return nothing{};
        }
    );
    DebugPrint("intersect result", result, env);
    
    return result;
}

expression Union(expression&& l, expression&& r) {
    return match(std::move(l),
        [&r](nothing&&) -> expression { return r; },
        [](any&& e) -> expression { return e; },
        [&r](auto&& l) -> expression { 
            return match(std::move(r),
                [](any&& e) -> expression { return e; },
                [&l](nothing&&) -> expression { return l; },
                [&l](auto&& r) -> expression { return make_operation<union_with>(std::move(l), std::move(r)); }
            ); 
        }
    );
}

}