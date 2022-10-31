#pragma once
#include <boost/core/demangle.hpp>
#include <iostream>
#include "types.h"
#include "util.h"

namespace cx {

expression Eval(expression&& e, environment& env);

inline std::string Show(expression&& e) {
    return std::visit(overload{
        [](data<int>&& e)  { return std::to_string(e.data); },
        [](identifier&& e) { return e.name; },
        [](error&& e) { return e.message; },
        [](auto&& e)  { return boost::core::demangle(typeid(decltype(e)).name()); }
    }, move(e));
}

template <typename datatype>
inline expression Add(addition_with<datatype>&& function,
                                   expression&& argument) {
    return std::visit(overload{
        [&function](data<datatype>&& other) -> expression {
            return data<datatype>{function.data + other.data}; 
        },
        [](auto&& e) -> expression { return error{std::string("error: can't add ") + Show(move(e))}; }
    }, move(argument));
}

inline expression MkAddition(expression&& argument) {
    return std::visit(overload{
        [](data<int>&& x) -> expression {
            return addition_with{x.data}; 
        },
        [](auto&& e) -> expression { return error{"error: can't make addition with " + Show(move(e))}; }
    }, move(argument));
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
//     }, move(function.x), move(argument));
// }

inline expression MkEquals(expression&& argument) {
    return equals_to{std::make_unique<expression>(move(argument))};
    // return std::visit(overload{
    //     [](auto&& x) -> expression { return equals_to{move(x)}; }
    // }, move(argument));
}

inline expression Apply(expression&& function, 
                        expression&& argument, 
                        environment& env) {
    return std::visit(overload{
        [&env, &argument](closure&& function) -> expression { return function; },
        [&env, &argument](addition&&) { return MkAddition(Eval(move(argument), env)); },
        [&env, &argument](addition_with<int>&& function) { 
            return Add(move(function), Eval(move(argument), env));
        },
        [&env, &argument](equality&&) { return MkEquals(Eval(move(argument), env)); },
        [&env, &argument](equals_to&& function) -> expression { 
            auto l = Eval(move(*function.x), env); 
            auto r = Eval(move(argument), env);
            if (l.index() == r.index()) // todo
                return r;
            else
                return nothing{};
        },
        [&argument](auto&& e) -> expression { 
            return error{"error: can't apply " + Show(move(e)) + " to " + Show(move(argument)) };
        }
    }, move(function));
}

inline expression Eval(expression&& e, 
                       environment& env) {
    return std::visit(overload{
        // [&env](identifier&& e) { return e.name; },
        [&env](application&& e) {
            auto argument = Eval(move(*e.argument), env);
            auto function = Eval(move(*e.function), env);
            // substitute
            return Apply(move(function), move(argument), env);
        },
        [](auto&& e) -> expression { return e; }
    }, move(e));
}

}