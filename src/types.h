#pragma once
#include <vector>
#include <utility>
#include <variant>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include "util.h"

namespace cx {

template <class Operator>
struct GetOperation {};

struct environment;
struct application;
struct closure;
struct equals_to;
struct negated;
struct then;
struct intersection_with;
struct addition_with_expr;
struct subtraction_with_expr;
struct multiplication_with_expr;
struct abstraction;

struct unit { bool operator==(unit const&) const = default; };
struct any { bool operator==(any const&) const = default; };
struct nothing { bool operator==(nothing const&) const = default; };
struct identifier {
    std::string name; 
    bool operator==(identifier const&) const = default;
};
struct error {
    std::string message;
    bool operator==(error const&) const = default;
};
struct equality { bool operator==(equality const&) const = default; };
struct inequality { bool operator==(inequality const&) const = default; };
struct intersection { bool operator==(intersection const&) const = default; };
struct addition { bool operator==(addition const&) const = default; };
template <typename datatype>
struct addition_with {
    datatype data;
    bool operator==(addition_with<datatype> const&) const = default;
};
struct subtraction { bool operator==(subtraction const&) const = default; };
template <typename datatype>
struct subtraction_with {
    datatype data;
    bool operator==(subtraction_with<datatype> const&) const = default;
};
struct multiplication { bool operator==(multiplication const&) const = default; };
template <typename datatype>
struct multiplication_with {
    datatype data;
    bool operator==(multiplication_with<datatype> const&) const = default;
};

struct show { bool operator==(show const&) const = default; };
struct print { bool operator==(print const&) const = default; };

template <typename T>
using rec = boost::recursive_wrapper<T>;

template<class T>
bool operator==(rec<T> const& l, rec<T> const& r) {
    return l.get() == r.get();
}

using expression = std::variant<
    nothing,
    identifier,
    unit,
    rec<then>,
    error,
    any,

    intersection,
    equality,
    inequality,
    addition,
    subtraction,
    multiplication,

    rec<application>,
    rec<intersection_with>,
    rec<abstraction>,
    rec<equals_to>,
    rec<closure>,
    rec<negated>,
    rec<addition_with_expr>,
    rec<multiplication_with_expr>,
    rec<subtraction_with_expr>,

    int,
    addition_with<int>,
    subtraction_with<int>,
    multiplication_with<int>,

    std::string,
    addition_with<std::string>,

    show,
    print
>;

struct application {
    expression function;
    expression argument;
    bool operator==(application const&) const = default;
};

template <typename Op>
expression make_operation(expression&& l, expression&& r) {
    return application{Op{std::move(l)}, std::move(r)};
}
template <typename Op>
expression make_operation_binary(expression&& l, expression&& r) {
    return application{application{Op{}, std::move(l)}, std::move(r)};
}

struct then {
    expression from;
    expression to;
    bool operator==(then const&) const = default;
};

struct intersection_with {
    expression x;
    bool operator==(intersection_with const&) const = default;
};

struct addition_with_expr {
    expression x;
    bool operator==(addition_with_expr const&) const = default;
};
struct sustraction_with_expr {
    expression x;
    bool operator==(sustraction_with_expr const&) const = default;
};
struct multiplication_with_expr {
    expression x;
    bool operator==(multiplication_with_expr const&) const = default;
};

struct abstraction {
    expression argument;
    expression body;
    bool operator==(abstraction const&) const = default;
};

struct equals_to {
    expression x;
    bool operator==(equals_to const&) const = default;
};

struct negated {
    expression f;
    bool operator==(negated const&) const = default;
};

// void DebugPrint(const std::string& msg, expression e, environment& env, int color = 1);

struct environment {
    std::vector<std::pair<std::string, expression>> variables;

    inline const expression* get(const std::string& key) const {
        for (auto& [currentKey, value]: boost::adaptors::reverse(variables))
            if (currentKey == key)
                return &value;
        return nullptr;
    }
    
    inline bool add(const std::string& key, expression&& value) {
        for (auto& [currentKey, oldValue]: boost::adaptors::reverse(variables))
            if (currentKey == key) {
                oldValue = make_operation<intersection_with>(std::move(oldValue), std::move(value));
                // DebugPrint("refining", oldValue, *this);
                return false;
            }
        variables.push_back({key, std::move(value)});
        return true;
    }
    bool operator==(environment const&) const = default;

    // debug-only
    int debugIndentation{};
    void increaseDebugIndentation() { debugIndentation++; }
    void decreaseDebugIndentation() { debugIndentation--; }
};

struct closure {
    expression argument;
    expression body;
    environment env;
    bool operator==(closure const&) const = default;
};

}