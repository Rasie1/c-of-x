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
        auto [evaluatedl, lvar] = FixWithVariable(move(move(e.get().f)), falseEnv);
        auto [evaluatedr, rvar] = FixWithVariable(move(r), falseEnv);
        DebugPrint(std::string("eq with negated, l(") + (lvar?(*lvar):std::string("-")) + ")", evaluatedl, env);
        DebugPrint(std::string("eq with negated, r(") + (rvar?(*rvar):std::string("-")) + ")", evaluatedr, env);
        auto eq = IsEqual(copy(evaluatedl), copy(evaluatedr), falseEnv);
        DebugPrint("eq with negated, is equal", eq, env);
        return match(move(eq),
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
                env.errors.push_back(Show(move(evaluatedl)) + " is equals to " + Show(move(evaluatedr)));
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
        return match(move(r),
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](identifier&& v) -> expression { return make_operation<equals_to>(l, move(v)); },
            map_union_r<equals_for_datatype<datatype>, datatype>{l, env},
            [&l](any&&) -> expression { return l; },
            [this, &l](auto&& r) -> expression { 
                env.errors.push_back(Show(move(l)) + " is not equal to " + Show(move(r)));
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
    auto result = match(move(l),
        equals_for_datatype<int>{r, env},
        equals_for_datatype<basic_type<int>>{r, env},
        equals_for_datatype<std::string>{r, env},
        equals_for_datatype<basic_type<std::string>>{r, env},
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(move(v), move(r)); },
        // map_union_l{r, env, IsEqual},
        [&r, &env](rec<application>&& lApplication) -> expression {
            auto mapped = map_union_l{r, env, IsEqual}.operator()<false>(move(lApplication.get()));
            return mapped;
        },
        equals_with_negated{r, env},
        [&r, &env](rec<equals_to>&& e) -> expression {
            auto element = GetElement(move(r), env);
            auto result = IsEqual(move(e.get().x), move(element), env);
            if (std::get_if<nothing>(&result))
                return result;
            else
                return equals_to{move(result)};
        },
        [&r, &env](rec<then>&& e) -> expression {
            auto from = Eval(move(e.get().from), env);
            DebugPrint("then in is equal", 0, env, 2);
            return match(move(from),
                [&env](nothing&&) -> expression {
                    env.errors.push_back("equality with nothing");
                    return nothing{}; 
                },
                [&e, &r, &env](auto&&) -> expression {
                    return IsEqual(move(e.get().to), move(r), env);
                }
            );
        },
        [&r](any&&) -> expression { return r; },
        [&r, &env](auto&& a) -> expression { 
            if (std::get_if<any>(&r))
                return a;
            env.errors.push_back(Show(move(a)) + " is not equal to " + Show(move(r)));
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
    
    l = move(Eval(move(l), env)); 
    r = move(Eval(move(r), env));
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

    auto ret = IsEqual(move(l), move(r), env);
    DebugPrint("equality result", ret, env);

    return ret;
}

}