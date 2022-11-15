#include "operations.h"
#include "robin_hood.h"

namespace cx {


template<typename datatype>
struct intersect_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](any&&) -> expression { return l; },
            [&l](identifier&& v) -> expression { return make_operation<intersection_with>(l, std::move(v)); },
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};


template<typename datatype>
struct equals_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](identifier&& v) -> expression { return make_operation<equals_to>(l, std::move(v)); },
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};

template<typename datatype>
struct addition_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return l + r; },
            [&l](identifier&& v) -> expression { return make_operation<addition_with>(l, std::move(v)); },
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};
template<typename datatype>
struct subtraction_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return l - r; },
            [&l](identifier&& v) -> expression { return make_operation<subtraction_with>(l, std::move(v)); },
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};
template<typename datatype>
struct multiplication_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return l * r; },
            [&l](identifier&& v) -> expression { return make_operation<multiplication_with>(l, std::move(v)); },
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};

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
                return any{}; // ?
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
    robin_hood::unordered_set<std::string> seen;
    return Fix(std::move(expr), env, seen);
}

inline expression Negate(expression&& f, environment&) {
    return std::visit(overload{
        [](int&&) -> expression { return nothing{}; },
        [](std::string&&) -> expression { return nothing{}; },
        [](any&&) -> expression { return nothing{}; },
        [](unit&&) -> expression { return nothing{}; },
        [](nothing&&) -> expression { return any{}; },
        [](identifier&& e) -> expression { return e; },
        // [](rec<equals_to>&& e) -> expression { return e; },
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
    l = std::move(Eval(std::move(l), env)); 
    r = std::move(Eval(std::move(r), env));
    if (l == r)
        return r;
    expression lCopy = l;
    expression rCopy = r;
    DebugPrint("eq1", l, env);
    DebugPrint("eq2", r, env);
    auto isUnapplySuccessful = Unapply(std::move(l), std::move(r), env);
    if (isUnapplySuccessful)
        return rCopy;
    // return nothing{};

    return std::visit(overload{
        equals_for_datatype<int>{rCopy},
        equals_for_datatype<std::string>{rCopy},
        [&rCopy](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(rCopy)); },
        [&rCopy](rec<application>&& e) -> expression { return make_operation<equals_to>(std::move(e), std::move(rCopy)); },
        [](auto&&) -> expression { return nothing{}; }
    }, std::move(lCopy));


    // probably should also use what's below instead of just returning rCopy or nothing

    // expression lCopy = l;
    // expression rCopy = r;
    // if (auto added = ExtendEnvironment(equals_to{std::move(l)}, r, env)) {
    //     ExtendEnvironment(equals_to{std::move(r)}, lCopy, env);
    //     return identifier{*added};
    // } else if (auto added = ExtendEnvironment(equals_to{std::move(r)}, lCopy, env)) {
    //     return identifier{*added};
    // }
    // l = std::move(Eval(std::move(lCopy), env)); 
    // r = std::move(Eval(std::move(rCopy), env));

    // DebugPrint("eq1", l, env);
    // DebugPrint("eq2", r, env);
    // if (l == r)
    //     return r; // TODO: is that really needed?
    // return std::visit(overload{
    //     equals_for_datatype<int>{r},
    //     equals_for_datatype<std::string>{r},
    //     [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
    //     [&r](rec<application>&& e) -> expression { return make_operation<equals_to>(std::move(e), std::move(r)); },
    //     [](auto&&) -> expression { return nothing{}; }
    // }, std::move(l));
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

template <template<typename datatype> typename operation_for_datatype, typename functor>
expression Calculate(expression&& l,
                     expression&& r,
                     environment& env) {
    r = Eval(std::move(r), env);
    return std::visit(overload{
        operation_for_datatype<int>{r},
        // operation_for_datatype<std::string>{r},
        [&r](identifier&& v) -> expression { return make_operation<functor>(std::move(v), std::move(r)); },
        [](auto&& e) -> expression { return error{std::string("can't do arithmetic with ") + Show(e)}; }
    }, Eval(std::move(l), env));
}

inline bool IsError(expression& e) {
    return std::visit(overload{
        [](nothing&&) { return true; },
        [](error&&) { return true; },
        [](auto&&) { return false; }
    }, std::move(e));
}

expression Apply(expression&& function, 
                 expression&& argument, 
                 environment& env) {
    DebugPrint("apply1", function, env);
    DebugPrint("apply2", argument, env);
    return std::visit(overload{
        [&env, &argument](rec<closure>&& function) -> expression { 
            if (auto inputId = std::get_if<identifier>(&function.get().argument)) {
                DebugPrint("applying closure", function, env);
                if (auto argumentId = std::get_if<identifier>(&argument)) {
                    expression argumentValue = *env.get(argumentId->name);
                    function.get().env.add(inputId->name, std::move(argumentValue));
                } else {
                    function.get().env.add(inputId->name, equals_to{std::move(argument)});
                }

                // auto evaluated = Eval(std::move(function.get().body), function.get().env);
                return Fix(std::move(function.get().body), function.get().env); // removes laziness :(

                // if (auto outputId = std::get_if<identifier>(&evaluated)) {
                //     return *function.get().env.get(outputId->name); // todo: replace all variables. Fix?
                // } else {
                //     return evaluated;
                // }
            } else {
                return error{"pattern matching not yet implemented"};
            }
        },
        [&env, &argument](addition&&) -> expression { return addition_with{Eval(std::move(argument), env)}; },
        [&env, &argument](rec<addition_with>&& function) -> expression {
            return Calculate<addition_for_datatype, addition_with>(std::move(function.get().x), std::move(argument), env);
        },
        [&env, &argument](subtraction&&) -> expression { return subtraction_with{Eval(std::move(argument), env)}; },
        [&env, &argument](rec<subtraction_with>&& function) -> expression {
            return Calculate<subtraction_for_datatype, subtraction_with>(std::move(function.get().x), std::move(argument), env);
        },
        [&env, &argument](multiplication&&) -> expression { return multiplication_with{Eval(std::move(argument), env)}; },
        [&env, &argument](rec<multiplication_with>&& function) -> expression {
            return Calculate<multiplication_for_datatype, multiplication_with>(std::move(function.get().x), std::move(argument), env);
        },
        [&env, &argument](equality&&) -> expression { return equals_to{Eval(std::move(argument), env)}; },
        [&env, &argument](inequality&&) -> expression { return negated{
                equals_to{Eval(std::move(argument), env)}
            }; 
        },
        [&env, &argument](rec<application>&& e) -> expression {
            // eval and apply already happened
            return application{std::move(e), std::move(argument)};
        },
            // todo: isn't it evaluated already?
        [&env, &argument](intersection&&) -> expression { return intersection_with{Eval(std::move(argument), env)}; },
        [&env, &argument](show&&) -> expression { return Show(Fix(std::move(argument), env)); },
        [&env, &argument](print&&) -> expression { return Print(std::move(argument), env); },
        [&env, &argument](set_trace_enabled&&) -> expression { return SetTraceEnabled(std::move(argument), env); },
        [&env, &argument](rec<equals_to>&& e) -> expression { return Equals(std::move(e.get().x), std::move(argument), env); },
        // [&env, &argument](rec<negated>&& e) -> expression { return Negate(std::move(e.get().f), env); },
        [&env, &argument](rec<intersection_with>&& function) -> expression {
            // todo: isn't it evaluated already?
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

template<typename datatype>
struct unapply_for_datatype {
    expression& match;
    environment& env;
    bool operator()(datatype&& pattern) {
        return std::visit(overload{
            [&pattern](datatype&& match) -> bool { return pattern == match; },
            [&pattern, this](identifier&& match) -> bool { 
                return static_cast<bool>(ExtendEnvironment(equals_to{std::move(match)}, std::move(pattern), env)); 
            },
            [](auto&&) -> bool { return false; }
        }, std::move(match)); 
    }
};

bool Unapply(expression&& pattern, 
             expression&& match, 
             environment& env) {
    DebugPrint("unapply1", pattern, env);
    DebugPrint("unapply2", match, env);
    return std::visit(overload{
        unapply_for_datatype<int>{match, env},
        [&env, &match](identifier&& pattern) -> bool {
            auto newEnv = env;
            // fix or just eval???
            auto evaluated = Fix(std::move(match), newEnv);
            if (IsError(evaluated))
                return false;
            if (env.define(pattern.name, equals_to{std::move(evaluated)}))
                return true;
            else {
                DebugPrint("already defined", pattern, env, 2);
                return false;
            }
        },
        [&env, &match](rec<application>&& pattern) -> bool { 
            auto wrapped = abstraction{std::move(pattern.get().argument), match};
            return Unapply(std::move(pattern.get().function), std::move(wrapped), env);
        },
        [](auto&&) -> bool { return false; }
    }, std::move(pattern));
}

}