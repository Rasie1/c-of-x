#include "operations.h"
#include "robin_hood.h"

namespace cx {

expression GetElement(expression&& set) {
    return std::visit(overload{
        [](rec<equals_to>&& e) { return e.get().x; },
        [](auto&& e) -> expression {
            return error{Show(std::move(e)) + " doesn't contain values"}; 
        }
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

inline expression Fix(expression&& expr, environment& env, robin_hood::unordered_set<std::string>& seen) {
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
            if (seen.contains(e.name))
                return any{};
            seen.insert(e.name);
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
        [&env, &seen](rec<addition_with_expr>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<subtraction_with_expr>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env, seen);
            return function;
        },
        [&env, &seen](rec<multiplication_with_expr>&& function) -> expression {
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
    robin_hood::unordered_set<std::string> seen;
    return Fix(std::move(expr), env, seen);
}

inline expression MkAddition(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression { return addition_with{x}; },
        [](std::string&& x) -> expression { return addition_with{x}; },
        [](auto&& e) -> expression { return addition_with_expr{std::move(e)}; }
        // [](auto&& e) -> expression { return error{"can't make addition with " + Show(std::move(e))}; }
    }, std::move(argument));
}

inline expression MkSubtraction(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression { return subtraction_with{x}; },
        [](auto&& e) -> expression { return subtraction_with_expr{std::move(e)}; }
        // [](auto&& e) -> expression { return error{"can't make subtraction with " + Show(std::move(e))}; }
    }, std::move(argument));
}

inline expression MkMultiplication(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression { return multiplication_with{x}; },
        [](auto&& e) -> expression { return multiplication_with_expr{std::move(e)}; }
        // [](auto&& e) -> expression { return error{"can't make multiplication with " + Show(std::move(e))}; }
    }, std::move(argument));
}

inline expression MkIntersection(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression { return intersection_with{x}; },
        [](auto&& e) -> expression { return error{"can't make intersection with " + Show(std::move(e))}; }
    }, std::move(argument));
}

