#pragma once
#include <vector>
#include <utility>
#include <variant>
#include <optional>
#include "recursive_wrapper.h"
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
struct division_with;
struct implication_with;
struct abstraction;
struct arrow;
struct application_operator;
struct set;

struct unit { bool operator==(unit const&) const = default; };
struct any { bool operator==(any const&) const = default; };
struct nothing { bool operator==(nothing const&) const = default; };
struct identifier {
    std::string name; 
    bool operator==(identifier const&) const = default;
};
struct equality { bool operator==(equality const&) const = default; };
struct inequality { bool operator==(inequality const&) const = default; };
struct intersection { bool operator==(intersection const&) const = default; };
struct union_ { bool operator==(union_ const&) const = default; };
struct addition { bool operator==(addition const&) const = default; };
struct multiplication { bool operator==(multiplication const&) const = default; };
struct division { bool operator==(division const&) const = default; };
struct subtraction { bool operator==(subtraction const&) const = default; };
struct implication { bool operator==(implication const&) const = default; };
struct arrow { bool operator==(arrow const&) const = default; };
struct application_operator { bool operator==(application_operator const&) const = default; };

template<typename datatype>
struct basic_type { bool operator==(basic_type<datatype> const&) const = default; };

struct show { bool operator==(show const&) const = default; };
struct read { bool operator==(read const&) const = default; };
struct print { bool operator==(print const&) const = default; };
struct set_trace_enabled { bool operator==(set_trace_enabled const&) const = default; };

using expression = std::variant<
    nothing,
    identifier,
    unit,
    rec<then>,
    any,

    intersection,
    union_,
    equality,
    inequality,
    addition,
    subtraction,
    multiplication,
    division,
    implication,
    arrow,
    application_operator,

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
    rec<division_with>,
    rec<subtraction_with>,
    rec<implication_with>,
    rec<set>,

    int, basic_type<int>,
    std::string, basic_type<std::string>,

    show,
    read,
    print,
    set_trace_enabled
>;

struct application {
    expression function;
    expression argument;
    bool operator==(application const&) const = default;
};

template <typename Op>
application make_operation(expression&& l, expression&& r) {
    return application{Op{move(l)}, move(r)};
}
template <typename Op>
application make_operation_binary(expression&& l, expression&& r) {
    return application{application{Op{}, move(l)}, move(r)};
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
struct division_with {
    expression x;
    bool operator==(division_with const&) const = default;
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
    expression x;
    bool operator==(negated const&) const = default;
};

}