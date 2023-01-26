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
        return std::visit(overload{
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
        }, std::move(eq));
    }
};

expression IsEqual(expression&& l,
                   expression&& r,
                   environment& env) {
    DebugPrint("is equal 1", l, env);
    DebugPrint("is equal 2", r, env);
    return std::visit(overload{
        equals_for_datatype<int>{r},
        equals_for_datatype<std::string>{r},
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
        map_union_l{r, env, IsEqual},
        equals_with_negated{r, env},
        [](auto&&) -> expression { return nothing{}; }
    }, std::move(l));
}

expression Equals(expression&& l,
                  expression&& r,
                  environment& env) {
    DebugPrint("equality", equality{}, env);
    env.increaseDebugIndentation();
    l = std::move(Eval(std::move(l), env)); 
    r = std::move(Eval(std::move(r), env));
    DebugPrint("eq1", l, env);
    DebugPrint("eq2", r, env);
    if (l == r)
        return r;
    expression lCopy = l;
    expression rCopy = r;
    auto [isUnapplySuccessful, id] = Unapply(std::move(l), std::move(r), env);
    if (isUnapplySuccessful) {
        if (!id.empty())
            return identifier{id};
        return rCopy;
    }

    auto ret = IsEqual(std::move(lCopy), std::move(rCopy), env);

    env.decreaseDebugIndentation();
    DebugPrint("equality result", ret, env);
    return ret;

    // probably should also use what's below instead of just returning rCopy or nothing

    // expression lCopy = l;
    // expression rCopy = r;
    // if (auto added = ExtendEnvironment(equals_to{std::move(l)}, r, env)) {
    //     ExtendEnvironment(equals_to{std::move(r)}, lCopy, env);
    //     return identifier{*added};
    // } else if (auto added = ExtendEnvironment(equals_to{std::move(r)}, lCopy, env)) {
    //     return identifier{*added};
    // }
    // l = std::move(Eval(std::move(lCopy), env)); 
    // r = std::move(Eval(std::move(rCopy), env));

    // DebugPrint("eq1", l, env);
    // DebugPrint("eq2", r, env);
    // if (l == r)
    //     return r; // TODO: is that really needed?
    // return std::visit(overload{
    //     equals_for_datatype<int>{r},
    //     equals_for_datatype<std::string>{r},
    //     [&r](identifier&& v) -> expression { return make_operation<intersection_with>(std::move(v), std::move(r)); },
    //     [&r](rec<application>&& e) -> expression { return make_operation<equals_to>(std::move(e), std::move(r)); },
    //     [](auto&&) -> expression { return nothing{}; }
    // }, std::move(l));
}

}