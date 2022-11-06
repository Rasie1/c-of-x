#pragma once
#include <iostream>
#include <optional>
#include <boost/core/demangle.hpp>
#include "types.h"
#include "util.h"

namespace cx {

expression Eval(expression&& e, environment& env);

inline std::string Show(expression&& e) {
    return std::visit(overload{
        [](int&& x)  { return std::to_string(x); },
        [](std::string&& x)  {
            std::stringstream s;
            s << '\"' << escaped{x.c_str()} << '\"';
            return s.str();
        },
        [](identifier&& e) { return std::string("id:") + e.name; },
        [](rec<application>&& e) {
            return std::string("(") + Show(std::move(e.get().function)) 
                              + " " + Show(std::move(e.get().argument)) + std::string(")"); 
        },
        [](error&& e) { return e.message; },
        [](auto&& e)  { return boost::core::demangle(typeid(decltype(e)).name()); }
    }, std::move(e));
}

inline expression GetElement(expression&& set) {
    // {
    //     auto copy = set;
    //     std::cout << "getting element of " << Show(std::move(copy)) << std::endl;
    // }
    return std::visit(overload{
        [](rec<equals_to>&& e) { return e.get().x; },
        [](auto&& e) -> expression {
            return error{std::string("error: ") + Show(std::move(e)) + " doesn't contain values"}; 
        }
    }, std::move(set));
}

// template <typename target_type>
// inline expression Cast(expression&& expr, environment& env) {
//     return std::visit(overload{
//         [](target_type&& e){ return e; },
//         [&env](identifier&& e) { 
//             if (auto expr = env.get(e.name)) {
//                 auto address = &expr;
//                 auto evaluated = Eval(std::move(*expr), env);
//                 auto res = std::visit(overload{
//                     [&function](datatype&& other) -> expression {
//                         return function.data + other; 
//                     },
//                     [](auto&& e) -> expression { return e; }
//                 }, std::move(evaluated));

//             }
//             return error{std::string("undefined variable ") + e.name};
//         },
//         [](auto&& e) -> expression { return error{"type error"}; }
//     }, std::move(expr), env);
// }

inline expression Fix(expression&& expr, environment& env) {
    // TODO: possibly this should be BFS
    return std::visit(overload{
        [&env](identifier&& e) -> expression { 
            if (auto expr = env.get(e.name)) {
                return GetElement(Fix(std::move(*expr), env));
                // auto copy = *expr;
                // auto evaluated = Eval(std::move(*expr), env) ;
                // if (evaluated == copy && !std::holds_alternative<identifier>(evaluated))
                //     return GetElement(std::move(evaluated));
                // else
                //     return GetElement(std::move(Fix(std::move(evaluated), env)));
            }
            return error{std::string("undefined variable ") + e.name};
        },
        [&env](rec<equals_to>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env);
            return function;
        },
        [&env](rec<intersection_with>&& function) -> expression {
            function.get().x = Fix(std::move(function.get().x), env);
            return function;
        },
        [&env](rec<application>&& e) -> expression {
            e.get().function = Fix(std::move(e.get().function), env);
            e.get().argument = Fix(std::move(e.get().argument), env);
            return e;
        },
        [&env](rec<then>&& e) -> expression {
            e.get().from = Fix(std::move(e.get().from), env);
            e.get().to = Fix(std::move(e.get().to), env);
            return e;
        },
        [](auto&& e) -> expression { return e; }
    }, Eval(std::move(expr), env));
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
        [](std::string&& x) -> expression {
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

inline expression MkIntersection(expression&& argument) {
    return std::visit(overload{
        [](int&& x) -> expression {
            return intersection_with{x}; 
        },
        [](auto&& e) -> expression { return error{"error: can't make intersection with " + Show(std::move(e))}; }
    }, std::move(argument));
}

inline bool Equals(expression&& l,
                   expression&& r) {
    return std::visit(overload{
        [&r](int&& l) { return std::visit(overload{
            [l](int&& r) { return l == r; },
            [](auto&&) { return false; }
        }, std::move(r)); },
        [&r](std::string&& l) { return std::visit(overload{
            [&l](std::string&& r) { return l == r; },
            [](auto&&) { return false; }
        }, std::move(r)); },
        [](auto&&) { return false; }
    }, std::move(l));
}

inline expression MkEquals(expression&& argument) {
    return equals_to{expression(std::move(argument))};
}

inline std::optional<std::string> ExtendEnvironment(
        expression&& function, 
        const expression& argument, 
        environment& env) {
    if (auto id = std::get_if<identifier>(&argument))
        return env.add(id->name, std::move(function))
             ? std::optional<std::string>(id->name)
             : std::nullopt;
    return std::nullopt;
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
        [&env, &argument](addition_with<std::string>&& function) {
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
        [&env, &argument](intersection&&) { return MkIntersection(Eval(std::move(argument), env)); },
        [&env, &argument](show&&) -> expression { return Show(Fix(std::move(argument), env)); },
        [&env, &argument](print&&) -> expression {
            auto evaluated = Fix(std::move(argument), env);
            if (auto s = std::get_if<std::string>(&evaluated)) {
                std::cout << *s;
                return unit{};
            } else {
                return error{"print expects string, and got " + Show(std::move(evaluated))};
            }
        },
        [&env, &argument](rec<equals_to>&& function) -> expression {

            auto lCopy = function.get().x;
            auto rCopy = argument;
            if (auto added = ExtendEnvironment(std::move(function), argument, env)) {
                ExtendEnvironment(equals_to{std::move(argument)}, lCopy, env);
                return identifier{*added};
            } else if (auto added = ExtendEnvironment(equals_to{std::move(argument)}, lCopy, env))
                return identifier{*added};
            auto l = Eval(std::move(lCopy), env); 
            auto r = Eval(std::move(rCopy), env);
            if (auto expr = Equals(std::move(l), std::move(r))) {
                return expr;
            } else {
                return nothing{};
            }
        },
        [&env, &argument](rec<intersection_with>&& function) -> expression {
            auto l = Eval(std::move(function.get().x), env); 
            auto r = Eval(std::move(argument), env);
            if (auto expr = Equals(std::move(l), std::move(r))) {
                return expr;
            } else {
                return nothing{};
            }
        },
        [&env](identifier&& e) -> expression {
            // if (auto expr = env.get(e.name)) {
            //     auto copy = *expr;
            //     return GetElement(std::move(copy));
            // }
            return e;
        },
        [&argument](auto&& e) -> expression {
            return error{"error: can't apply " + Show(std::move(e)) + " to " + Show(std::move(argument)) };
        }
    }, std::move(function));
}

inline expression Eval(expression&& e, 
                       environment& env) {
    // {
    //     auto copy = e;
    //     std::cout << "evaluating " << Show(std::move(copy)) << std::endl;
    // }
    return std::visit(overload{
        // [&env](identifier&& e) { return e.name; },
        [&env](rec<application>&& e) {
            auto argument = Eval(std::move(e.get().argument), env);
            auto function = Eval(std::move(e.get().function), env);
            // substitute
            return Apply(std::move(function), std::move(argument), env);
        },
        [&env](rec<then>&& e) {
            auto from = Eval(std::move(e.get().from), env);
            if (std::get_if<error>(&from))
                return from;
            auto to = Eval(std::move(e.get().to), env);
            return to;
        },
        [](auto&& e) -> expression { return e; }
    }, std::move(e));
}

}