#include "ast_builder.h"
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <iostream>
#include <sstream>
#include <stack>
#include <queue>

#include "robin_hood.h"
#include "environment.h"
#include "grammar.h"
#include "util.h"
#include "io.h"

namespace cx::parser {

inline bool isFirstChildOperator(const tao::pegtl::parse_tree::node& child) {
    return !child.children.empty() && child.children[0]->type.starts_with("cx::parser::operators_");
}

inline int getOperatorPriority(const cx::expression& e) {
    return match(e,
        [](const cx::multiplication&) { return 9; },
        [](const cx::division&) { return 9; },
        [](const cx::intersection&) { return 9; },
        [](const cx::addition&) { return 8; },
        [](const cx::subtraction&) { return 8; },
        [](const cx::union_&) { return 7; },
        [](const cx::arrow&) { return 3; },
        [](const cx::application_operator&) { return 2; },
        [](const cx::equality&) { return 1; },
        [](const cx::inequality&) { return 1; },
        [](const cx::implication&) { return 0; },
        [](const auto&) { return 99; }
    );
}

inline bool isLeftAssociative(const cx::expression& e) {
    return match(e,
        [](const cx::implication&) { return false; },
        [](const auto&) { return true; }
    );
}

inline bool isHigherPriority(const cx::expression& l, const cx::expression& r) {
    if (isLeftAssociative(r)) {
        return getOperatorPriority(l) >= getOperatorPriority(r);
    } else {
        return getOperatorPriority(l) >  getOperatorPriority(r);
    }
}

cx::expression makeOperation(cx::expression&& l, cx::expression&& op, cx::expression&& r) {
    return cx::match(cx::move(op),
        [&l, &r](cx::arrow&&) -> cx::expression { return cx::abstraction{cx::move(l), cx::move(r)}; },
        [&l, &r](cx::application_operator&&) -> cx::expression { return cx::application{cx::move(r), cx::move(l)}; },
        [&l, &r](cx::addition&&) -> cx::expression { return cx::application{cx::addition_with(r), cx::move(l)}; },
        [&l, &r](cx::subtraction&&) -> cx::expression { return cx::application{cx::subtraction_with(r), cx::move(l)}; },
        [&l, &r](cx::multiplication&&) -> cx::expression { return cx::application{cx::multiplication_with(r), cx::move(l)}; },
        [&l, &r](cx::division&&) -> cx::expression { return cx::application{cx::division_with(r), cx::move(l)}; },
        [&l, &r](auto&& op) -> cx::expression { return cx::application{cx::application{cx::move(op), cx::move(l)}, cx::move(r)}; }
    );
}

cx::expression build(const tao::pegtl::parse_tree::node& node) {
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
            return cx::application_operator{};
    } else if (node.type == "cx::parser::operators_3") {
        if (node.string() == "->")
            return cx::arrow{};
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
        else if (node.string() == "- ") // todo: fix grammar
            return cx::subtraction{};
    } else if (node.type == "cx::parser::operators_9") {
        if (node.string() == "*")
            return cx::multiplication{};
        else if (node.string() == "/")
            return cx::division{};
        else if (node.string() == "&")
            return cx::intersection{};
        else if (node.string() == "%")
            return cx::multiplication{};
    } else if (node.type == std::string("cx::parser::operation_apply") || 
               node.type == std::string("cx::parser::definition") ||
               node.type == std::string("cx::parser::curly_brace_expr") ||
               node.type == std::string("cx::parser::bracket_expr") ||
               node.type.starts_with("tao::pegtl::star_must<cx::parser::")) {
        std::vector<const tao::pegtl::parse_tree::node*> relevantChildren;
        for (const auto& child: node.children) {
            if (child->has_content())
            if (!child->string().empty())
                relevantChildren.push_back(child.get());
        }
        if (node.type == std::string("cx::parser::operation_apply") && relevantChildren.size() > 1) {
            std::optional<cx::expression> ret;
            for (auto& child: relevantChildren) {
                auto expr = build(*child);
                if (!ret) {
                    ret = expr;
                } else {
                    ret = cx::application{
                        *ret, expr
                    };
                }
            }

            return *ret;
        } else if (relevantChildren.size() > 1) {
            std::stack<cx::expression> operators;
            std::stack<cx::expression> operands;

            for (auto& child: relevantChildren) {
                auto expr = build(*child);

                if (child->type.starts_with("cx::parser::operators_")) {
                    while (!operators.empty() && 
                            isHigherPriority(operators.top(), expr)) {
                        auto op = operators.top();
                        operators.pop();
                        auto r = operands.top();
                        operands.pop();
                        auto l = operands.top();
                        operands.pop();
                        operands.push(makeOperation(cx::move(l), cx::move(op), cx::move(r)));
                    }
                    operators.push(expr);
                } else {
                    operands.push(expr);
                }
            }

            while (!operators.empty()) {
                auto op = operators.top();
                operators.pop();
                auto r = operands.top();
                operands.pop();
                auto l = operands.top();
                operands.pop();
                operands.push(makeOperation(cx::move(l), cx::move(op), cx::move(r)));
            }
            if (node.type == std::string("cx::parser::curly_brace_expr"))
                return cx::equals_to{operands.top()};

            return operands.top();
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