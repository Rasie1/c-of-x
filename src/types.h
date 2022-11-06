#pragma once
#include <vector>
#include <utility>
#include <variant>
#include <boost/variant/recursive_wrapper.hpp>
#include <boost/range/adaptor/reversed.hpp>
#include "util.h"

namespace cx {

struct environment;
struct application;
struct closure;
struct equals_to;
struct then;
struct intersection_with;

struct unit {
    bool operator==(unit const&) const = default;
};
struct nothing {
    bool operator==(nothing const&) const = default;
};
struct identifier {
    std::string name; 
    bool operator==(identifier const&) const = default;
};
struct error {
    std::string message;
    bool operator==(error const&) const = default;
};
struct equality {
    bool operator==(equality const&) const = default;
};
struct intersection {
    bool operator==(intersection const&) const = default;
};

struct addition {
    bool operator==(addition const&) const = default;
};
template <typename datatype>
struct addition_with {
    datatype data;
    bool operator==(addition_with<datatype> const&) const = default;
};
struct subtraction {
    bool operator==(subtraction const&) const = default;
};
template <typename datatype>
struct subtraction_with {
    datatype data;
    bool operator==(subtraction_with<datatype> const&) const = default;
};
struct multiplication {
    bool operator==(multiplication const&) const = default;
};
template <typename datatype>
struct multiplication_with {
    datatype data;
    bool operator==(multiplication_with<datatype> const&) const = default;
};

struct show {
    bool operator==(show const&) const = default;
};
struct print {
    bool operator==(print const&) const = default;
};

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
    rec<closure>,

    rec<application>,
    intersection,
    equality,
    addition,
    subtraction,
    multiplication,

    rec<intersection_with>,
    rec<equals_to>,

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

struct then {
    expression from;
    expression to;
    bool operator==(then const&) const = default;
};

struct intersection_with {
    expression x;
    bool operator==(intersection_with const&) const = default;
};

struct equals_to {
    expression x;
    bool operator==(equals_to const&) const = default;
};

struct environment {
    std::vector<std::pair<std::string, expression>> variables;

    inline const expression* get(const std::string& key) const {
        for (auto& [currentKey, value]: boost::adaptors::reverse(variables))
            if (currentKey == key)
                return &value;
        return nullptr;
    }
    
    inline expression* get(const std::string& key) {
        for (auto& [currentKey, value]: boost::adaptors::reverse(variables))
            if (currentKey == key)
                return &value;
        return nullptr;
    }

    inline bool add(const std::string& key, expression&& value) {
        for (auto& [currentKey, oldValue]: boost::adaptors::reverse(variables))
            if (currentKey == key) {
                oldValue = application{
                    intersection_with{std::move(oldValue)},
                    std::move(value)
                };
                return false;
            }
        variables.push_back({key, std::move(value)});
        return true;
    }
    bool operator==(environment const&) const = default;
};

struct closure {
    expression e;
    environment env;
    bool operator==(closure const&) const = default;
};

}