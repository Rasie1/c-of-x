#include "unapply.h"
#include "operations.h"

namespace cx {

inline bool IsError(expression& e) {
    return std::visit(overload{
        [](nothing&&) { return true; },
        [](error&&) { return true; },
        [](auto&&) { return false; }
    }, std::move(e));
}

template<typename datatype>
struct unapply_for_datatype {
    expression& match;
    environment& env;
    unapply_result operator()(datatype&& pattern) {
        return std::visit(overload{
            [&pattern](datatype&& match) -> unapply_result { return {pattern == match, {}}; },
            [&pattern, this](identifier&& match) -> unapply_result { 
                auto defined = ExtendEnvironment(equals_to{std::move(match)}, std::move(pattern), env);
                if (defined)
                    return {true, {}};
                else
                    return {false, match.name};
            },
            [](auto&&) -> unapply_result { return {}; }
        }, std::move(match)); 
    }

    unapply_result operator()(basic_type<datatype>&& pattern) {
        DebugPrint("typechecking", pattern, env);
        DebugPrint("typechecking value", match, env);
        return std::visit(overload{
            [&pattern, this](rec<abstraction>&& e) -> unapply_result { 
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
        }, std::move(match)); 
    }
};

std::optional<expression> Inverse(expression&& e, environment& env) {
    DebugPrint("inverting", e, env);
    env.increaseDebugIndentation();
    auto result = std::visit(overload{
        // the case with strings is a bit different, but if I express subtraction
        // correctly, there can be the same code for both integers addition and
        // strings matching
        [](rec<addition_with>&& f) -> std::optional<expression> { 
            return subtraction_with{std::move(f.get().x)}; 
            // return abstraction{
                
            // };
        },
        [&env](rec<application>&& app) -> std::optional<expression> {
            DebugPrint("inverting application", app.get(), env);
            return std::visit(overload{
                [&app, &env](rec<union_with>&& lUnion) -> std::optional<expression> {
                    auto leftInverse = Inverse(std::move(lUnion.get()), env);
                    // if (!leftInverse)
                    //     return std::nullopt;
                    auto rightInverse = Inverse(std::move(app.get().argument), env);
                    // if (!rightInverse)
                    //     return std::nullopt;
                    if (leftInverse && rightInverse)
                        return application{union_with{*leftInverse}, *rightInverse};
                    else if (leftInverse)
                        return leftInverse;
                    else
                        return rightInverse;
                },
                [](auto&&) -> std::optional<expression> { return std::nullopt; }
            }, std::move(app.get().function));
        },
        [](int&& x) -> std::optional<expression> { return x; },
        [](std::string&& x) -> std::optional<expression> { return x; },
        // [](rec<union_with>&& x) -> std::optional<expression> { return x; },
        [](auto&&) -> std::optional<expression> { return std::nullopt; }
    }, std::move(e));
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
//         return std::visit(overload{
//             [](nothing&&) -> unapply_result { return {true, {}}; },
//             [](identifier&&) -> unapply_result { return {true, {}}; },
//             [](auto&&) -> unapply_result { return {}; }
//         }, std::move(eq));
//     }
// };


struct map_unapply_union_l {
    expression& r;
    environment& env;
    auto operator()(rec<application>&& lApplication) -> unapply_result {
        auto& rUnion = lApplication.get().argument;
        return std::visit(overload{
            [this, &rUnion](rec<union_with>&& lUnion) -> unapply_result {
                auto rCopy = r;
                auto lCalculated = Unapply(
                    std::move(lUnion.get().x), std::move(r), env);
                auto rCalculated = Unapply(
                    std::move(rUnion), std::move(rCopy), env);
                return {lCalculated.success || rCalculated.success, 
                        lCalculated.conflictingVariable.empty() ? rCalculated.conflictingVariable : lCalculated.conflictingVariable 
                };
            },
            [](auto&&) -> unapply_result { 
                return {};
            }
        }, std::move(lApplication.get().function));
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
        return std::visit(overload{
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
        }, std::move(eq));
    }
};

unapply_result Unapply(expression&& pattern, 
                       expression&& match, 
                       environment& env) {
    DebugPrint("unapply1", pattern, env);
    DebugPrint("unapply2", match, env);
    env.increaseDebugIndentation();
    auto ret = std::visit(overload{
        unapply_for_datatype<int>{match, env},
        unapply_for_datatype<std::string>{match, env},
        [](any&&) -> unapply_result { return {true, {}}; },
        equals_with_negated{match, env},
        [&env, &match](identifier&& pattern) -> unapply_result {
            auto newEnv = env;
            auto oldEvaluated = Eval(std::move(match), newEnv);
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
            auto newVariableCopy = newVariable;
            if (env.define(pattern.name, equals_to{std::move(newVariable)}))
                return {true, {}};
            else {
                DebugPrint("already defined", pattern, env, 2);

                auto fromEnv = *env.get(pattern.name);
                // todo: use variable instead of any? it's getting lost
                // probably no. But we just unpacked it! anyway, it's not getting sent out
                // auto value = Apply(std::move(fromEnv), any{}, env);
                auto value = Apply(std::move(fromEnv), identifier{pattern.name}, env);
                auto unapplied = Unapply(std::move(value), std::move(newVariableCopy), env);

                // if (unapplied.conflictingVariable.empty())
                //     unapplied.conflictingVariable = pattern.name;
                unapplied.conflictingVariable = pattern.name;

                DebugPrint("Unapplied variable", unapplied.conflictingVariable, env);

                return unapplied;
            }
        },

        // something wrong with associativity in parser and this doesn't work as expected
        // [&env, &match](rec<addition_with>&& pattern) -> unapply_result {
        //     auto wrapped = make_operation<subtraction_with>(std::move(match), std::move(pattern.get().x));
        //     DebugPrint("moved addition", wrapped, env);
        //     return Unapply(std::move(pattern.get().function), std::move(wrapped), env);
        // },

        [&env, &match](rec<application>&& pattern) -> unapply_result {
            // isn't it a mix of two concepts in one? "constrainted definitions" and "curried function definitions"
            // or this conditional is enough to distinct between them?
            // also, destructuring should go here as well... just remove part if it's the same as in match?
            auto functionCopy = pattern.get().function;
            if (auto inversed = Inverse(std::move(functionCopy), env)) {
                auto wrapped = application{std::move(*inversed), match};
                DebugPrint("got inverse", wrapped, env);
                return Unapply(std::move(pattern.get().argument), std::move(wrapped), env);
            } else {
                auto wrapped = abstraction{std::move(pattern.get().argument), std::move(match)};
                DebugPrint("moved abstraction", wrapped, env);
                return Unapply(std::move(pattern.get().function), std::move(wrapped), env);
            }
        },
        [&env](auto&& e) -> unapply_result { 
            DebugPrint("couldn't unapply", e, env);
            return {}; 
        }
    }, std::move(pattern));
    env.decreaseDebugIndentation();
    DebugPrint("unapply result", ret ? std::string("yes") : std::string("no"), env);
    return ret;
}

}