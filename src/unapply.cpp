#include "unapply.h"
#include "operations.h"

namespace cx {

inline bool IsError(expression& e) {
    return match(std::move(e),
        [](nothing&&) { return true; },
        [](error&&) { return true; },
        [](auto&&) { return false; }
    );
}

template<typename datatype>
struct unapply_for_datatype {
    expression& valueToMatch;
    environment& env;
    unapply_result operator()(datatype&& pattern) {
        return match(std::move(valueToMatch),
            [&pattern](datatype&& v) -> unapply_result { return {pattern == v, {}}; },
            [&pattern, this](identifier&& v) -> unapply_result { 
                auto defined = ExtendEnvironment(equals_to{std::move(v)}, std::move(pattern), env);
                if (defined)
                    return {true, {}};
                else
                    return {false, v.name};
            },
            [](auto&&) -> unapply_result { return {}; }
        ); 
    }

    unapply_result operator()(basic_type<datatype>&& pattern) {
        DebugPrint("typechecking", pattern, env);
        DebugPrint("typechecking value", valueToMatch, env);
        return match(std::move(valueToMatch),
            [&pattern](basic_type<datatype>&& valueToMatch) -> unapply_result { return {pattern == valueToMatch, {}}; },
            [&pattern, this](identifier&& valueToMatch) -> unapply_result {
                DebugPrint("type and identifier", pattern, env);
                auto defined = ExtendEnvironment(equals_to{std::move(valueToMatch)}, std::move(pattern), env);
                if (defined)
                    return {true, {}};
                else
                    return {false, valueToMatch.name};
            },
            [&pattern, this](rec<abstraction>&& e) -> unapply_result { // shouldn't this apply to above as well?
                auto a = e.get();
                auto bodyCopy = a.body;
                auto applied = Apply(std::move(pattern), std::move(a.body), env);
                DebugPrint("typechecking apply result", applied, env);
                if (applied == bodyCopy)
                    return Unapply(std::move(a.argument), std::move(applied), env);
                else 
                    return {};
            },
            [&pattern, this](rec<closure>&& e) -> unapply_result { 
                auto a = e.get();
                auto bodyCopy = a.body;
                // TODO: something could wrong with env. Should they be merged?
                auto applied = Apply(std::move(pattern), std::move(a.body), env);
                DebugPrint("typechecking apply result", applied, env);
                if (applied == bodyCopy)
                    return Unapply(std::move(a.argument), std::move(applied), env);
                else 
                    return {};
            },
            [&pattern, this](rec<equals_to>&& e) -> unapply_result { 
                auto newPattern = application{std::move(pattern), any{}};
                return Unapply(std::move(newPattern), std::move(e.get().x), env);
            },
            [](auto&&) -> unapply_result { return {}; }
        ); 
    }
};

std::optional<expression> Inverse(expression&& e, environment& env) {
    DebugPrint("inverting", e, env);
    env.increaseDebugIndentation();
    auto result = match(std::move(e),
        [](rec<addition_with>&& f) -> std::optional<expression> { 
            return subtraction_with{std::move(f.get().x)}; 
        },
        [&env](rec<application>&& app) -> std::optional<expression> {
            DebugPrint("inverting application", app.get(), env);
            return match(std::move(std::move(app.get().function)),
                [&app, &env](rec<union_with>&& lUnion) -> std::optional<expression> {
                    auto leftInverse = Inverse(std::move(lUnion.get()), env);
                    auto rightInverse = Inverse(std::move(app.get().argument), env);
                    if (leftInverse && rightInverse)
                        return application{union_with{*leftInverse}, *rightInverse};
                    else if (leftInverse)
                        return leftInverse;
                    else
                        return rightInverse;
                },
                [](auto&&) -> std::optional<expression> { return std::nullopt; }
            );
        },
        [](int&& x) -> std::optional<expression> { return x; },
        [](std::string&& x) -> std::optional<expression> { return x; },
        [](auto&&) -> std::optional<expression> { return std::nullopt; }
    );
    if (result)
        DebugPrint("inverse result", *result, env);
    env.decreaseDebugIndentation();
    return result;
}

// struct is_equal_with_negated {
//     expression& r;
//     environment& env;
//     inline auto operator()(rec<negated>&& e) -> unapply_result { 
//         DebugPrint("is equal with negated", e.get().f, env);
//         auto falseEnv = env;
//         auto eq = IsEqual(std::move(r), std::move(e.get().f), falseEnv);
//         DebugPrint("is equal result", eq, env);
//         return match(std::move(eq),
//             [](nothing&&) -> unapply_result { return {true, {}}; },
//             [](identifier&&) -> unapply_result { return {true, {}}; },
//             [](auto&&) -> unapply_result { return {}; }
//         );
//     }
// };

struct map_unapply_union_l {
    expression& r;
    environment& env;
    auto operator()(rec<application>&& lApplication) -> unapply_result {
        auto& rUnion = lApplication.get().argument;
        return match(std::move(std::move(lApplication.get().function)),
            [this, &rUnion](rec<union_with>&& lUnion) -> unapply_result {
                auto rCopy = r;
                auto lCalculated = Unapply(
                    std::move(lUnion.get().x), std::move(r), env);
                auto rCalculated = Unapply(
                    std::move(rUnion), std::move(rCopy), env);
                return {lCalculated.success || rCalculated.success, 
                        lCalculated.outerVariable.empty() ? rCalculated.outerVariable : lCalculated.outerVariable 
                };
            },
            [](auto&&) -> unapply_result { 
                return {};
            }
        );
    }
};

struct equals_with_negated {
    expression& r;
    environment& env;
    inline auto operator()(rec<negated>&& e) -> unapply_result { 
        DebugPrint("is equal with negated", e.get().f, env);
        auto falseEnv = env;
        auto [evaluatedl, lvar] = FixWithVariable(std::move(e.get().f), falseEnv);
        auto [evaluatedr, rvar] = FixWithVariable(std::move(r), falseEnv);
        DebugPrint(std::string("unapply ne, l(") + (lvar?(*lvar):std::string("-")) + ")", evaluatedl, env);
        DebugPrint(std::string("unapply ne, r(") + (rvar?(*rvar):std::string("-")) + ")", evaluatedr, env);
        auto eq = IsEqual(std::move(evaluatedl), std::move(evaluatedr), falseEnv);
        DebugPrint("unapply ne, is equal", eq, env);
        return match(std::move(eq),
            [&lvar, &rvar](nothing&&) -> unapply_result { 
                if (lvar)
                    return {true, {*lvar}};
                else if (rvar)
                    return {true, {*rvar}};
                else
                    return {true, {}};
            },
            map_unapply_union_l{r, env},
            // map intersection
            // [](identifier&& e) -> unapply_result { return negated{std::move(e)}; },
            [](identifier&& e) -> unapply_result { return {false, {e.name}}; },
            [](auto&&) -> unapply_result { return {}; }
        );
    }
};

unapply_result Unapply(expression&& pattern, 
                       expression&& valueToMatch, 
                       environment& env) {
    DebugPrint("unapply1", pattern, env);
    DebugPrint("unapply2", valueToMatch, env);
    env.increaseDebugIndentation();
    auto ret = match(std::move(pattern),
        unapply_for_datatype<int>{valueToMatch, env},
        // unapply_for_datatype<basic_type<int>>{valueToMatch, env},
        unapply_for_datatype<std::string>{valueToMatch, env},
        // unapply_for_datatype<basic_type<std::string>>{valueToMatch, env},
        [](any&&) -> unapply_result { return {true, {}}; },
        equals_with_negated{valueToMatch, env},
        [&env, &valueToMatch](identifier&& pattern) -> unapply_result {
            auto newEnv = env;
            auto oldEvaluated = Eval(std::move(valueToMatch), newEnv);
            auto oldCopy = oldEvaluated;
            auto evaluated = SubstituteVariables(std::move(oldEvaluated), newEnv); // danger
            if (IsError(evaluated))
                return {};
            expression newVariable;
            if (oldCopy == evaluated)
                newVariable = std::move(evaluated);
            else
                newVariable = make_operation<intersection_with>(std::move(oldCopy), std::move(evaluated));
            DebugPrint(std::string("defining variable ") + pattern.name, newVariable, env, 2);
            // define only tries to add new data and it works
            // add also adds checks about all calculations
            // but these should be divided from predicates and not participate in env get
            auto isEnvironmentExtended = env.add(pattern.name, equals_to{copy(newVariable)});
            if (isEnvironmentExtended == environment::extension_result::Added) {
                return {true, pattern.name};
            } else if (isEnvironmentExtended == environment::extension_result::Void) {
                return {};
            } else {
                DebugPrint("already defined", pattern, env, 2);

                auto fromEnv = *env.get(pattern.name);
                // todo: use variable instead of any? it's getting lost
                // probably no. But we just unpacked it! anyway, it's not getting sent out
                // auto value = Apply(std::move(fromEnv), any{}, env);
                auto value = Apply(std::move(fromEnv), identifier{pattern.name}, env);
                auto unapplied = Unapply(std::move(value), std::move(newVariable), env);

                // if (unapplied.outerVariable.empty())
                //     unapplied.outerVariable = pattern.name;
                unapplied.outerVariable = pattern.name;

                DebugPrint("Unapplied variable", unapplied.outerVariable, env);

                return unapplied;
            }
        },

        // something wrong with associativity in parser and this doesn't work as expected
        // [&env, &valueToMatch](rec<addition_with>&& pattern) -> unapply_result {
        //     auto wrapped = make_operation<subtraction_with>(std::move(valueToMatch), std::move(pattern.get().x));
        //     DebugPrint("moved addition", wrapped, env);
        //     return Unapply(std::move(pattern.get().function), std::move(wrapped), env);
        // },

        [&env, &valueToMatch](rec<application>&& pattern) -> unapply_result {
            auto functionCopy = pattern.get().function;
            if (auto inversed = Inverse(std::move(functionCopy), env)) {
                auto wrapped = application{std::move(*inversed), valueToMatch};
                DebugPrint("got inverse", wrapped, env);
                return Unapply(std::move(pattern.get().argument), std::move(wrapped), env);
            } else {
                auto wrapped = abstraction{copy(pattern.get().argument), copy(valueToMatch)};
                DebugPrint("moved abstraction", wrapped, env);
                auto result = Unapply(copy(pattern.get().function), std::move(wrapped), env);
                // if (result)
                //     return result;
                // auto intersected = make_operation<intersection_with>(std::move(pattern.get().function), std::move(valueToMatch));
                // auto result2 = Unapply(std::move(pattern.get().argument), std::move(intersected), env);
                // if (result2)
                //     return result2;
                return result;
                // try to create something in env if it's application?
                // or perhaps closure arg will become integer
            }
        },

        [&env, &valueToMatch](rec<then>&& e) -> unapply_result {
            auto from = Eval(std::move(e.get().from), env);
            DebugPrint("then unapply", from, env);
            return match(std::move(from),
                [](error&&) -> unapply_result { return {}; },
                [](nothing&&) -> unapply_result { return {}; },
                [&valueToMatch, &e, &env](auto&&) -> unapply_result {
                    return Unapply(std::move(e.get().to), std::move(valueToMatch), env); 
                }
            );
        },
        [&env](auto&& e) -> unapply_result { 
            DebugPrint("couldn't unapply", e, env);
            return {}; 
        }
    );
    env.decreaseDebugIndentation();
    DebugPrint("unapply result", 
                (ret ? std::string("yes") : std::string("no")) + (ret.outerVariable.empty() ? "" : (" " + ret.outerVariable)), 
                env);
    return ret;
}

}