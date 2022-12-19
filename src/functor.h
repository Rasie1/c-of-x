#pragma once
#include <type_traits>
#include "types.h"

namespace cx {

template <typename F>
expression struct_map(equals_to&& e) { return equals_to{F{std::move(e.x)}}; }

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
                auto lCalculated = operation(
                    std::move(lUnion.get().x), std::move(r), env);
                auto rCalculated = operation(
                    std::move(rUnion), std::move(rCopy), env);
                return make_operation<union_with>(std::move(lCalculated), std::move(rCalculated));
            },
            [](auto&& e) -> expression { return error{std::string("calculation/union error (left): ") + Show(e)};; }
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
                auto rCalculated = operation_for_datatype{rUnion}(std::move(lCopy));
                return make_operation<union_with>(std::move(lCalculated), std::move(rCalculated));
            },
            [](auto&& e) -> expression { return error{std::string("calculation/union error (right): ") + Show(e)};; }
        }, std::move(rApplication.get().function));
    }
};

template <typename container, typename F>
struct unmapped {
    environment& env;
    expression operator()(rec<container>&& e) { 
        return Eval(struct_map<F>(std::move(e.get())), env); 
    }
};

}