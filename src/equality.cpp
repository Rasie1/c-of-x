#include "eval.h"
#include "operations.h"
#include "operations_for_datatypes.h"

namespace cx {

struct equals_with_negated {
    expression& r;
    environment& env;
    inline auto operator()(rec<negated>&& e) -> expression { 
        DebugPrint("eq with negated", e.get().f, env);
        auto falseEnv = env;
        auto [evaluatedl, lvar] = FixWithVariable(std::move(std::move(e.get().f)), falseEnv);
        auto [evaluatedr, rvar] = FixWithVariable(std::move(r), falseEnv);
        DebugPrint(std::string("eq with negated, l(") + (lvar?(*lvar):std::string("-")) + ")", evaluatedl, env);
        DebugPrint(std::string("eq with negated, r(") + (rvar?(*rvar):std::string("-")) + ")", evaluatedr, env);
        auto eq = IsEqual(copy(evaluatedl), copy(evaluatedr), falseEnv);
        DebugPrint("eq with negated, is equal", eq, env);
        return match(std::move(eq),
            [&lvar, &rvar](nothing&&) -> expression { 
                if (lvar)
                    return identifier{*lvar};
                else if (rvar)
                    return identifier{*rvar};
                else
                    return any{}; 
            },
            [](identifier&& e) -> expression { return e; },
            [this, &evaluatedl, &evaluatedr](auto&&) -> expression { 
                env.errors.push_back(Show(std::move(evaluatedl)) + " is equals to " + Show(std::move(evaluatedr)));
                return nothing{}; 
            }
        );
    }
};

template<typename datatype>
struct equals_for_datatype {
    expression& r;
    environment& env;
    expression operator()(datatype&& l) {
        return match(std::move(r),
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](identifier&& v) -> expression { return make_operation<equals_to>(l, std::move(v)); },
            map_union_r<equals_for_datatype<datatype>, datatype>{l, env},
            [&l](any&&) -> expression { return l; },
            [this, &l](auto&& r) -> expression { 
                env.errors.push_back(Show(std::move(l)) + " is not equal to " + Show(std::move(r)));
                return nothing{}; 
            }
        ); 
    }
};

expression IsEqual(expression&& l,
                   expression&& r,
                   environment& env) {
    DebugPrint("is equal 1", l, env);
    DebugPrint("is equal 2", r, env);
    auto result = match(std::move(l),
        equals_for_datatype<int>{r, env},
        equals_for_datatype<basic_type<int>>{r, env},
        equals_for_datatype<std::string>{r, env},
        equals_for_datatype<basic_type<std::string>>{r, env},
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
        map_union_l{r, env, IsEqual},
        equals_with_negated{r, env},
        [&r, &env](rec<equals_to>&& e) -> expression {
            auto element = GetElement(std::move(r), env);
            auto result = IsEqual(std::move(e.get().x), std::move(element), env);
            if (std::get_if<nothing>(&result))
                return result;
            else
                return equals_to{std::move(result)};
        },
        [&r, &env](rec<then>&& e) -> expression {
            auto from = Eval(std::move(e.get().from), env);
            DebugPrint("then in is equal", 0, env, 2);
            return match(std::move(from),
                [&env](nothing&&) -> expression {
                    env.errors.push_back("equality with nothing");
                    return nothing{}; 
                },
                [&e, &r, &env](auto&&) -> expression {
                    return IsEqual(std::move(e.get().to), std::move(r), env);
                }
            );
        },
        [&r](any&&) -> expression { return r; },
        [&r, &env](auto&& a) -> expression { 
            if (std::get_if<any>(&r))
                return a;
            env.errors.push_back(Show(std::move(a)) + " is not equal to " + Show(std::move(r)));
            return nothing{}; 
        }
    );
    DebugPrint("is equal result", result, env);

    return result;
}

expression Equals(expression&& l,
                  expression&& r,
                  environment& env) {
    DebugPrint("equality", equality{}, env);
    env.increaseDebugIndentation();
    defer { env.decreaseDebugIndentation(); };

    stash executionState(env.isExecuting, false);
    
    l = std::move(Eval(std::move(l), env)); 
    r = std::move(Eval(std::move(r), env));
    DebugPrint("eq1", l, env);
    DebugPrint("eq2", r, env);
    if (l == r)
        return r;

    auto [isUnapplySuccessful, id] = Unapply(copy(l), copy(r), env);
    if (isUnapplySuccessful) {
        DebugPrint("eq, unapply success", id, env);
        if (!id.empty()) {
            return identifier{id};
        }
        return r;
    }
    env.errors.clear();

    auto ret = IsEqual(std::move(l), std::move(r), env);
    DebugPrint("equality result", ret, env);

    return ret;
}

}