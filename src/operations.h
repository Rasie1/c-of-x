#pragma once
#include <optional>
#include "types.h"
#include "util.h"
#include "io.h"

namespace cx {

expression Eval(expression&& e, environment& env);
expression GetElement(expression&& set);
expression Fix(expression&& expr, environment& env);

template <typename datatype>
inline expression Add(addition_with<datatype>&& function,
                                   expression&& argument) {
    return std::visit(overload{
        [&function](datatype&& other) -> expression {
            return function.data + other; 
        },
        [](auto&& e) -> expression { return error{std::string("can't add ") + Show(std::move(e))}; }
    }, std::move(argument));
}


template <typename datatype>
inline expression Subtract(subtraction_with<datatype>&& function,
                                   expression&& argument) {
    return std::visit(overload{
        [&function](datatype&& other) -> expression {
            return function.data - other; 
        },
        [](auto&& e) -> expression { return error{std::string("can't subtract ") + Show(std::move(e))}; }
    }, std::move(argument));
}


template <typename datatype>
inline expression Multiply(multiplication_with<datatype>&& function,
                                   expression&& argument) {
    return std::visit(overload{
        [&function](datatype&& other) -> expression {
            return function.data * other; 
        },
        [](auto&& e) -> expression { return error{std::string("can't multiply ") + Show(std::move(e))}; }
    }, std::move(argument));
}


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
std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env);
expression Equals(expression&& l,
                  expression&& r,
                  environment& env);

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
expression Intersect(expression&& l, expression&& r);
bool IsError(expression& e);
expression Apply(expression&& function, 
                 expression&& argument, 
                 environment& env);
expression Eval(expression&& e, 
                environment& env);

}