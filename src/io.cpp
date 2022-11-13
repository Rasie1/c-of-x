#include "io.h"
#include "operations.h"
#include <boost/core/demangle.hpp>
#include <iostream>
#include <sstream>

namespace cx {

std::string Show(expression&& e) {
    return std::visit(overload{
        [](int&& x)  { return std::to_string(x); },
        [](std::string&& x)  {
            std::stringstream s;
            s << '\"' << escaped{x.c_str()} << '\"';
            return s.str();
        },
        [](identifier&& e) { return std::string("id(") + e.name + ")"; },
        [](rec<application>&& e) {
            return std::string("(")
                 + Show(std::move(e.get().function)) + " "
                 + Show(std::move(e.get().argument)) + ")"; 
        },
        [](rec<then>&& e) {
            return Show(std::move(e.get().from)) + "; "
                 + Show(std::move(e.get().to)); 
        },
        [](rec<intersection_with>&& e) {
            return Show(std::move(e.get().x)) + std::string(" &"); 
        },
        [](rec<equals_to>&& e) {
            return Show(std::move(e.get().x)) + std::string(" ="); 
        },
        [](addition&&) -> std::string {
            return "+"; 
        },
        [](equality&&) -> std::string {
            return "="; 
        },
        [](multiplication&&) -> std::string {
            return "*"; 
        },
        [](error&& e) { return std::string("error(\"") + e.message + "\")"; },
        [](auto&& e)  { return boost::core::demangle(typeid(decltype(e)).name()); }
    }, std::move(e));
}


void DebugPrint(const std::string& msg, expression e, environment& env, int color) {
    if (color == 1)
        std::cout << "\033[0;34m";
    else if (color == 2)
        std::cout << "\033[0;35m";
    for (int indentation = 0; indentation < env.debugIndentation; ++indentation)
        std::cout << ". ";
    std::cout << msg << ": \033[1;30m" << Show(std::move(e)) << std::endl;
    std::cout << "\033[0m";
}

expression Print(expression&& e, environment& env) {
    auto evaluated = Fix(std::move(e), env);
    if (auto s = std::get_if<std::string>(&evaluated)) {
        std::cout << *s;
        return unit{};
    } else {
        return error{"print expects string, and got " + Show(std::move(evaluated))};
    }
}

}