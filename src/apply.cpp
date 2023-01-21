#include "apply.h"
#include "operations.h"
#include "operations_for_datatypes.h"

namespace cx {

template <template<typename datatype> typename operation_for_datatype, typename functor>
expression Calculate(expression&& l,
                     expression&& r,
                     environment& env) {
    r = Eval(std::move(r), env);
    return std::visit(overload{
        operation_for_datatype<int>{r},
        // operation_for_datatype<std::string>{r},
        [&r](identifier&& v) -> expression { return make_operation<functor>(std::move(v), std::move(r)); },
        map_union_l{r, env, Calculate<operation_for_datatype, functor>},
        [](auto&& e) -> expression { return error{std::string("can't do arithmetic with ") + Show(e)}; }
    }, Eval(std::move(l), env));
}

inline expression ApplyToClosure(environment& env, closure&& function, expression&& argumentValue) {
    // doesn't shadow env. Should remove old variable

    // auto envCopy = env;
    DebugPrint("in closure", function, env);
    auto& pattern = function.argument;
    // auto [fixed, variable] = FixWithVariable(std::move(pattern), env);
    // if (variable)
    //     pattern = identifier{*variable};
    // else
    //     pattern = std::move(function.argument);

    // a bug is here. It unapplies it and a variable is already defined
    // could be fixed by searching for this specific variable or by transfering info instead of bool
    auto [unapplied, conflictingVariable] = Unapply(std::move(pattern), std::move(argumentValue), function.env);

    if (unapplied) {
        auto combinedEnv = env;
        for (auto& v: function.env.variables) {
            combinedEnv.variables.push_back(std::move(v));
        }
        return Fix(std::move(function.body), combinedEnv);
    } else if (!conflictingVariable.empty()) {
        auto combinedEnv = env;
        for (auto& v: function.env.variables) {
            if (v.first == conflictingVariable)
                continue;
            combinedEnv.variables.push_back(std::move(v));
        }
        return Fix(std::move(function.body), combinedEnv);
    } else {
        // use after move? then why does it work?
        return error{std::string("can't apply ") 
                + Show(std::move(argumentValue)) + " to closure with signature "
                + Show(std::move(pattern))};
    }
}

expression Apply(expression&& function, 
                 expression&& argument, 
                 environment& env) {
    DebugPrint("apply function", function, env);
    DebugPrint("apply argument", argument, env);
    env.increaseDebugIndentation();
    auto ret = std::visit(overload{
        [&env, &argument](rec<closure>&& function) -> expression {
            return std::visit(overload{
                [&env, &function](identifier&& id) -> expression {
                    auto argumentValue = *env.get(id.name);
                    argumentValue = GetElement(std::move(argumentValue));
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
                        return error{std::string("can't apply application to closure")};
                    }
                },
                [&env, &function](auto&& argument) -> expression {
                    return ApplyToClosure(env, std::move(function.get()), std::move(argument));
                }
            }, std::move(argument));
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
        [&env, &argument](implication&&) -> expression { return implication_with{Eval(std::move(argument), env)}; },
        [&env, &argument](rec<implication_with>&& function) -> expression {
            return Eval(then{std::move(function.get().x), std::move(argument)}, env);
        },
        [&env, &argument](equality&&) -> expression { return equals_to{Eval(std::move(argument), env)}; },
        [&env, &argument](inequality&&) -> expression { return negated{
                equals_to{Eval(std::move(argument), env)}
            }; 
        },
        [&env, &argument](rec<application>&& e) -> expression { return application{std::move(e), std::move(argument)}; },
            // todo: isn't it evaluated already?
        [&env, &argument](intersection&&) -> expression { return intersection_with{Eval(std::move(argument), env)}; },
        [&env, &argument](union_&&) -> expression { return union_with{Eval(std::move(argument), env)}; },
        [&env, &argument](show&&) -> expression { return Show(Fix(std::move(argument), env)); },
        [&env, &argument](print&&) -> expression { return Print(std::move(argument), env); },
        [&env, &argument](set_trace_enabled&&) -> expression { return SetTraceEnabled(std::move(argument), env); },
        [&env, &argument](rec<equals_to>&& e) -> expression { return Equals(std::move(e.get().x), std::move(argument), env); },
        [&env, &argument](rec<negated>&& e) -> expression { 
            return Apply(Negate(std::move(e.get().f), env), std::move(argument), env); 
        },
        [&env, &argument](rec<intersection_with>&& function) -> expression {
            // todo: isn't it evaluated already?
            auto l = Eval(std::move(function.get().x), env);
            auto r = Eval(std::move(argument), env);
            return Intersect(std::move(l), std::move(r), env);
        },
        [&env, &argument](rec<union_with>&& function) -> expression {
            // todo: isn't it evaluated already?
            auto l = Eval(std::move(function.get().x), env);
            auto r = Eval(std::move(argument), env);
            return Union(std::move(l), std::move(r));
        },
        [&argument](identifier&& f) -> expression { return application{std::move(f), std::move(argument)}; },
        [&argument](auto&& e) -> expression {
            return error{"can't apply " + Show(std::move(e)) + " to " + Show(std::move(argument)) };
        }
    }, std::move(function));
    env.decreaseDebugIndentation();
    DebugPrint("apply result", ret, env);
    
    return ret;
}

}