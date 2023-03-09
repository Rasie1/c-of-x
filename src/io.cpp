#include "io.h"
#include "operations.h"
#include <boost/core/demangle.hpp>
#include <iostream>
#include <sstream>

namespace cx {

std::string Show(expression&& e) {
    return match(std::move(e),
        [](basic_type<int>&&) -> std::string { return "Int"; },
        [](int&& x)  { return std::to_string(x); },
        [](basic_type<std::string>&&) -> std::string { return "String"; },
        [](std::string&& x)  {
            std::stringstream s;
            s << '\"' << escaped{x.c_str()} << '\"';
            return s.str();
        },
        [](identifier&& e) { return e.name; },
        [](rec<application>&& e) {
            return std::string("(")
                 + Show(std::move(e.get().function)) + " "
                 + Show(std::move(e.get().argument)) + ")"; 
        },
        [](rec<then>&& e) {
            return Show(std::move(e.get().from)) + "; "
                 + Show(std::move(e.get().to)); 
        },
        [](rec<closure>&& e) {
            std::stringstream out;
            out << Show(std::move(e.get().argument))
                << " ->' "
                << Show(std::move(e.get().body));
            if (!e.get().env.variables.empty()) {
                out << " [";
                int i = e.get().env.variables.size() - 1;
                int lastVariableToPrint = std::max(0, i - 1);
                for (; i >= lastVariableToPrint; --i) {
                    auto& var = e.get().env.variables[i];
                    auto copy = var.second;
                    out << var.first << ": " << Show(std::move(copy));
                    if (i != lastVariableToPrint)
                        out << "; ";
                }
                if (lastVariableToPrint > 0)
                    out << "; ...";
                out << "]";
            }
            return out.str();
        },
        [](rec<abstraction>&& e) {
            return Show(std::move(e.get().argument)) + " -> "
                 + Show(std::move(e.get().body)); 
        },
        // [](rec<set>&& e) {
        //     return std::string("{") + Show(std::move(e.get().x)) + std::string("}"); 
        // },
        [](rec<intersection_with>&& e) {
            return Show(std::move(e.get().x)) + std::string(" &"); 
        },
        [](rec<equals_to>&& e) {
            return std::string("{") + Show(std::move(e.get().x)) + std::string("}"); 
        },
        [](rec<negated>&& e) {
            return std::string("!(") + Show(std::move(e.get().f)) + ")"; 
        },
        [](rec<addition_with>&& e) {
            return Show(std::move(e.get().x)) + std::string(" +"); 
        },
        [](rec<subtraction_with>&& e) {
            return Show(std::move(e.get().x)) + std::string(" -"); 
        },
        [](rec<multiplication_with>&& e) {
            return Show(std::move(e.get().x)) + std::string(" *"); 
        },
        [](rec<union_with>&& e) {
            return Show(std::move(e.get().x)) + std::string(" |"); 
        },
        [](rec<implication_with>&& e) {
            return Show(std::move(e.get().x)) + std::string(";"); 
        },
        [](addition&&) -> std::string { return "+"; },
        [](equality&&) -> std::string { return "="; },
        [](multiplication&&) -> std::string { return "*"; },
        [](implication&&) -> std::string { return ";"; },
        [](union_&&) -> std::string { return "|"; },
        [](any&&) -> std::string { return "any"; },
        [](nothing&&) -> std::string { return "void"; },
        [](error&& e) { return std::string("error(\"") + e.message + "\")"; },
        [](auto&& e)  { return boost::core::demangle(typeid(decltype(e)).name()); }
    );
}


void DebugPrint(const std::string& msg, expression e, environment& env, int color) {
    if (!env.isTraceEnabled)
        return;
    if (color == 1)
        std::cout << "\033[0;36m";
    else if (color == 2)
        std::cout << "\033[0;35m";
    for (int indentation = 0; indentation < env.debugIndentation; ++indentation)
        std::cout << ". ";
    std::cout << msg << ": \033[1;37m" << Show(std::move(e));
    if (!env.variables.empty()) {
        std::cout << "        \033[0;34m[ ";
        for (auto& v: env.variables) {
            auto value = v.second;
            // std::cout << "\033[1;34m";
            std::cout << v.first;
            // std::cout << "\033[0;34m";
            std::cout << ": " << Show(std::move(value)) << "; ";
        }
        std::cout << "]";
    }
    std::cout << std::endl;
    std::cout << "\033[0m";
}

expression Print(expression&& e, environment& env) {
    auto evaluated = SubstituteVariables(std::move(e), env);
    if (auto s = std::get_if<std::string>(&evaluated)) {
        std::cout << *s;
        return unit{};
    } else {
        return error{"print expects string, and got " + Show(std::move(evaluated))};
    }
}

expression SetTraceEnabled(expression&& e, environment& env) {
    auto evaluated = SubstituteVariables(std::move(e), env);
    if (auto s = std::get_if<int>(&evaluated)) {
        env.isTraceEnabled = static_cast<bool>(s);
    } else {
        return error{"set_trace_enabled expects int, and got " + Show(std::move(evaluated))};
    }
    return unit{};
}

}