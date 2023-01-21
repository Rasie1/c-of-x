#include "operations.h"
#include "robin_hood.h"
#include "functor.h"
#include "operations_for_datatypes.h"

namespace cx {

expression GetElement(expression&& set) {
    return std::visit(overload{
        [](rec<equals_to>&& e) { return e.get().x; },
        [](int&& e) -> expression { return error{Show(std::move(e)) + " doesn't contain values"}; },
        [](std::string&& e) -> expression { return error{Show(std::move(e)) + " doesn't contain values"}; },
        [](auto&& e) -> expression { return application{std::move(e), any{}}; }
    }, std::move(set));
}

// inline expression Substitute(expression&& expr, environment& env) {
//     return std::visit(overload{
//         [&env, &seen](identifier&& e) -> expression {
//             if (auto expr = env.get(e.name)) {
//                 auto copy = *expr;
//                 DebugPrint("fixing in id, got from env", *expr, env);
//                 return GetElement(Substitute(std::move(copy), env, seen));
//             }
//             return error{std::string("undefined variable \"") + e.name + "\""};
//         },
//         [&env, &seen](rec<equals_to>&& function) -> expression {
//             function.get().x = Substitute(std::move(function.get().x), env, seen);
//             return function;
//         },
//         [&env, &seen](rec<negated>&& function) -> expression {
//             function.get().f = Substitute(std::move(function.get().f), env, seen);
//             return function;
//         },
//         [&env, &seen](rec<intersection_with>&& function) -> expression {
//             function.get().x = Substitute(std::move(function.get().x), env, seen);
//             return function;
//         },
//         [&env, &seen](rec<application>&& e) -> expression {
//             e.get().function = Substitute(std::move(e.get().function), env, seen);
//             e.get().argument = Substitute(std::move(e.get().argument), env, seen);
//             return e;
//         },
//         [&env, &seen](rec<then>&& e) -> expression {
//             e.get().from = Substitute(std::move(e.get().from), env, seen);
//             e.get().to = Substitute(std::move(e.get().to), env, seen);
//             return e;
//         },
//         [](auto&& e) -> expression { return e; }
//     }, std::move(expr));
// }

inline expression Fix(expression&& expr, environment& env, std::vector<std::string>& seen) {
    // TODO: possibly this should be BFS
    DebugPrint("fix - evaluating", expr, env);
    env.increaseDebugIndentation();
    auto evaluated = Eval(std::move(expr), env);
    env.decreaseDebugIndentation();
    DebugPrint("fixing", evaluated, env);
    env.increaseDebugIndentation();
    auto fixed = std::visit(overload{
        [&env, &seen](identifier&& e) -> expression {
            DebugPrint("fixing in id", e, env);
            if (std::find(seen.begin(), seen.end(), e.name) != seen.end())
                return any{}; // ?
            seen.push_back(e.name);
            if (auto expr = env.get(e.name)) {
                auto copy = *expr;
                DebugPrint("fixing in id, got from env", *expr, env);
                return GetElement(Fix(std::move(copy), env, seen));
            }
            return error{std::string("undefined variable \"") + e.name + "\""};
        },
        [&env, &seen](rec<equals_to>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<negated>&& function) -> expression {
            function.get().f = Fix(std::move(function.get().f), env, seen);
            return function;
        },
        [&env, &seen](rec<intersection_with>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<union_with>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<addition_with>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<subtraction_with>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<multiplication_with>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<application>&& e) -> expression {
            e.get().function = Fix(std::move(e.get().function), env, seen);
            e.get().argument = Fix(std::move(e.get().argument), env, seen);
            return e;
        },
        [&env, &seen](rec<then>&& e) -> expression {
            e.get().from = Fix(std::move(e.get().from), env, seen);
            e.get().to = Fix(std::move(e.get().to), env, seen);
            return e;
        },
        [](auto&& e) -> expression { return e; }
    }, std::move(evaluated));
    env.decreaseDebugIndentation();
    DebugPrint("fix - evaluating at the end", fixed, env);
    env.increaseDebugIndentation();
    auto ret = Eval(std::move(fixed), env);
    env.decreaseDebugIndentation();
    return ret;
}

expression Fix(expression&& expr, environment& env) {
    std::vector<std::string> seen;
    return Fix(std::move(expr), env, seen);
}

std::pair<expression, std::optional<std::string>> FixWithVariable(expression&& expr, environment& env) {
    std::vector<std::string> seen;
    std::optional<std::string> variable;
    if (!seen.empty())
        variable = seen.front();
    return {Fix(std::move(expr), env, seen), variable};
}

expression Negate(expression&& f, environment& env) {
    DebugPrint("negating", f, env, 2);
    return std::visit(overload{
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
        // [](auto&& e) -> expression { return error{"can't negate " + Show(std::move(e))}; }
    }, Eval(std::move(f), env));
}

std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env) {
    if (auto id = std::get_if<identifier>(&argument)) {
        DebugPrint("extending", argument, env, 2);
        DebugPrint("new complonent", function, env, 2);
        return env.add(id->name, std::move(function))
             ? std::optional<std::string>(id->name)
             : std::nullopt;
    }
    return std::nullopt;
}

expression Intersect(expression&& l,
                     expression&& r) {
    // DebugPrint("intersect1", l);
    // DebugPrint("intersect2", r);
    if (l == r)
        return r; // TODO: is that really needed?
    return std::visit(overload{
        intersect_for_datatype<int>{r},
        intersect_for_datatype<std::string>{r},
        [&r](rec<equals_to>&& l) -> expression {
            return std::visit(overload{
                [&l](rec<equals_to>&& r) -> expression { return equals_to{Intersect(std::move(l.get().x), std::move(r.get().x))}; },
                [&l](any&&) -> expression { return l; },
                [&l](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(l), std::move(v)); },
                [](auto&&) -> expression { return nothing{}; }
            }, std::move(r)); 
        },
        [&r](rec<intersection_with>&& l) -> expression {
            return std::visit(overload{
                [&l](rec<intersection_with>&& r) -> expression { 
                    return intersection_with{Intersect(std::move(l.get().x), std::move(r.get().x))}; 
                },
                [&l](any&&) -> expression { return l; },
                [&l](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(l), std::move(v)); },
                [](auto&&) -> expression { return nothing{}; }
            }, std::move(r)); 
        },
        [&r](any&&) -> expression { return r; },
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
        [](nothing&& e) -> expression { return e; },
        [](auto&& e) -> expression { return error{std::string("can't intersect ") + Show(e)}; }
    }, std::move(l));
}

expression Union(expression&& l, expression&& r) {
    return std::visit(overload{
        [&r](nothing&&) -> expression { return r; },
        [](any&& e) -> expression { return e; },
        [&r](auto&& l) -> expression { 
            return std::visit(overload{
                [](any&& e) -> expression { return e; },
                [&l](nothing&&) -> expression { return l; },
                [&l](auto&& r) -> expression { return make_operation<union_with>(std::move(l), std::move(r)); }
            }, std::move(r)); 
        }
    }, std::move(l));
}

}