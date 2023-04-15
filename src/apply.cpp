#include "apply.h"
#include "operations.h"
#include "operations_for_datatypes.h"

namespace cx {

template <template<typename datatype> typename operation_for_datatype, typename functor>
expression Calculate(expression&& l,
                     expression&& r,
                     environment& env) {
    r = Eval(move(r), env);
    l = Eval(move(l), env);
    return match(move(l),
        operation_for_datatype<int>{r, env},
        // operation_for_datatype<std::string>{r},
        [&r](identifier&& v) -> expression { return make_operation<functor>(move(v), move(r)); },
        map_union_l{r, env, Calculate<operation_for_datatype, functor>},
        [&env](auto&& e) -> expression {
            env.errors.push_back(std::string("can't do arithmetic with ") + Show(e));
            return nothing{};
        }
    );
}

inline expression ApplyToClosure(environment& env, closure&& function, expression&& argumentValue) {
    DebugPrint("in closure", function, env);
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
       
    return SubstituteVariables(move(function.body), combinedEnv);
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
    expression operator()(closure&& value) {
        DebugPrint("checking function type", type, env);
        auto envCopy = env;
        auto unappliedL = Unapply(move(type.argument), move(value.argument), envCopy);
        if (!unappliedL) {
            env.errors.push_back(
                std::string("can't match function type")
            );
            return nothing{};
        }
        auto unappliedR = Unapply(move(type.body), move(value.body), envCopy);
        if (!unappliedR) {
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
                            return application{move(function), move(id)};
                        }
                    }
                    auto argumentValue = *maybeValue;
                    argumentValue = GetElement(move(argumentValue), env);
                    return ApplyToClosure(env, move(*function), move(argumentValue));
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

                        return Union(move(lApplied), move(rApplied));
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
                    return ApplyToClosure(env, move(*function), move(argument));
                }
            );
        },
        [&env, &argument](addition&&) -> expression { return addition_with{Eval(move(argument), env)}; },
        [&env, &argument](rec<addition_with>&& function) -> expression {
            return Calculate<addition_for_datatype, addition_with>(move(function->x), move(argument), env);
        },
        [&env, &argument](subtraction&&) -> expression { return subtraction_with{Eval(move(argument), env)}; },
        [&env, &argument](rec<subtraction_with>&& function) -> expression {
            return Calculate<subtraction_for_datatype, subtraction_with>(move(function->x), move(argument), env);
        },
        [&env, &argument](multiplication&&) -> expression { return multiplication_with{Eval(move(argument), env)}; },
        [&env, &argument](rec<multiplication_with>&& function) -> expression {
            return Calculate<multiplication_for_datatype, multiplication_with>(move(function->x), move(argument), env);
        },
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
            return Apply(Negate(move(e->f), env), move(argument), env);
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

            return Union(move(l), move(r));
        },
        [&argument, &env](identifier&& f) -> expression { 
            auto substituted = SubstituteVariables(move(f), env);
            if (std::get_if<identifier>(&substituted))
                return application{move(substituted), move(argument)}; 
            
            return Apply(move(substituted), move(argument), env);
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