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
};

std::optional<expression> Inverse(expression&& e) {
    // can't fix because we still need variables
    return std::visit(overload{
        // the case with strings is a bit different, but if I express subtraction
        // correctly, there can be the same code for both integers addition and
        // strings matching
        [](rec<addition_with>&& f) -> std::optional<expression> { 
            return subtraction_with{std::move(f.get().x)}; 
            // return abstraction{
                
            // };
        },
        [](rec<application>&& app) -> std::optional<expression> {
            return std::visit(overload{
                [&app](rec<union_with>&& lUnion) -> std::optional<expression> {
                    auto leftInverse = Inverse(std::move(lUnion.get()));
                    // if (!leftInverse)
                    //     return std::nullopt;
                    auto rightInverse = Inverse(std::move(app.get().argument));
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
        [](auto&&) -> std::optional<expression> { return std::nullopt; }
    }, std::move(e));
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
            auto evaluated = Fix(std::move(oldEvaluated), newEnv); // danger
            if (IsError(evaluated))
                return {};
            expression newVariable;
            if (oldCopy == evaluated)
                newVariable = std::move(evaluated);
            else
                newVariable = make_operation<intersection_with>(std::move(oldCopy), std::move(evaluated));
            DebugPrint(std::string("defining variable ") + pattern.name, newVariable, env, 2);
            if (env.define(pattern.name, equals_to{std::move(newVariable)}))
                return {true, {}};
            else {
                DebugPrint("already defined", pattern, env, 2);
                return {false, pattern.name};
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
            DebugPrint("unapplying application", pattern.get(), env);
            auto functionCopy = pattern.get().function;
            if (auto inversed = Inverse(std::move(functionCopy))) {
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