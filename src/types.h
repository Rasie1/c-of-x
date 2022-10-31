#pragma once
#include <vector>
#include <utility>
#include <variant>
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

template <typename datatype>
struct data { datatype data; };

struct addition {};
template <typename datatype>
struct addition_with { datatype data; };

using expression = std::variant<
    identifier,
    unit,
    error,
    closure,
    nothing,

    application,
    addition,
    equality,
    equals_to,

    data<int>,
    addition_with<int>
>;

struct application {
    std::unique_ptr<expression> function;
    std::unique_ptr<expression> argument;
};

struct equals_to { std::unique_ptr<expression> x; };

struct environment {
    std::vector<std::pair<std::string, std::unique_ptr<expression>>> variables;
};

struct closure {
    std::unique_ptr<expression> e;
    environment env;
};

}