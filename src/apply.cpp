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

expression Apply(expression&& function, 
                 expression&& argument, 
                 environment& env) {
    DebugPrint("apply function", function, env);
    DebugPrint("apply argument", argument, env);
    env.increaseDebugIndentation();
    auto ret = std::visit(overload{
        [&env, &argument](rec<closure>&& function) -> expression {
            expression argumentValue;

            if (auto argumentId = std::get_if<identifier>(&argument)) {
                argumentValue = *env.get(argumentId->name);
                argumentValue = GetElement(std::move(argumentValue));
            } else {
                argumentValue = std::move(argument);
            }
            if (Unapply(std::move(function.get().argument), std::move(argumentValue), function.get().env)) {
                auto combinedEnv = env;
                for (auto& v: function.get().env.variables) {
                    combinedEnv.variables.push_back(std::move(v));
                }
                return Fix(std::move(function.get().body), combinedEnv);
            } else {
                return error{std::string("can't apply ") 
                     + Show(std::move(argumentValue)) + " to closure with signature "
                     + Show(std::move(function.get().argument))};
            }
            // unapply should be here
            // if (auto inputId = std::get_if<identifier>(&function.get().argument)) {
            //     DebugPrint("applying closure", function, env);
            //     if (auto argumentId = std::get_if<identifier>(&argument)) {
            //         expression argumentValue = *env.get(argumentId->name);
            //         function.get().env.add(inputId->name, std::move(argumentValue));
            //     } else {
            //         function.get().env.add(inputId->name, equals_to{std::move(argument)});
            //     }

            //     // auto evaluated = Eval(std::move(function.get().body), function.get().env);
            //     return Fix(std::move(function.get().body), function.get().env); // removes laziness :(

            //     // if (auto outputId = std::get_if<identifier>(&evaluated)) {
            //     //     return *function.get().env.get(outputId->name); // todo: replace all variables. Fix?
            //     // } else {
            //     //     return evaluated;
            //     // }
            // } else {
            //     return error{"pattern matching not yet implemented"};
            // }
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
            return Intersect(std::move(l), std::move(r));
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