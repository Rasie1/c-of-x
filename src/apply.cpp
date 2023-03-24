#include "apply.h"
#include "operations.h"
#include "operations_for_datatypes.h"

namespace cx {

template <template<typename datatype> typename operation_for_datatype, typename functor>
expression Calculate(expression&& l,
                     expression&& r,
                     environment& env) {
    r = Eval(std::move(r), env);
    l = Eval(std::move(l), env);
    return match(std::move(l),
        operation_for_datatype<int>{r},
        // operation_for_datatype<std::string>{r},
        [&r](identifier&& v) -> expression { return make_operation<functor>(std::move(v), std::move(r)); },
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
    auto [unapplied, outerVariable] = Unapply(std::move(pattern), std::move(argumentValue), function.env);

    auto combinedEnv = env;
    for (auto& v: function.env.variables)
        combinedEnv.variables.push_back(std::move(v));
       
    return SubstituteVariables(std::move(function.body), combinedEnv);
}

template<typename datatype>
struct check_datatype {
    environment& env;
    expression& argument;
    expression operator()(basic_type<datatype>&&) {
        auto evaluated = Eval(std::move(argument), env);
        return match(std::move(evaluated),
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
                    std::string("expecting ") + Show(basic_type<datatype>{}) + ", got " + Show(std::move(e))
                );
                return nothing{};
            }
        );
    }
};

expression Apply(expression&& function,
                 expression&& argument,
                 environment& env) {
    DebugPrint("apply function", function, env);
    DebugPrint("apply argument", argument, env);
    env.increaseDebugIndentation();
    auto ret = match(std::move(function),
        [&env, &argument](rec<closure>&& function) -> expression {
            return match(std::move(argument),
                [&env, &function](identifier&& id) -> expression {
                    auto argumentValue = *env.get(id.name);
                    argumentValue = GetElement(std::move(argumentValue), env);
                    return ApplyToClosure(env, std::move(function.get()), std::move(argumentValue));
                },
                [&env, &function](rec<application>&& app) -> expression {
                    if (auto unionWith = std::get_if<rec<union_with>>(&app.get().function)) {
                        auto l = Eval(std::move(unionWith->get().x), env);
                        auto r = Eval(std::move(app.get().argument), env);
                        auto functionCopy = function;
                        auto lApplied = Apply(std::move(function),     std::move(l), env);
                        auto rApplied = Apply(std::move(functionCopy), std::move(r), env);
                        return Union(std::move(lApplied), std::move(rApplied));
                    } else {
                        env.errors.push_back("can't apply application to closure");
                        return nothing{};
                    }
                },
                [&env, &function](auto&& argument) -> expression {
                    return ApplyToClosure(env, std::move(function.get()), std::move(argument));
                }
            );
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
        check_datatype<int>{env, argument},
        check_datatype<std::string>{env, argument},
        [&env, &argument](implication&&) -> expression { return implication_with{Eval(std::move(argument), env)}; },
        [&env, &argument](rec<implication_with>&& function) -> expression {
            return Eval(then{std::move(function.get().x), std::move(argument)}, env);
        },
        [&env, &argument](equality&&) -> expression { return equals_to{ Eval(std::move(argument), env) }; },
        [&env, &argument](inequality&&) -> expression { return negated{ equals_to{Eval(std::move(argument), env)} }; },
        [&env, &argument](rec<application>&& e) -> expression {
            return match(copy(e.get().function),
                [&](rec<intersection_with>&& lApplication) -> expression {
                    DebugPrint("matched application intersection", lApplication, env);
                    auto l = Apply(std::move(lApplication.get().x), copy(argument), env); // todo: copy envs?
                    auto r = Apply(std::move(e.get().argument), std::move(argument), env);
                    return Intersect(std::move(l), std::move(r), env);
                },
                [&](auto&&) -> expression { return application{std::move(e), std::move(argument)}; }
            );
        },
        [&env, &argument](intersection&&) -> expression { return intersection_with{Eval(std::move(argument), env)}; },
        [&env, &argument](union_&&) -> expression { return union_with{Eval(std::move(argument), env)}; },
        [&env, &argument](show&&) -> expression { return ShowSafe(std::move(argument), env); },
        [&env, &argument](print&&) -> expression { return Print(std::move(argument), env); },
        [&env, &argument](read&&) -> expression { return Read(std::move(argument), env); },
        [&env, &argument](set_trace_enabled&&) -> expression { return SetTraceEnabled(std::move(argument), env); },
        [&env, &argument](rec<equals_to>&& e) -> expression { return Equals(std::move(e.get().x), std::move(argument), env); },
        [&env, &argument](rec<negated>&& e) -> expression {
            return Apply(Negate(std::move(e.get().f), env), std::move(argument), env);
        },
        [&env, &argument](rec<intersection_with>&& function) -> expression {
            auto l = Eval(std::move(function.get().x), env);
            auto r = Eval(std::move(argument), env);
           
            return Intersect(std::move(l), std::move(r), env);
        },
        [&env, &argument](rec<union_with>&& function) -> expression {
            auto l = Eval(std::move(function.get().x), env);
            auto r = Eval(std::move(argument), env);

            // DebugPrint("union l", l, env);
            // DebugPrint("union r", r, env);

            return Union(std::move(l), std::move(r));
        },
        [&argument](identifier&& f) -> expression { return application{std::move(f), std::move(argument)}; },
        [&argument](nothing&& n) -> expression { return n; },
        [&env, &argument](auto&& e) -> expression {
            env.errors.push_back(
                std::string("can't apply ") + Show(std::move(e)) + " to " + Show(std::move(argument))
            );
            return nothing{};
        }
    );
    env.decreaseDebugIndentation();
    DebugPrint("apply result", ret, env);
   
    return ret;
}

}