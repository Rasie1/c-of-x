#pragma once
#include "operations.h"
#include "functor.h"

namespace cx {

template<typename datatype>
struct intersect_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](any&&) -> expression { return l; },
            [&l](identifier&& v) -> expression { return make_operation<intersection_with>(l, std::move(v)); },
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};

template<typename datatype>
struct equals_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](identifier&& v) -> expression { return make_operation<equals_to>(l, std::move(v)); },
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};

template<typename datatype, typename calculation, typename calculation_function> 
struct calculation_for_datatype {
    expression& r;
    expression operator()(datatype&& l) {
        return std::visit(overload{
            [&l](datatype&& r) -> expression { return calculation::calculate(std::move(l), std::move(r)); },
            [&l](identifier&& r) -> expression { return make_operation<calculation_function>(l, std::move(r)); },
            map_union_r<calculation, datatype>{l},
            [](auto&&) -> expression { return nothing{}; }
        }, std::move(r)); 
    }
};

template<typename datatype>
struct addition_for_datatype : calculation_for_datatype<datatype, addition_for_datatype<datatype>, addition_with> {
    static datatype calculate(datatype&& l, datatype&& r) { return l + r; }
};
template<typename datatype>
struct subtraction_for_datatype : calculation_for_datatype<datatype, subtraction_for_datatype<datatype>, subtraction_with> {
    static datatype calculate(datatype&& l, datatype&& r) { return l - r; }
};
template<typename datatype>
struct multiplication_for_datatype : calculation_for_datatype<datatype, multiplication_for_datatype<datatype>, multiplication_with> {
    static datatype calculate(datatype&& l, datatype&& r) { return l * r; }
};

}