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
        auto eq = IsEqual(std::move(evaluatedl), std::move(evaluatedr), falseEnv);
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
            [](auto&&) -> expression { return nothing{}; }
        );
    }
};

expression IsEqual(expression&& l,
                   expression&& r,
                   environment& env) {
    DebugPrint("is equal 1", l, env);
    DebugPrint("is equal 2", r, env);
    auto result = match(std::move(l),
        equals_for_datatype<int>{r},
        equals_for_datatype<basic_type<int>>{r},
        equals_for_datatype<std::string>{r},
        equals_for_datatype<basic_type<std::string>>{r},
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
        map_union_l{r, env, IsEqual},
        // [&r, &env](rec<application>&& lApplication) -> expression {
        //     auto rCopy = r;
        //     auto lCopy = lApplication.get();
        //     auto mapped = map_union_l{rCopy, env, IsEqual}(std::move(lApplication.get()));
        //     if (mapped == expression{lCopy})
        //         return nothing{};
        //     return mapped;
        // },
        equals_with_negated{r, env},
        [&r](any&&) -> expression { return r; },
        [&r](auto&& a) -> expression { 
            if (std::get_if<any>(&r))
                return a;

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

    auto ret = IsEqual(std::move(l), std::move(r), env);
    DebugPrint("equality result", ret, env);
    return ret;
}

}