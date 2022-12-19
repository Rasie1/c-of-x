#include "unapply.h"
#include "operations.h"

namespace cx {

inline bool IsError(expression& e) {
    return std::visit(overload{
        [](nothing&&) { return true; },
        [](error&&) { return true; },
        [](auto&&) { return false; }
    }, std::move(e));
}

template<typename datatype>
struct unapply_for_datatype {
    expression& match;
    environment& env;
    bool operator()(datatype&& pattern) {
        return std::visit(overload{
            [&pattern](datatype&& match) -> bool { return pattern == match; },
            [&pattern, this](identifier&& match) -> bool { 
                return static_cast<bool>(ExtendEnvironment(equals_to{std::move(match)}, std::move(pattern), env)); 
            },
            [](auto&&) -> bool { return false; }
        }, std::move(match)); 
    }
};

std::optional<expression> Inverse(expression& e) {
    // can't fix because we still need variables
    return std::visit(overload{
        // the case with strings is a bit different, but if I express subtraction
        // correctly, there can be the same code for both integers addition and
        // strings matching
        [](rec<addition_with>& f) -> std::optional<expression> { 
            return subtraction_with{std::move(f.get().x)}; 
            // return abstraction{
                
            // };
        },
        [](auto&) -> std::optional<expression> { return std::nullopt; }
    }, e);
}

struct is_equal_with_negated {
    expression& r;
    environment& env;
    inline auto operator()(rec<negated>&& e) -> bool { 
        DebugPrint("is equal with negated", e.get().f, env);
        auto falseEnv = env;
        auto eq = IsEqual(std::move(r), std::move(e.get().f), falseEnv);
        DebugPrint("is equal result", eq, env);
        return std::visit(overload{
            [](nothing&&) { return true; },
            [](identifier&&) { return true; },
            [](auto&&) { return false; }
        }, std::move(eq));
    }
};

bool Unapply(expression&& pattern, 
             expression&& match, 
             environment& env) {
    DebugPrint("unapply1", pattern, env);
    DebugPrint("unapply2", match, env);
    env.increaseDebugIndentation();
    auto ret = std::visit(overload{
        unapply_for_datatype<int>{match, env},
        unapply_for_datatype<std::string>{match, env},
        [](any&&) { return true; },
        is_equal_with_negated{match, env},
        [&env, &match](identifier&& pattern) -> bool {
            auto newEnv = env;
            auto evaluated = Fix(std::move(match), newEnv);
            if (IsError(evaluated))
                return false;
            if (env.define(pattern.name, equals_to{std::move(evaluated)}))
                return true;
            else {
                DebugPrint("already defined", pattern, env, 2);
                return false;
            }
        },
        // [&env, &match](rec<addition_with>&& pattern) -> bool {
        //     auto wrapped = make_operation<subtraction_with>(std::move(match), std::move(pattern.get().x));
        //     DebugPrint("moved addition", wrapped, env);
        //     return Unapply(std::move(pattern.get().function), std::move(wrapped), env);
        // },

        // this is a bad idea because intersection is moved anyway
        // [&env, &match](rec<intersection_with>&& pattern) -> bool {
        //     // auto ret = std::visit(overload{
        //     //     [&env](rec<abstraction>&& match) {

        //     //     }
        //     // }, std::move(match));

        //     auto [argument, body] = std::get<rec<abstraction>>(match).get();
        //     auto bodyCopy = body;
        //     return Unapply(std::move(pattern.get().x), std::move(body), env) 
        //         && Unapply(std::move(argument), std::move(bodyCopy), env);
        // },

        [&env, &match](rec<application>&& pattern) -> bool {
            // isn't it a mix of two concepts in one? "constrainted definitions" and "curried function definitions"
            // or this conditional is enough to distinct between them?
            DebugPrint("unapplying application", pattern.get(), env);
            if (auto inversed = Inverse(pattern.get().function)) {
                auto wrapped = application{std::move(*inversed), match};
                DebugPrint("got inverse", wrapped, env);
                return Unapply(std::move(pattern.get().argument), std::move(wrapped), env);
            } else {
                auto wrapped = abstraction{std::move(pattern.get().argument), std::move(match)};
                DebugPrint("moved abstraction", wrapped, env);
                return Unapply(std::move(pattern.get().function), std::move(wrapped), env);
            }
        },
        [&env](auto&& e) -> bool { 
            DebugPrint("couldn't unapply", e, env);
            return false; 
        }
    }, std::move(pattern));
    env.decreaseDebugIndentation();
    DebugPrint("unapply result", ret ? std::string("yes") : std::string("no"), env);
    return ret;
}

}