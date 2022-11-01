#pragma once
#include <boost/core/demangle.hpp>
#include <iostream>
#include "types.h"
#include "util.h"

namespace cx {

expression Eval(expression&& e, environment& env);

inline std::string Show(expression&& e) {
    return std::visit(overload{
        [](int&& x)  { return std::to_string(x); },
        [](identifier&& e) { return e.name; },
        [](error&& e) { return e.message; },
        [](auto&& e)  { return boost::core::demangle(typeid(decltype(e)).name()); }
    }, std::move(e));
}

template <typename datatype>
inline expression Add(addition_with<datatype>&& function,
                                   expression&& argument) {
    return std::visit(overload{
        [&function](datatype&& other) -> expression {
            return function.data + other; 
        },
        [](auto&& e) -> expression { return error{std::string("error: can't add ") + Show(std::move(e))}; }
    }, std::move(argument));
}


inline expression MkAddition(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression {
            return addition_with{x}; 
        },
        [](auto&& e) -> expression { return error{"error: can't make addition with " + Show(std::move(e))}; }
    }, std::move(argument));
}

inline expression MkSubtraction(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression {
            return subtraction_with{x}; 
        },
        [](auto&& e) -> expression { return error{"error: can't make subtraction with " + Show(std::move(e))}; }
    }, std::move(argument));
}

template <typename datatype>
inline expression Subtract(subtraction_with<datatype>&& function,
                                   expression&& argument) {
    return std::visit(overload{
        [&function](datatype&& other) -> expression {
            return function.data - other; 
        },
        [](auto&& e) -> expression { return error{std::string("error: can't subtract ") + Show(std::move(e))}; }
    }, std::move(argument));
}

inline expression MkMultiplication(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression {
            return multiplication_with{x}; 
        },
        [](auto&& e) -> expression { return error{"error: can't make multiplication with " + Show(std::move(e))}; }
    }, std::move(argument));
}

template <typename datatype>
inline expression Multiply(multiplication_with<datatype>&& function,
                                   expression&& argument) {
    return std::visit(overload{
        [&function](datatype&& other) -> expression {
            return function.data * other; 
        },
        [](auto&& e) -> expression { return error{std::string("error: can't multiply ") + Show(std::move(e))}; }
    }, std::move(argument));
}

// inline expression Equals(equals_to&& function,
//                         expression&& argument) {
//     return std::visit(overload{
//         [](auto&& l, auto&& r) -> expression { 
//             if (l.index() == r.index()) // todo
//                 return r;
//             else
//                 return nothing{};
//         }
//     }, std::move(function.x), std::move(argument));
// }

inline expression MkEquals(expression&& argument) {
    return equals_to{expression(std::move(argument))};
    // return std::visit(overload{
    //     [](auto&& x) -> expression { return equals_to{std::move(x)}; }
    // }, std::move(argument));
}

inline expression Apply(expression&& function, 
                        expression&& argument, 
                        environment& env) {
    return std::visit(overload{
        [&env, &argument](closure&& function) -> expression { return function; },
        [&env, &argument](addition&&) { return MkAddition(Eval(std::move(argument), env)); },
        [&env, &argument](addition_with<int>&& function) { 
            return Add(std::move(function), Eval(std::move(argument), env));
        },
        [&env, &argument](subtraction&&) { return MkSubtraction(Eval(std::move(argument), env)); },
        [&env, &argument](subtraction_with<int>&& function) { 
            return Subtract(std::move(function), Eval(std::move(argument), env));
        },
        [&env, &argument](multiplication&&) { return MkMultiplication(Eval(std::move(argument), env)); },
        [&env, &argument](multiplication_with<int>&& function) { 
            return Multiply(std::move(function), Eval(std::move(argument), env));
        },
        [&env, &argument](equality&&) { return MkEquals(Eval(std::move(argument), env)); },
        [&env, &argument](rec<equals_to>&& function) -> expression { 
            auto l = Eval(std::move(function.get().x), env); 
            auto r = Eval(std::move(argument), env);
            if (l.index() == r.index()) // todo
                return r;
            else
                return nothing{};
        },
        [&argument](auto&& e) -> expression { 
            return error{"error: can't apply " + Show(std::move(e)) + " to " + Show(std::move(argument)) };
        }
    }, std::move(function));
}

inline expression Eval(expression&& e, 
                       environment& env) {
    return std::visit(overload{
        // [&env](identifier&& e) { return e.name; },
        [&env](rec<application>&& e) {
            auto argument = Eval(std::move(e.get().argument), env);
            auto function = Eval(std::move(e.get().function), env);
            // substitute
            return Apply(std::move(function), std::move(argument), env);
        },
        [](auto&& e) -> expression { return e; }
    }, std::move(e));
}

}