inline expression Negate(expression&& f, environment&) {
    return std::visit(overload{
        [](int&&) -> expression { return nothing{}; },
        [](std::string&&) -> expression { return nothing{}; },
        [](any&&) -> expression { return nothing{}; },
        [](unit&&) -> expression { return nothing{}; },
        [](nothing&&) -> expression { return any{}; },
        [](identifier&& e) -> expression { return e; },
        [](auto&& e) -> expression { return negated{std::move(e)}; }
        // [](auto&& e) -> expression { return error{"can't negate " + Show(std::move(e))}; }
    }, std::move(f));
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

expression Equals(expression&& l,
                  expression&& r,
                  environment& env) {
    expression lCopy = l;
    expression rCopy = r;
    if (auto added = ExtendEnvironment(equals_to{std::move(l)}, r, env)) {
        ExtendEnvironment(equals_to{std::move(r)}, lCopy, env);
        return identifier{*added};
    } else if (auto added = ExtendEnvironment(equals_to{std::move(r)}, lCopy, env)) {
        return identifier{*added};
    }
    l = std::move(Eval(std::move(lCopy), env)); 
    r = std::move(Eval(std::move(rCopy), env));

    DebugPrint("eq1", l, env);
    DebugPrint("eq2", r, env);
    if (l == r)
        return r; // TODO: is that really needed?
    return std::visit(overload{
        equals_for_datatype<int>{r},
        equals_for_datatype<std::string>{r},
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
        [&r](rec<application>&& e) -> expression { return make_operation<equals_to>(std::move(e), std::move(r)); },
        [](auto&&) -> expression { return nothing{}; }
    }, std::move(l));
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

inline expression MkEquals(expression&& argument) {
    return equals_to{expression(std::move(argument))};
}

inline bool IsError(expression& e) {
    return std::visit(overload{
        [](nothing&&) { return true; },
        [](error&&) { return true; },
        [](auto&&) { return false; }
    }, std::move(e));
}

// bool Unapply(expression&& pattern, 
//                     expression&& match, 
//                     environment& env) {
//     return std::visit(overload{
//         [&env, &match](identifier&& e) {
//             auto newEnv = env;
//             auto evaluated = Eval(std::move(match), newEnv);
//             if (IsError(evaluated))
//                 return false;
//             // fix???
//             env.add(e.name, std::move(evaluated));

//             return true;
//         },
//         [](auto&& e) -> expression { return nothing{}; }
//     }, std::move(pattern));
// }

expression Apply(expression&& function, 
                 expression&& argument, 
                 environment& env) {
    DebugPrint("apply1", function, env);
    DebugPrint("apply2", argument, env);
    return std::visit(overload{
        [&env, &argument](rec<closure>&& function) -> expression { 
            if (auto inputId = std::get_if<identifier>(&function.get().argument)) {
                if (auto argumentId = std::get_if<identifier>(&argument)) {
                    expression argumentValue = *env.get(argumentId->name);
                    function.get().env.add(inputId->name, std::move(argumentValue));
                } else {
                    function.get().env.add(inputId->name, std::move(argument));
                }

                auto evaluated = Eval(std::move(function.get().body), function.get().env);
                // return Fix(std::move(evaluated), function.get().env); // removes laziness :(

                if (auto outputId = std::get_if<identifier>(&evaluated)) {
                    return *function.get().env.get(outputId->name); // todo: replace all variables. Fix?
                } else {
                    return evaluated;
                }
            } else {
                return error{"pattern matching not yet implemented"};
            }
        },
        [&env, &argument](addition&&) { return MkAddition(Eval(std::move(argument), env)); },
        [&env, &argument](addition_with<int>&& function) {
            return Add(std::move(function), Eval(std::move(argument), env));
        },
        [&env, &argument](addition_with<std::string>&& function) {
            return Add(std::move(function), Eval(std::move(argument), env));
        },
        [&env, &argument](subtraction&&) { return MkSubtraction(Eval(std::move(argument), env)); },
        [&env, &argument](subtraction_with<int>&& function) {
            return Subtract(std::move(function), Eval(std::move(argument), env));
        },
        [&env, &argument](multiplication&&) { return MkMultiplication(Eval(std::move(argument), env)); },
        [&env, &argument](multiplication_with<int>&& function) {
            return Multiply(std::move(function), Eval(std::move(argument), env));
        },
        [&env, &argument](equality&&) { return MkEquals(Eval(std::move(argument), env)); },
                                            // todo: incorrect? should be evaluated?
        [&env, &argument](inequality&&) -> expression { return negated{MkEquals(Eval(std::move(argument), env))}; },
        [&env, &argument](intersection&&) { return MkIntersection(Eval(std::move(argument), env)); },
        [&env, &argument](show&&) -> expression { return Show(Fix(std::move(argument), env)); },
        [&env, &argument](print&&) -> expression { return Print(std::move(argument), env); },
        [&env, &argument](rec<equals_to>&& e) -> expression { return Equals(std::move(e.get().x), std::move(argument), env); },
        // [&env, &argument](rec<negated>&& e) -> expression { return Negate(std::move(e.get().f), env); },
        [&env, &argument](rec<intersection_with>&& function) -> expression {
            auto l = Eval(std::move(function.get().x), env); 
            auto r = Eval(std::move(argument), env);
            return Intersect(std::move(l), std::move(r));
        },
        [&argument](identifier&& f) -> expression { return application{std::move(f), std::move(argument)}; },
        [&argument](auto&& e) -> expression {
            return error{"can't apply " + Show(std::move(e)) + " to " + Show(std::move(argument)) };
        }
    }, std::move(function));
}

expression Eval(expression&& e, 
                environment& env) {
    DebugPrint("eval", e, env);
    env.increaseDebugIndentation();
    auto ret = std::visit(overload{
        [&env](rec<application>&& e) {
            auto argument = Eval(std::move(e.get().argument), env);
            auto function = Eval(std::move(e.get().function), env);
            return Apply(std::move(function), std::move(argument), env);
        },
        [&env](rec<then>&& e) {
            auto from = Eval(std::move(e.get().from), env);
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
        [&env](rec<abstraction>&& e) -> expression { return closure{std::move(e.get().argument), std::move(e.get().body), env}; },
        [](auto&& e) -> expression { return e; }
    }, std::move(e));
    env.decreaseDebugIndentation();
    return ret;
}

}