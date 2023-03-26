#pragma once
#include "operations.h"
#include "functor.h"

namespace cx {

template<typename datatype>
struct intersect_for_datatype {
    expression& r;
    environment& env;
    expression operator()(datatype&& l) {
        return match(std::move(r),
            [&l](datatype&& r) -> expression { return l == r ? expression(r) : expression(nothing{}); },
            [&l](any&&) -> expression { return l; },
            [&l](identifier&& v) -> expression { return make_operation<intersection_with>(l, std::move(v)); },
            [this, &l](rec<application>&& rApplication) -> expression { 
                auto lCopy = l;
                auto rCopy = rApplication.get();
                auto mapped = map_union_r<intersect_for_datatype<datatype>, datatype>{lCopy, env}.template operator()<true>(rApplication.get());
                return mapped;
            },
            [this, &l](auto&& r) -> expression { 
                env.errors.push_back("can't intersect " + Show(std::move(l)) + " with " + Show(std::move(r)));
                return nothing{}; 
            }
        ); 
    }
};

template<typename datatype, typename calculation, typename calculation_function> 
struct calculation_for_datatype {
    expression& r;
    environment& env;
    expression operator()(datatype&& l) {
        return match(std::move(r),
            [&l](datatype&& r) -> expression { return calculation::calculate(std::move(l), std::move(r)); },
            [&l](identifier&& r) -> expression { return make_operation<calculation_function>(l, std::move(r)); },
            map_union_r<calculation, datatype>{l, env},
            [this, &l](auto&&) -> expression { 
                env.errors.push_back("types don't match for calculation: " + Show(std::move(l)) + " and " + Show(std::move(r)));
                return nothing{}; 
            }
        ); 
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