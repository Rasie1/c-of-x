#pragma once
#include <vector>
#include <utility>
#include <variant>
#include <boost/variant/recursive_wrapper.hpp>
#include "util.h"

namespace cx {

struct environment;
struct application;
struct closure;
struct equals_to;

struct unit {};
struct nothing {};
struct identifier { std::string name; };
struct error      { std::string message; };
struct equality {};

struct addition {};
template <typename datatype>
struct addition_with { datatype data; };
struct subtraction {};
template <typename datatype>
struct subtraction_with { datatype data; };
struct multiplication {};
template <typename datatype>
struct multiplication_with { datatype data; };

template <typename T>
using rec = boost::recursive_wrapper<T>;

using expression = std::variant<
    identifier,
    unit,
    error,
    rec<closure>,
    nothing,

    rec<application>,
    equality,
    addition,
    subtraction,
    multiplication,

    rec<equals_to>,

    int,
    addition_with<int>,
    subtraction_with<int>,
    multiplication_with<int>
>;

struct application {
    expression function;
    expression argument;
};

struct equals_to { expression x; };

struct environment {
    std::vector<std::pair<std::string, expression>> variables;
};

struct closure {
    expression e;
    environment env;
};

}