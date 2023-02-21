#pragma once
#include <type_traits>
#include "types.h"

namespace cx {

template<typename operation_function>
struct map_union_l {
    expression& r;
    environment& env;
    operation_function& operation;
    auto operator()(rec<application>&& lApplication) -> expression {
        auto& rUnion = lApplication.get().argument;
        return std::visit(overload{
            [this, &rUnion](rec<union_with>&& lUnion) -> expression {
                auto rCopy = r;
                auto lCalculated = operation(std::move(lUnion.get().x), std::move(r),     env);
                auto rCalculated = operation(std::move(rUnion),         std::move(rCopy), env);
                return Union(std::move(lCalculated), std::move(rCalculated));
            },
            [&lApplication](auto&& e) -> expression { 
                return application{e, std::move(lApplication.get().argument)};
            }
        }, std::move(lApplication.get().function));
    }
};

template<typename operation_for_datatype, typename datatype>
struct map_union_r {
    datatype& l;
    auto operator()(rec<application>&& rApplication) -> expression { 
        auto& rUnion = rApplication.get().argument;
        return std::visit(overload{
            [this, &rUnion](rec<union_with>&& lUnion) -> expression {
                auto lCopy = l;
                auto lCalculated = operation_for_datatype{lUnion.get().x}(std::move(l));
                auto rCalculated = operation_for_datatype{rUnion        }(std::move(lCopy));
                return Union(std::move(lCalculated), std::move(rCalculated));
            },
            [&rApplication](auto&& e) -> expression { 
                return application{e, std::move(rApplication.get().argument)};
            }
        }, std::move(rApplication.get().function));
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