#pragma once
#include "operations.h"
#include "functor.h"

namespace cx {

template<typename datatype, typename calculation, typename calculation_function> 
struct calculation_for_datatype {
    expression& r;
    environment& env;
    expression operator()(datatype&& l) {
        return match(move(r),
            [&l](datatype&& r) -> expression { return calculation::calculate(move(l), move(r)); },
            [&l](identifier&& r) -> expression { return make_operation<calculation_function>(l, move(r)); },
            map_union_r<calculation, datatype, calculation_function>{l, env},
            [this, &l](auto&&) -> expression { 
                env.errors.push_back("types don't match for calculation: " + Show(move(l)) + " and " + Show(move(r)));
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
    static datatype calculate(datatype&& l, datatype&& r) { return r - l; }
};
template<typename datatype>
struct multiplication_for_datatype : calculation_for_datatype<datatype, multiplication_for_datatype<datatype>, multiplication_with> {
    static datatype calculate(datatype&& l, datatype&& r) { return l * r; }
};
template<typename datatype>
struct division_for_datatype : calculation_for_datatype<datatype, division_for_datatype<datatype>, division_with> {
    static datatype calculate(datatype&& l, datatype&& r) { return r / l; }
};

}