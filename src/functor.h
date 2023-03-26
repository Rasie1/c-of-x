#pragma once
#include <type_traits>
#include "environment.h"

namespace cx {
    
template<typename operation_function>
struct map_union_l {
    expression& r;
    environment& env;
    operation_function& operation;
    template<bool failOnUnknown = false>
    auto operator()(rec<application>&& lApplication) -> expression {
        DebugPrint("map_union_l, r", r, env);
        auto& rUnion = lApplication.get().argument;
        return match(std::move(lApplication.get().function),
            [this, &rUnion](rec<union_with>&& lUnion) -> expression {
                auto rCopy = r;
                env.increaseDebugIndentation();
                defer { env.decreaseDebugIndentation(); };
                auto lCalculated = operation(std::move(lUnion.get().x), std::move(r),     env);
                auto rCalculated = operation(std::move(rUnion),         std::move(rCopy), env);
                return Union(std::move(lCalculated), std::move(rCalculated));
            },
            [&lApplication, this](auto&& e) -> expression {
                auto applied = application{e, std::move(lApplication.get().argument)};
                if constexpr (failOnUnknown) {
                    env.errors.push_back(
                        "can't correlate " + Show(std::move(r)) + " and " + Show(std::move(applied))
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
        auto& rUnion = rApplication.get().argument;
        return match(std::move(rApplication.get().function),
            [this, &rUnion](rec<union_with>&& lUnion) -> expression {
                auto lCopy = l;
                DebugPrint("map_union_r, union_with, l", lUnion, env);
                env.increaseDebugIndentation();
                auto lCalculated = operation_for_datatype{lUnion.get().x, env}(std::move(l));
                env.decreaseDebugIndentation();
                DebugPrint("map_union_r, union_with, r", rUnion, env);
                env.increaseDebugIndentation();
                auto rCalculated = operation_for_datatype{rUnion        , env}(std::move(lCopy));
                env.decreaseDebugIndentation();
                return Union(std::move(lCalculated), std::move(rCalculated));
            },
            [&rApplication, this](auto&& e) -> expression {
                auto applied = application{e, std::move(rApplication.get().argument)};
                if constexpr (failOnUnknown) {
                    env.errors.push_back(
                        "can't correlate " + Show(std::move(l)) + " and " + Show(std::move(applied))
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
        auto wrapped = wrap(std::move(e.get()));
        return Eval(std::move(wrapped), env);
    }
private:
    expression wrap(equals_to&& e) { return equals_to{F{std::move(e.x)}}; }
};

}