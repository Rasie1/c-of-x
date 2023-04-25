#pragma once
#include <type_traits>
#include "environment.h"

namespace cx {
    
template<typename operation_for_datatype, typename operation_function>
struct map_union_l {
    expression& r;
    environment& env;
    operation_function& operation;
    template<bool failOnUnknown = true>
    auto operator()(rec<application>&& lApplication) -> expression {
        DebugPrint("map_union_l, r", r, env);
        auto& rUnion = lApplication->argument;
        return match(move(lApplication->function),
            [this, &rUnion](rec<union_with>&& lUnion) -> expression {
                env.increaseDebugIndentation();
                defer { env.decreaseDebugIndentation(); };
                auto lCalculated = operation(move(lUnion->x), copy(r), env);
                auto rCalculated = operation(move(rUnion),    move(r), env);
                return Union(move(lCalculated), move(rCalculated));
            },
            [&lApplication, this](auto&& e) -> expression {
                auto applied = operation_for_datatype{application{e, move(lApplication->argument)}, move(r)};
                // auto applied = operation(application{e, move(lApplication->argument)}, move(r), env);
                // auto applied = application{e, move(lApplication->argument)};
                if constexpr (failOnUnknown) {
                    env.errors.push_back(
                        "can't correlate " + Show(move(r)) + " and " + Show(move(applied))
                    );
                    return nothing{};
                } else {
                    return applied;
                }
            }
        );
    }
};

template<typename operation_for_datatype, typename datatype>
struct map_union_r {
    datatype& l;
    environment& env;
    template<bool failOnUnknown = false>
    auto operator()(rec<application>&& rApplication) -> expression { 
        DebugPrint("map_union_r, l", l, env);
        auto& rUnion = rApplication->argument;
        return match(move(rApplication->function),
            [this, &rUnion](rec<union_with>&& lUnion) -> expression {
                DebugPrint("map_union_r, union_with, l", lUnion, env);
                env.increaseDebugIndentation();
                auto newEnv1 = env;
                auto lCalculated = operation_for_datatype{lUnion->x, newEnv1}(copy(l));
                env.decreaseDebugIndentation();
                DebugPrint("map_union_r, union_with, r", rUnion, env);
                env.increaseDebugIndentation();
                auto newEnv2 = env;
                auto rCalculated = operation_for_datatype{rUnion, newEnv2}(move(l));
                env.decreaseDebugIndentation();
                return Union(move(lCalculated), move(rCalculated));
            },
            [&rApplication, this](auto&& e) -> expression {
                auto applied = application{e, move(rApplication->argument)}; // TODO: l is forgotten
                if constexpr (failOnUnknown) {
                    env.errors.push_back(
                        "can't correlate " + Show(move(l)) + " and " + Show(move(applied))
                    );
                    return nothing{};
                } else {
                    return applied;
                }
            }
        );
    }
};

template <typename container, typename F>
struct unmapped {
    environment& env;
    expression operator()(rec<container>&& e) {
        auto wrapped = wrap(move(*e));
        return Eval(move(wrapped), env);
    }
private:
    expression wrap(equals_to&& e) { return equals_to{F{move(e.x)}}; }
};

}