#include "apply.h"
#include "operations.h"
#include "operations_for_datatypes.h"

namespace cx {

inline expression ApplyToClosure(environment& env, 
                                 closure&& function, 
                                 expression&& argumentValue, 
                                 bool prohibitFreeVariables) {
    DebugPrint("in ApplyToClosure", function, env);
    auto& pattern = function.argument;
    function.env.debugIndentation = env.debugIndentation;
    auto [unapplied, outerVariable] = Unapply(move(pattern), move(argumentValue), function.env);

    if (!unapplied) {
        env.errors.push_back(std::string("couldn't apply argument"));
        return nothing{};
    }

    auto combinedEnv = env;
    for (auto& v: function.env.variables)
        combinedEnv.variables.push_back(move(v));
    return SubstituteVariables(move(function.body), combinedEnv, prohibitFreeVariables);
}

inline expression ApplyToClosure2(environment& env, 
                                 closure&& function, 
                                 expression&& argumentValue, 
                                 bool prohibitFreeVariables) {
    DebugPrint("in ApplyToClosure2", function, env);
    auto& pattern = function.argument;
    function.env.debugIndentation = env.debugIndentation;
    auto [unapplied, outerVariable] = Unapply(copy(pattern), move(argumentValue), function.env);

    if (!unapplied)
        return function;

    auto combinedEnv = env;
    for (auto& v: function.env.variables)
        combinedEnv.variables.push_back(move(v));
    return SubstituteVariables(move(function.body), combinedEnv, prohibitFreeVariables);

    // for (auto& v: function.env.variables)
    //     env.variables.push_back(move(v));
    // return SubstituteVariables(move(function.body), env, prohibitFreeVariables);
}

template<typename datatype>
struct check_datatype {
    environment& env;
    expression& argument;
    expression operator()(basic_type<datatype>&&) {
        auto evaluated = Eval(move(argument), env);
        return match(move(evaluated),
            // todo: union
            [](datatype&& r) -> expression { return r; },
            [](any&&) -> expression { return application{basic_type<datatype>{}, any{}}; },
            [](nothing&& v) -> expression { return v; },
            [this](identifier&& v) -> expression {
                ExtendEnvironment(basic_type<datatype>{}, v, env);
                return v;
            },
            [this](auto&& e) -> expression {
                env.errors.push_back(
                    std::string("expecting ") + Show(basic_type<datatype>{}) + ", got " + Show(move(e))
                );
                return nothing{};
            }
        );
    }
};

struct check_function_datatype {
    environment& env;
    closure& type;
    expression operator()(rec<closure>&& value) {
        DebugPrint("checking function type", type, env);
        stash variables(env.variables);
        auto appliedL = Apply(move(type.argument), copy(value->argument), env);
        // this is incorrect, because these might not fail when needed. Probably should substitute too
        if (std::get_if<nothing>(&appliedL)) {
            env.errors.push_back(
                std::string("can't match function type")
            );
            return nothing{};
        }
        auto appliedR = Apply(move(type.body), copy(value->body), env);
        if (std::get_if<nothing>(&appliedR)) {
            env.errors.push_back(
                std::string("can't match function type")
            );
            return nothing{};
        }
        return value;
    }
};

expression GetSet(expression&& e, environment& env) {
    DebugPrint("getset", e, env);
    auto ret = match(move(e),
        [&env](rec<application>&& app) -> expression {
            return match(move(app->function),
                // todo: generalize and check function signatures
                [&app](read&&) -> expression { return app->argument; },
                [&app](show&&) -> expression { return basic_type<std::string>{}; },
                [&app](print&&) -> expression { return cx::unit{}; },
                [](auto&&) -> expression { return cx::any{}; }
            );
        },
        [](auto&&) -> expression { return cx::any{}; }
    );

    return ret;
}

template <template<typename datatype> typename operation_for_datatype, typename function>
expression Calculate(expression&& l,
                     expression&& r,
                     environment& env) {
    r = Eval(move(r), env);
    l = Eval(move(l), env);
    return match(move(l),
        operation_for_datatype<int>{r, env},
        // operation_for_datatype<std::string>{r},
        [&r](identifier&& v) -> expression { return make_operation<function>(move(v), move(r)); },
        map_union_l<function>{r, env, Calculate<operation_for_datatype, function>},
        [&env](auto&& e) -> expression {
            env.errors.push_back(std::string("can't do arithmetic with ") + Show(e));
            return nothing{};
        }
    );
}

template <class operation, template<typename datatype> typename operation_impl, class operation_function>
struct apply_calculation {
    environment& env;
    expression& argument;
    expression operator()(operation&&) {
        return operation_function{ Eval(move(argument), env)};
    }
    expression operator()(rec<operation_function>&& function) {
        return Calculate<operation_impl, operation_function>(move(function->x), move(argument), env);
    }
};

expression Apply(expression&& function,
                 expression&& argument,
                 environment& env) {
    DebugPrint("apply function", function, env);
    DebugPrint("apply argument", argument, env);
    env.increaseDebugIndentation();
    auto ret = match(move(function),
        [&env, &argument](rec<closure>&& function) -> expression {
            return match(move(argument),
                [&env, &function](identifier&& id) -> expression {
                    auto maybeValue = env.get(id.name);
                    if (!maybeValue) {
                        if (env.isExecuting) {
                            env.errors.push_back("unknown variable \"" + id.name + "\"");
                            return nothing{};
                        } else {
                            // halting problem here, TODO: go multiple levels down, because
                            //                             user can make it so that function will be alternating between N values
                            //                             each application
                            DebugPrint("apply - closure id eval", function, env);
                            auto evaluatedFunction = Eval(expression{move(function)}, env);
                            if (auto evaluatedClosure = std::get_if<rec<closure>>(&evaluatedFunction)) {
                                closure convertedClosure = copy(evaluatedClosure->get()); 
                                auto untouched = copy(convertedClosure);
                                auto applied = ApplyToClosure2(env, move(convertedClosure), copy(id), false);
                                if (expression{untouched} == applied) {
                                    DebugPrint("unknown variable when applying to closure, extending with", evaluatedFunction, env);
                                    ExtendEnvironment(copy(evaluatedFunction), id, env);
                                    return application{move(evaluatedFunction), move(id)};
                                } else {
                                    DebugPrint("unknown variable when applying to closure, not extending with", evaluatedFunction, env);
                                    return applied;
                                }
                            } else {
                                DebugPrint("unknown variable when applying to closure, extending with (2)", evaluatedFunction, env);
                                ExtendEnvironment(copy(evaluatedFunction), id, env);
                                return application{move(evaluatedFunction), move(id)};
                            }
                        }
                    }
                    auto argumentValue = *maybeValue;
                    argumentValue = GetElement(move(argumentValue), env);
                    // should try to do the same thing before going into ApplyToClosure, and not get caught in an endless loop
                    return Apply(move(*function), move(argumentValue), env);
                    // old:
                    // return ApplyToClosure(env, move(*function), move(argumentValue), false);
                },
                [&env, &function](rec<application>&& app) -> expression {
                    if (auto unionWith = std::get_if<rec<union_with>>(&app->function)) {
                        auto l = Eval(move(unionWith->get().x), env);
                        auto r = Eval(move(app->argument), env);

                        DebugPrint("apply closure $ application l", l, env);
                        env.increaseDebugIndentation();
                        auto lApplied = Apply(copy(function), move(l), env);
                        env.decreaseDebugIndentation();

                        DebugPrint("apply closure $ application r", r, env);
                        env.increaseDebugIndentation();
                        auto rApplied = Apply(move(function), move(r), env);
                        env.decreaseDebugIndentation();

                        return Union(move(lApplied), move(rApplied), env);
                    } else {
                        // DebugPrint("======================", app, env);
                        // extra typecheck can happen here. It tries to apply application that
                        // can't be resolved to be applied the closure - e.g. with side effects
                        // now we have to get some extra information about the set of that expr

                        // auto set = GetSet(copy(app), env);
                        // auto envForTypecheck = env;

                        if (env.isExecuting) {
                            env.errors.push_back("can't apply application to closure");
                            return nothing{};
                        } else {
                            return *app;
                        }
                    }
                },
                check_function_datatype{env, *function},
                [&env, &function](auto&& argument) -> expression {
                    return ApplyToClosure(env, move(*function), move(argument), true);
                }
            );
        },
        apply_calculation<addition, addition_for_datatype, addition_with>{env, argument},
        apply_calculation<subtraction, subtraction_for_datatype, subtraction_with>{env, argument},
        apply_calculation<multiplication, multiplication_for_datatype, multiplication_with>{env, argument},
        apply_calculation<division, division_for_datatype, division_with>{env, argument},
        check_datatype<int>{env, argument},
        check_datatype<std::string>{env, argument},
        [&env, &argument](implication&&) -> expression { return implication_with{Eval(move(argument), env)}; },
        [&env, &argument](rec<implication_with>&& function) -> expression {
            return Eval(then{move(function->x), move(argument)}, env);
        },
        [&env, &argument](equality&&) -> expression { return equals_to{ Eval(move(argument), env) }; },
        [&env, &argument](inequality&&) -> expression { return negated{ equals_to{Eval(move(argument), env)} }; },
        [&env, &argument](rec<application>&& e) -> expression {
            return match(copy(e->function),
                [&](rec<intersection_with>&& lApplication) -> expression {
                    DebugPrint("matched application intersection l", lApplication, env);
                    env.increaseDebugIndentation();
                    auto l = Apply(move(lApplication->x), copy(argument), env); // todo: copy envs?
                    env.decreaseDebugIndentation();
                    DebugPrint("matched application intersection r", e->argument, env);
                    env.increaseDebugIndentation();
                    auto r = Apply(move(e->argument), move(argument), env);
                    env.decreaseDebugIndentation();
                    return Intersect(move(l), move(r), env);
                },
                [&](rec<union_with>&& lApplication) -> expression {
                    DebugPrint("matched application intersection l", lApplication, env);
                    env.increaseDebugIndentation();
                    auto l = Apply(move(lApplication->x), copy(argument), env); // todo: copy envs?
                    env.decreaseDebugIndentation();
                    DebugPrint("matched application intersection r", e->argument, env);
                    env.increaseDebugIndentation();
                    auto r = Apply(move(e->argument), move(argument), env);
                    env.decreaseDebugIndentation();
                    return Union(move(l), move(r), env);
                },
                [&](auto&&) -> expression { return application{move(e), move(argument)}; }
            );
        },
        [&env, &argument](intersection&&) -> expression { return intersection_with{Eval(move(argument), env)}; },
        [&env, &argument](union_&&) -> expression { return union_with{Eval(move(argument), env)}; },
        [&env, &argument](show&&) -> expression { return ShowSafe(move(argument), env); },
        [&env, &argument](print&&) -> expression { return Print(move(argument), env); },
        [&env, &argument](read&&) -> expression { return Read(move(argument), env); },
        [&env, &argument](set_trace_enabled&&) -> expression { return SetTraceEnabled(move(argument), env); },
        [&env, &argument](rec<equals_to>&& e) -> expression { return Equals(move(e->x), move(argument), env); },
        [&env, &argument](rec<negated>&& e) -> expression {
            return Apply(Negate(move(e->x), env), move(argument), env);
        },
        [&env, &argument](rec<intersection_with>&& function) -> expression {
            auto l = Eval(move(function->x), env);
            auto r = Eval(move(argument), env);
           
            return Intersect(move(l), move(r), env);
        },
        [&env, &argument](rec<union_with>&& function) -> expression {
            auto l = Eval(move(function->x), env);
            auto r = Eval(move(argument), env);

            // DebugPrint("union l", l, env);
            // DebugPrint("union r", r, env);

            return Union(move(l), move(r), env);
        },
        [&argument, &env](identifier&& f) -> expression {
            auto substituted = SubstituteVariables(copy(f), env);
            if (std::get_if<identifier>(&substituted))
                return application{move(substituted), move(argument)}; 
            
            auto applied = Apply(move(substituted), copy(argument), env);
            return applied;
            // if (applied == argument && get_if<identifier>(&argument))
            // {
            //     // auto ret =  application{move(f), move(argument)}; // double application? looks super wrong
            //                                                     // also, f shouldn't be probably modifier?
            //     // DebugPrint("Returning both application parts", ret, env);

            //     // return ret;

            //     ExtendEnvironment(move(f), move(applied), env);

            //     // this should be not an env addition, but just a "check". Like "x: if (a -> a) x"
                
            //     return argument;
            // } else {
            //     DebugPrint("Returning only identifier", applied, env);
            //     return applied;
            // }
        },
        [&argument](nothing&& n) -> expression { return n; },
        [&env, &argument](auto&& e) -> expression {
            env.errors.push_back(
                std::string("can't apply ") + Show(move(e)) + " to " + Show(move(argument))
            );
            return nothing{};
        }
    );
    env.decreaseDebugIndentation();
    DebugPrint("apply result", ret, env);
   
    return ret;
}

}