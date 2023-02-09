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
struct union_with;
struct tuple;
struct addition_with;
struct subtraction_with;
struct multiplication_with;
struct implication_with;
struct abstraction;
struct set;

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
struct union_ { bool operator==(union_ const&) const = default; };
struct addition { bool operator==(addition const&) const = default; };
struct multiplication { bool operator==(multiplication const&) const = default; };
struct subtraction { bool operator==(subtraction const&) const = default; };
struct implication { bool operator==(implication const&) const = default; };

template<typename datatype>
struct basic_type { bool operator==(basic_type<datatype> const&) const = default; };

struct show { bool operator==(show const&) const = default; };
struct print { bool operator==(print const&) const = default; };
struct set_trace_enabled { bool operator==(set_trace_enabled const&) const = default; };

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
    union_,
    equality,
    inequality,
    addition,
    subtraction,
    multiplication,
    implication,

    rec<application>,
    rec<intersection_with>,
    rec<union_with>,
    rec<tuple>,
    rec<abstraction>,
    rec<equals_to>,
    rec<closure>,
    rec<negated>,
    rec<addition_with>,
    rec<multiplication_with>,
    rec<subtraction_with>,
    rec<implication_with>,
    rec<set>,

    int, basic_type<int>,
    std::string, basic_type<std::string>,

    show,
    print,
    set_trace_enabled
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

struct union_with {
    expression x;
    bool operator==(union_with const&) const = default;
};

struct tuple {
    std::vector<expression> xs;
    bool operator==(tuple const&) const = default;
};

struct addition_with {
    expression x;
    bool operator==(addition_with const&) const = default;
};
struct subtraction_with {
    expression x;
    bool operator==(subtraction_with const&) const = default;
};
struct multiplication_with {
    expression x;
    bool operator==(multiplication_with const&) const = default;
};
struct implication_with {
    expression x;
    bool operator==(implication_with const&) const = default;
};

struct abstraction {
    expression argument;
    expression body;
    bool operator==(abstraction const&) const = default;
};

struct set {
    std::vector<expression> x;
    bool operator==(set const&) const = default;
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
                return false;
            }
        variables.push_back({key, std::move(value)});
        return true;
    }

    inline bool define(const std::string& key, expression&& value) {
        for (auto& [currentKey, oldValue]: boost::adaptors::reverse(variables))
            if (currentKey == key)
                return false;
        variables.push_back({key, std::move(value)});
        return true;
    }
    bool operator==(environment const&) const = default;

    // debug-only
    int debugIndentation{};
    bool isTraceEnabled{};
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