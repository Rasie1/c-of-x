#pragma once
#include <vector>
#include <utility>
#include <variant>
#include <optional>
#include <boost/range/adaptor/reversed.hpp>
#include "types.h"
#include "io.h"

namespace cx {

std::optional<expression> IntersectFind(expression&& l, expression&& r, environment& env);
void DebugPrint(const std::string& msg, expression e, environment& env, int color);

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

    enum class extension_result {
        Match,
        NotAdded,
        Added,
        Extended,
        Void,
    };
    
    inline extension_result add(const std::string& key, expression&& value) {
        for (auto& [currentKey, oldValue]: boost::adaptors::reverse(variables))
            if (currentKey == key) {
                if (oldValue == value)
                    return extension_result::Match;
                // oldValue = make_operation<intersection_with>(move(oldValue), move(value));
                if (auto intersected = IntersectFind(copy(oldValue), move(value), *this)) {
                    if (oldValue == *intersected)
                        return extension_result::Match;
                    DebugPrint("tried to extend env variable", *intersected, *this, 0);
                    oldValue = *intersected;
                } else
                    return extension_result::Void;
                    
                return extension_result::NotAdded;
            }
        variables.push_back({key, move(value)});
        return extension_result::Added;
    }

    inline bool define(const std::string& key, expression&& value) {
        for (auto& [currentKey, oldValue]: boost::adaptors::reverse(variables))
            if (currentKey == key) {
                // if (oldValue == value)
                //     return true; // incorrect because no variable is defined?
                return false;
            }
        variables.push_back({key, move(value)});
        return true;
    }

    inline bool remove(const std::string& key) {
        for (auto it = variables.begin(); it != variables.end(); ++it) {
            if (it->first == key) {
                variables.erase(it);
                return true;
            }
        }

        return false;
    }

    bool operator==(environment const&) const = default;

    int debugIndentation{};
    bool isTraceEnabled{};
    bool isExecuting{};
    void increaseDebugIndentation() { debugIndentation++; }
    void decreaseDebugIndentation() { debugIndentation--; }

    std::vector<std::string> errors;
};

struct closure {
    expression argument;
    expression body;
    environment env;
    bool operator==(closure const&) const = default;
};

}