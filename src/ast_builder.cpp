#include "ast_builder.h"
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <iostream>
#include <sstream>

#include "robin_hood.h"
#include "environment.h"
#include "grammar.h"
#include "util.h"

namespace cx::parser {

inline bool isFirstChildOperator(const tao::pegtl::parse_tree::node& child) {
    return !child.children.empty() && child.children[0]->type.starts_with("cx::parser::operators_");
}

inline int getOperatorPriority(const cx::expression& e) {
    return match(e,
        [](const cx::addition&) { return 8; },
        [](const cx::subtraction&) { return 8; },
        [](const cx::equality&) { return 1; },
        [](const cx::inequality&) { return 1; },
        [](const cx::multiplication&) { return 9; },
        [](const cx::implication&) { return 0; },
        [](const auto&) { return 99; }
    );
}

inline bool isLeftAssociative(const cx::expression& e) {
    return match(e,
        [](const cx::subtraction&) { return true; },
        [](const auto&) { return false; }
    );
}

inline bool isHigherPriority(const cx::expression& l, const cx::expression& r) {
    return getOperatorPriority(l) > getOperatorPriority(r);
}

cx::expression build(const tao::pegtl::parse_tree::node& node) {
    // std::cout << "{" << std::endl;
    // std::cout << node.type << ", " << (node.has_content() ? node.string() : "") << std::endl;
    // for(auto& child: node.children) {
    //     build(*child);
    // }
    // std::cout << "}" << std::endl;

    if (node.is_root()) {
    } else if (node.type == "cx::parser::digits") {
        std::stringstream ss(node.string());
        int ret;
        ss >> ret;
        return ret;
    } else if (node.type == "cx::parser::literal_string") {
        return unescape(node.string().substr(1, node.string().size() - 2));
    } else if (node.type == "cx::parser::operators_0") {
        // if (node.string() == ";")
            return cx::implication{};
    } else if (node.type == "cx::parser::operators_1") {
        if (node.string() == "=")
            return cx::equality{};
        else if (node.string() == "!=")
            return cx::inequality{};
    } else if (node.type == "cx::parser::operators_2") {
        // if (node.string() == ":")
            return cx::equality{}; // todo: application
    } else if (node.type == "cx::parser::operators_3") {
        // if (node.string() == "->")
            return cx::abstraction{};
    } else if (node.type == "cx::parser::identifier") {
        if (node.string() == "show")
            return cx::show{};
        else if (node.string() == "read")
            return cx::read{};
        else if (node.string() == "print")
            return cx::print{};
        else if (node.string() == "any")
            return cx::any{};
        else if (node.string() == "setTraceEnabled")
            return cx::set_trace_enabled{};
        else if (node.string() == "Int")
            return cx::basic_type<int>{};
        else if (node.string() == "String")
            return cx::basic_type<std::string>{};
        else
            return cx::identifier{node.string()};
    } else if (node.type == "cx::parser::operators_7") {
        return cx::union_{};
    } else if (node.type == "cx::parser::operators_8") {
        if (node.string() == "+")
            return cx::addition{};
    } else if (node.type == "cx::parser::operators_9") {
        if (node.string() == "*")
            return cx::multiplication{};
        else if (node.string() == "/")
            return cx::multiplication{};
        else if (node.string() == "&")
            return cx::intersection{};
        else if (node.string() == "%")
            return cx::multiplication{};
    } else if (node.type == std::string("cx::parser::operation_apply") || 
               node.type == std::string("cx::parser::definition") ||
               node.type == std::string("cx::parser::bracket_expr") ||
               node.type == std::string("cx::parser::curly_brace_expr") ||
               node.type.starts_with("tao::pegtl::star_must<cx::parser::")) {
        std::vector<const tao::pegtl::parse_tree::node*> relevantChildren;
        for (const auto& child: node.children) {
            if (child->has_content())
            if (!child->string().empty())
                relevantChildren.push_back(child.get());
        }
        if (relevantChildren.size() > 1) {
            std::optional<cx::expression> ret;
            std::optional<cx::expression> currentOperator;
            std::optional<cx::expression> previousOperator;
            for (size_t i = 0; i < relevantChildren.size(); ++i) {
                const auto& child = *relevantChildren[i];
                if (child.type.starts_with("cx::parser::operators_")) {
                    if (i == 0)
                        continue;
                    if (currentOperator) {
                        // two times operators in a row isn't correct, but this case is used with operator
                        // prioriity
                        continue;
                    }
                    currentOperator = {build(child)};
                } else if (currentOperator) {
                    auto right = build(child);
                    ret = cx::application{
                        cx::application{move(*currentOperator), move(*ret)},
                        move(right)
                    };
                    previousOperator = currentOperator;
                    currentOperator = std::nullopt;
                // } else if (isFirstChildOperator(child) && (i > 0 && i + 1 < relevantChildren.size()) && 
                //          relevantChildren[i + 1]->type.starts_with("tao::pegtl::star_must<cx::parser::")) {
                //     auto& nextChild = *relevantChildren[i + 1];
                //     ret = cx::application{
                //         move(*ret),
                //         move(build(child))
                //     };
                //     // if (isFirstChildOperator(nextChild)) {
                //         std::cout << "======================" << nextChild.children[0]->string() << std::endl;
                //         auto op = build(*nextChild.children[0]);
                //         ret = cx::application{
                //             cx::application{move(op), move(*ret)},
                //             move(build(nextChild))
                //         };
                //         ++i;
                //     // }
                } else if (isFirstChildOperator(child)) {
                    auto right = build(child);
                    // if (child.type.starts_with("tao::pegtl::star_must<cx::parser::")) {
                    //     // todo
                    if (child.children[0]->string() == "->") {
                        ret = cx::abstraction{move(*ret), move(right)};
                    } else if (child.children[0]->string() == ":") {
                        ret = cx::application{move(right), move(*ret)};
                    } else {
                        auto op = build(*child.children[0]);
                        if (previousOperator && !isHigherPriority(*previousOperator, op)) {
                            auto previousApplication = std::get_if<cx::rec<cx::application>>(&*ret);
                            if (!previousApplication)
                                throw std::runtime_error("parse error");
                            auto lhs = move(previousApplication->get().argument);
                            auto rhs = cx::application{
                                cx::application{move(op), move(lhs)},
                                move(right)
                            };
                            previousApplication->get().argument = move(rhs);
                        } else {
                            ret = cx::application{
                                cx::application{move(op), move(*ret)},
                                move(right)
                            };
                        }
                    }
                } else if (ret) {
                    ret = cx::application{
                        move(*ret),
                        move(build(child))
                    };
                } else {
                    ret = build(child);
                }
            }
            if (ret) {
                if (node.type == std::string("cx::parser::curly_brace_expr"))
                    return cx::equals_to{*ret};
                return *ret;
            }
        }
    }
    
    std::optional<cx::expression> ret;
    for (auto& child: node.children) {
        if (child->has_content() && child->string().empty())
            continue;
        if (ret) {
            ret = cx::then{
                move(*ret),
                move(build(*child))
            };
        } else {
            ret = build(*child);
        }
    }

    if (ret) {
        if (node.type == std::string("cx::parser::curly_brace_expr"))
            return cx::equals_to{*ret};
        return *ret;
    }

    throw std::runtime_error(std::string("empty leaf (") + node.type.data() + ")");
    return unit{};
}

}