#include "operations.h"
#include "robin_hood.h"
#include "functor.h"
#include "operations_for_datatypes.h"
#include "unapply.h"

namespace cx {

template<typename datatype>
struct intersect_for_datatype {
    expression& r;
    environment& env;
    expression operator()(datatype&& l) {
        return match(move(r),
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](any&&) -> expression { return l; },
            [&l](identifier&& v) -> expression { return make_operation<intersection_with>(l, move(v)); },
            [this, &l](rec<application>&& rApplication) -> expression { 
                auto mapped = map_union_r<intersect_for_datatype<datatype>, datatype>{l, env}.template operator()<true>(move(rApplication));
                return mapped;
            },
            [this, &l](auto&& r) -> expression { 
                env.errors.push_back("can't intersect " + Show(move(l)) + " with " + Show(move(r)));
                return nothing{}; 
            }
        );
    }
};

expression GetElement(expression&& set, environment& env) {
    DebugPrint("get element", set, env);
    set = Eval(move(set), env);
    return match(move(set),
        [](rec<equals_to>&& e) { return e->x; },
        [&env](int&& e) -> expression {
            env.errors.push_back(Show(move(e)) + " doesn't contain values");
            return nothing{};
        },
        [&env](std::string&& e) -> expression {
            env.errors.push_back(Show(move(e)) + " doesn't contain values");
            return nothing{};
        },
        [](auto&& e) -> expression { return application{move(e), any{}}; }
    );
}

expression Negate(expression&& f, environment& env) {
    DebugPrint("negating", f, env, 2);
    auto evaluated = Eval(move(f), env);
    return match(move(evaluated),
        // [](int&&) -> expression { return nothing{}; },
        // [](std::string&&) -> expression { return nothing{}; },
        [](any&&) -> expression { return nothing{}; },
        // [](unit&&) -> expression { return nothing{}; },
        [](nothing&&) -> expression { return any{}; },
        [](identifier&& e) -> expression { return negated{move(e)}; },
        [](rec<negated>&& e) -> expression { return e->f; },
        unmapped<equals_to, negated>(env),
        // [](auto&& e) -> expression { return make_operation<intersection_with>(any{}, negated{move(e)}); }
        [](auto&& e) -> expression { return negated{move(e)}; }
    );
}

std::optional<expression> IntersectFind(
    expression&& l,
    expression&& r,
    environment& env) {
    // DebugPrint("intersect-find", l, env);
    // if (l == r)
    //     return l;
    DebugPrint("intersect-find-2", r, env);
    // return make_operation<intersection_with>(move(l), move(r));
    auto intersected = Intersect(move(l), move(r), env);
    DebugPrint("intersect-find return", intersected, env);
    return match(move(intersected),
        [](nothing&&) -> std::optional<expression> { return {}; },
        [&](rec<equals_to>&& e) -> std::optional<expression> {
            // todo: make it fully recursive
            // DebugPrint("intersect-find eq", e, env);
            return match(move(e->x),
                [](nothing&&) -> std::optional<expression> { return {}; },
                [](auto&& e) -> std::optional<expression> { return {equals_to{move(e)}}; }
            );
        },
        [](auto&& e) -> std::optional<expression> { return { e }; }
    );
}

expression Intersect(expression&& l,
                     expression&& r,
                     environment& env) {
    DebugPrint("intersect1", l, env);
    DebugPrint("intersect2", r, env);
    if (l == r)
        return r;

    // todo: perhaps it's an extra eval here, but it seems to work with it
    l = Eval(move(l), env);
    r = Eval(move(r), env);
    if (l == r)
        return r;
    if (std::get_if<any>(&l))
        return r;
    if (std::get_if<any>(&r))
        return l;
    {
        DebugPrint("intersect unapply l", l, env);
        env.increaseDebugIndentation();
        defer { env.decreaseDebugIndentation(); };
        if (Unapply(copy(l), copy(r), env))
            return r;
        env.errors.clear();
    }
    {
        DebugPrint("intersect unapply r", r, env);
        env.increaseDebugIndentation();
        defer { env.decreaseDebugIndentation(); };
        if (Unapply(copy(r), copy(l), env))
            return l;
        env.errors.clear();
    }
    auto result = match(move(l),
        intersect_for_datatype<int>{r, env},
        intersect_for_datatype<basic_type<int>>{r, env},
        intersect_for_datatype<std::string>{r, env},
        intersect_for_datatype<basic_type<std::string>>{r, env},
        [&r, &env](rec<equals_to>&& l) -> expression {
            return match(move(r),
                [&l, &env](rec<equals_to>&& r) -> expression { return equals_to{Intersect(move(l->x), move(r->x), env)}; },
                [&l](any&&) -> expression { return l; },
                [&l](identifier&& v) -> expression { return make_operation<intersection_with>(move(l), move(v)); },
                [&env](auto&& e) -> expression { 
                    env.errors.push_back("can't intersect with " + Show(move(e)));
                    return nothing{}; 
                }
            );
        },
        [&r, &env](rec<intersection_with>&& l) -> expression {
            return match(move(r),
                [&l, &env](rec<intersection_with>&& r) -> expression { 
                    return intersection_with{Intersect(move(l->x), move(r->x), env)}; 
                },
                [&l](any&&) -> expression { return l; },
                [&l](identifier&& v) -> expression { return make_operation<intersection_with>(move(l), move(v)); },
                [&env](auto&& e) -> expression { 
                    env.errors.push_back("can't intersect with " + Show(move(e)));
                    return nothing{}; 
                }
            );
        },
        [&r, &env](rec<application>&& lApplication) -> expression {
            auto mapped = map_union_l{r, env, Intersect}.operator()<true>(move(*lApplication));
            return mapped;
        },
        [&r, &env](rec<closure>&& l) -> expression {
            return make_operation<intersection_with>(move(l), move(r));
        },
        [&r](any&&) -> expression { return r; },
        [&r](identifier&& v) -> expression { return make_operation<intersection_with>(move(v), move(r)); },
        [](nothing&& e) -> expression { return e; },
        [&env](auto&& e) -> expression {
            env.errors.push_back(std::string("can't intersect ") + Show(e));
            return nothing{};
        }
    );
    DebugPrint("intersect result", result, env);
    
    return result;
}

expression Union(expression&& l, expression&& r) {
    return match(move(l),
        [&r](nothing&&) -> expression { return r; },
        [](any&& e) -> expression { return e; },
        [&r](auto&& l) -> expression { 
            return match(move(r),
                [](any&& e) -> expression { return e; },
                [&l](nothing&&) -> expression { return l; },
                [&l](auto&& r) -> expression { return make_operation<union_with>(move(l), move(r)); }
            ); 
        }
    );
}

}