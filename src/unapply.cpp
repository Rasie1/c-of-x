    #include "unapply.h"
#include "operations.h"

namespace cx {

inline bool IsError(expression& e) {
    return match(move(e),
        [](nothing&&) { return true; },
        [](auto&&) { return false; }
    );
}

template<typename datatype>
struct unapply_for_datatype {
    expression& valueToMatch;
    environment& env;
    unapply_result operator()(datatype&& pattern) {
        return match(move(valueToMatch),
            [&pattern](datatype&& v) -> unapply_result { return {pattern == v, {}}; },
            [&pattern, this](identifier&& valueToMatch) -> unapply_result {
                auto added = env.add(valueToMatch.name, equals_to{move(pattern)});
                return {added != environment::extension_result::Void, valueToMatch.name};
            },
            // [](any&&) -> unapply_result { return {true, {}}; },
            [](auto&&) -> unapply_result { return {}; }
        ); 
    }

    unapply_result operator()(basic_type<datatype>&& pattern) {
        DebugPrint("typechecking", pattern, env);
        DebugPrint("typechecking value", valueToMatch, env);
        return match(move(valueToMatch),
            [&pattern](basic_type<datatype>&& valueToMatch) -> unapply_result { return {pattern == valueToMatch, {}}; },
            [&pattern, this](identifier&& valueToMatch) -> unapply_result {
                DebugPrint("type and identifier", pattern, env);
                auto added = env.add(valueToMatch.name, equals_to{move(pattern)});
                return {added != environment::extension_result::Void, valueToMatch.name};
            },
            [&pattern, this](rec<abstraction>&& function) -> unapply_result { // shouldn't this apply to above as well?
                DebugPrint("apply in {datatype}", pattern, env);
                env.increaseDebugIndentation();
                auto applied = Apply(move(pattern), copy(function->body), env);
                env.decreaseDebugIndentation();
                DebugPrint("typechecking apply result", applied, env);
                env.increaseDebugIndentation();
                defer { env.decreaseDebugIndentation(); };
                if (applied == function->body)
                    return Unapply(move(function->argument), move(applied), env);
                else 
                    return {};
            },
            [&pattern, this](rec<closure>&& function) -> unapply_result { 
                // TODO: something could wrong with env. Should they be merged?
                auto applied = Apply(move(pattern), copy(function->body), env);
                DebugPrint("typechecking apply result", applied, env);
                if (applied == function->body)
                    return Unapply(move(function->argument), move(applied), env);
                else 
                    return {};
            },
            [&pattern, this](rec<equals_to>&& e) -> unapply_result { 
                auto newPattern = application{move(pattern), any{}};
                return Unapply(move(newPattern), move(e->x), env);
            },
            // [](any&&) -> unapply_result { return {true, {}}; },
            [](auto&&) -> unapply_result { return {}; }
        ); 
    }
};

std::optional<expression> Inverse(expression&& e, environment& env) {
    DebugPrint("inverting", e, env);
    env.increaseDebugIndentation();
    auto result = match(move(e),
        [](rec<addition_with>&& f) -> std::optional<expression> { 
            // return subtraction_with{move(f->x)}; 
            // todo: typed
            return addition_with{make_operation<multiplication_with>(-1, move(f->x))}; 
        },
        [](rec<subtraction_with>&& f) -> std::optional<expression> { 
            // return addition_with{move(f->x)}; 
            return subtraction_with{make_operation<multiplication_with>(-1, move(f->x))}; 
        },
        [&env](rec<application>&& app) -> std::optional<expression> {
            DebugPrint("inverting application", *app, env);
            return match(move(move(app->function)),
                [&app, &env](rec<union_with>&& lUnion) -> std::optional<expression> {
                    auto leftInverse = Inverse(move(*lUnion), env);
                    auto rightInverse = Inverse(move(app->argument), env);
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
//         DebugPrint("is equal with negated", e->f, env);
//         auto falseEnv = env;
//         auto eq = IsEqual(move(r), move(e->f), falseEnv);
//         DebugPrint("is equal result", eq, env);
//         return match(move(eq),
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
        auto& rUnion = lApplication->argument;
        return match(move(move(lApplication->function)),
            [this, &rUnion](rec<union_with>&& lUnion) -> unapply_result {
                auto lCalculated = Unapply(
                    move(lUnion->x), copy(r), env);
                auto rCalculated = Unapply(
                    move(rUnion), move(r), env);
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
        DebugPrint("is equal with negated", e->f, env);
        auto falseEnv = env;
        auto [evaluatedl, lvar] = FixWithVariable(move(e->f), falseEnv);
        auto [evaluatedr, rvar] = FixWithVariable(move(r), falseEnv);
        DebugPrint(std::string("unapply ne, l(") + (lvar?(*lvar):std::string("-")) + ")", evaluatedl, env);
        DebugPrint(std::string("unapply ne, r(") + (rvar?(*rvar):std::string("-")) + ")", evaluatedr, env);
        auto eq = IsEqual(move(evaluatedl), move(evaluatedr), falseEnv);
        DebugPrint("unapply ne, is equal", eq, env);
        return match(move(eq),
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
            // [](identifier&& e) -> unapply_result { return negated{move(e)}; },
            [](identifier&& e) -> unapply_result { return {false, {e.name}}; },
            [](auto&&) -> unapply_result { return {}; }
        );
    }
};

inline auto flipCommutativeOperation(application&& app, environment& env) -> std::optional<application> {
    DebugPrint("trying to flip", app, env);
    return match(move(app.function),
        [&argument = app.argument](rec<addition_with>&& f) -> std::optional<application> {
            return make_operation<addition_with>(move(argument), move(f->x));
        },
        [&argument = app.argument](rec<subtraction_with>&& f) -> std::optional<application> {
            return make_operation<multiplication_with>(
                -1,
                make_operation<subtraction_with>(move(argument), move(f->x))
            );
        },
        [](auto&&) -> std::optional<application> { return {}; }
    );
}

inline cx::expression toUnion(std::vector<expression>& variants) {
    if (variants.size() == 0)
        return nothing{};

    if (variants.size() == 1)
        return variants[0];

    expression ret;

    for (auto it = ++variants.begin(); it != variants.end(); ++it) {
        ret = application{union_with{move(ret)}, move(*it)};
    }

    return ret;
}

bool combineResults(unapply_result& l, const unapply_result& r) {
    l.success |= r.success;
    if (l.outerVariable.empty()) {
        if (r.outerVariable.empty())
            return false;

        l.outerVariable = r.outerVariable;
    }

    return true;
}

unapply_result Unapply(expression&& pattern, 
                       expression&& valueToMatch, 
                       environment& env) {
    DebugPrint("unapply1", pattern, env);
    DebugPrint("unapply2", valueToMatch, env);
    env.increaseDebugIndentation();
    auto ret = match(move(pattern),
        unapply_for_datatype<int>{valueToMatch, env},
        unapply_for_datatype<std::string>{valueToMatch, env},
        [](any&&) -> unapply_result { return {true, {}}; },
        equals_with_negated{valueToMatch, env},
        [&env, &valueToMatch](identifier&& pattern) -> unapply_result {
            if (expression{pattern} == valueToMatch)
                return {true, pattern.name};

            auto newEnv = env;
            auto oldEvaluated = Eval(move(valueToMatch), newEnv);
            auto evaluated = SubstituteVariables(copy(oldEvaluated), newEnv); // danger
            if (IsError(evaluated))
                return {};
            if (std::get_if<any>(&evaluated)) {
                return {true, ""};
            }
            expression newVariable;
            if (oldEvaluated == evaluated)
                newVariable = move(evaluated);
            else
                newVariable = make_operation<intersection_with>(move(oldEvaluated), move(evaluated));
            DebugPrint(std::string("defining variable ") + pattern.name, newVariable, env, 2);
            // define only tries to add new data and it works
            // add also adds checks about all calculations
            // but these should be divided from predicates and not participate in env get
            auto isEnvironmentExtended = env.add(pattern.name, equals_to{copy(newVariable)});
            if (isEnvironmentExtended == environment::extension_result::Added) {
                return {true, pattern.name};
            } else if (isEnvironmentExtended == environment::extension_result::Match) {
                return {true, ""}; // don't return variable because it's possible that it's not
                                   // an identity transformation in equality

                                   // or return inner variable? Yes, should try that!
            } else if (isEnvironmentExtended == environment::extension_result::Void) {
                return {};
            } else {
                DebugPrint("already defined", pattern, env, 2);

                auto fromEnv = *env.get(pattern.name);
                auto value = Apply(move(fromEnv), identifier{pattern.name}, env);
                auto unapplied = Unapply(move(value), move(newVariable), env);

                unapplied.outerVariable = pattern.name;

                DebugPrint("unapplied variable", unapplied.outerVariable, env);

                return unapplied;
            }
        },

        // new stuff
        [&env, &valueToMatch](rec<equals_to>&& pattern) -> unapply_result {
            auto element = GetElement(move(valueToMatch), env);
            auto intersection = Unapply(move(pattern->x), move(element), env);
            return intersection;
            // return Unapply(move(intersection), move(element), env);
            // if (std::get_if<identifier>(&intersection))
            //     return {true, {}};
            // else 
            //     return {};
        },

        // something wrong with associativity in parser and this doesn't work as expected
        // [&env, &valueToMatch](rec<addition_with>&& pattern) -> unapply_result {
        //     auto wrapped = make_operation<subtraction_with>(move(valueToMatch), move(pattern->x));
        //     DebugPrint("moved addition", wrapped, env);
        //     return Unapply(move(pattern->function), move(wrapped), env);
        // },

        [&env, &valueToMatch](rec<application>&& pattern) -> unapply_result {
            unapply_result result = {};

            if (auto inversed = Inverse(copy(pattern->function), env)) {
                auto wrapped = application{move(*inversed), valueToMatch};
                DebugPrint("got l inverse", wrapped, env);
                if (combineResults(result, Unapply(move(pattern->argument), move(wrapped), env)))
                    return result;
            }
            if (auto flipped = flipCommutativeOperation(copy(*pattern), env)) {
                // auto wrapped = application{move(*flipped), valueToMatch};
                DebugPrint("got r inverse", *flipped, env);
                if (combineResults(result, Unapply(move(*flipped), move(valueToMatch), env)))
                    return result;
            }
            {
                auto wrapped = abstraction{copy(pattern->argument), copy(valueToMatch)};
                DebugPrint("moved abstraction", wrapped, env);
                combineResults(result, Unapply(move(pattern->function), move(wrapped), env));
            }

            return result;

            // if (result.success) {
            //     env.errors.clear();
            // }
        },
        
        [&env, &valueToMatch](rec<closure>&& function) -> unapply_result {
            DebugPrint("unapply closure", valueToMatch, env);
            env.increaseDebugIndentation();

            function->env.debugIndentation = env.debugIndentation;

            auto lArgument = SubstituteVariables(move(function->argument), function->env);

            if (std::get_if<nothing>(&lArgument)) {
                DebugPrint("incorrect function argument", lArgument, env);
                return {}; 
            }

            auto l = SubstituteVariables(move(function->body), function->env);

            // todo: what if the output is an id?

            auto envCopy = env;

            auto rFunction = SubstituteVariables(copy(valueToMatch), envCopy);

            env.decreaseDebugIndentation();
            DebugPrint("unapply closure - apply", valueToMatch, envCopy);
            env.increaseDebugIndentation();

            auto body = Apply(move(rFunction), move(l), envCopy);

            if (std::get_if<nothing>(&body)) {
                DebugPrint("couldn't intersect closure with", valueToMatch, env);
                return {}; 
            }

            auto rElement = GetElement(move(valueToMatch), envCopy);
            rElement = Eval(move(rElement), envCopy);

            env.decreaseDebugIndentation();
            DebugPrint("unapply closure - uapply", rElement, env);
            env.increaseDebugIndentation();
            defer { env.decreaseDebugIndentation(); };
            // auto argument = Unapply(move(function->argument), move(rElement), env);
            return Unapply(move(lArgument), move(rElement), envCopy);

            // if (std::get_if<nothing>(&argument)) {
            //     DebugPrint("couldn't intersect closure", body, env);
            //     return {}; 
            // }

            // return {true, ""};

        //     if (auto rightClosure = std::get_if<rec<closure>>(&set)) {
        //         auto unappliedArgument = Unapply(move(function->argument), 
        //                                          move(rightClosure->get().argument),
        //                                          env);
        //         if (!unappliedArgument.success) {
        //             return {}; // error?
        //         }
        //     }
        //     return Unapply(move(evaluated), GetElement(move(valueToMatch), env), env);

        //     // return match(move(set),
        //     //     [](rec<closure>&& e) { 
        //     //         return e->x; 
        //     //     },
        //     //     [&env](auto&& e) -> expression { 
        //     //         return GetElement(move(e), env); 
        //     //     }
        //     // );
        }, //*/
        [&env, &valueToMatch](rec<then>&& e) -> unapply_result {
            auto from = Eval(move(e->from), env);
            DebugPrint("then unapply", from, env);
            return match(move(from),
                [](nothing&&) -> unapply_result { return {}; },
                [&valueToMatch, &e, &env](auto&&) -> unapply_result {
                    return Unapply(move(e->to), move(valueToMatch), env); 
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
                (ret ? std::string("yes") : std::string("no")) + 
                (ret.outerVariable.empty() ? "" : (", variable: " + ret.outerVariable)), 
                env);
    return ret;
}

}