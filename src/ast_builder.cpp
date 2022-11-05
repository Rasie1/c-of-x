#include "ast_builder.h"
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <iostream>
#include <sstream>

#include "robin_hood.h"
#include "types.h"
#include "grammar.h"
#include "util.h"

namespace cx::parser {

static cx::environment defaultEnvironment = {
    {
        {"show", cx::show{}},
        {"print", cx::print{}},
    }
};

cx::expression build(const tao::pegtl::parse_tree::node& node) {
    // std::cout << "{" << std::endl;
    // std::cout << node.type << ", " << (node.has_content() ? node.string() : "") << std::endl;
    // for(auto& child: node.children) {
    //     build(*child);
    // }
    // std::cout << "}" << std::endl;

    if (node.type == "cx::parser::digits") {
        std::stringstream ss(node.string());
        int ret;
        ss >> ret;
        return ret;
    } else if (node.type == "cx::parser::literal_string") {
        return node.string();
    } else if (node.type == "cx::parser::operators_0") {
        if (node.string() == "=")
            return cx::equality{};
    } else if (node.type == "cx::parser::identifier") {
        if (auto builtIn = defaultEnvironment.get(node.string()))
            return *builtIn;
        return cx::identifier{node.string()};
    } else if (node.type == "cx::parser::operators_8") {
        if (node.string() == "+")
            return cx::addition{};
        else if (node.string() == "-")
            return cx::subtraction{};
    } else if (node.type == "cx::parser::operators_9") {
        if (node.string() == "*")
            return cx::multiplication{};
        else if (node.string() == "/")
            return cx::multiplication{};
        else if (node.string() == "%")
            return cx::multiplication{};
    } else if (node.type == std::string("cx::parser::operation_apply") || 
               node.type == std::string("cx::parser::definition") ||
               node.type.starts_with("tao::pegtl::star_must<cx::parser::")) {
        std::vector<const tao::pegtl::parse_tree::node*> relevantChildren;
        for (const auto& child: node.children) {
            if (child->has_content())
            if (child->string() != "")
                relevantChildren.push_back(child.get());
        }
        // if (relevantChildren.size() == 2) {
        //     return cx::application{
        //         std::move(build(*relevantChildren[0])),
        //         std::move(build(*relevantChildren[1]))
        //     };
        // } else 
        if (relevantChildren.size() > 1) {
            std::optional<cx::expression> ret;
            std::optional<cx::expression> lastOperator;
            for (size_t i = 0; i < relevantChildren.size(); ++i) {
                const auto& child = *relevantChildren[i];
                if (child.type.starts_with("cx::parser::operators_")) {
                    if (i == 0)
                        continue;
                    if (lastOperator)
                        throw std::runtime_error("parse error: two operators");
                    lastOperator = {build(child)};
                } else if (lastOperator) {
                    auto right = build(child);
                    ret = cx::application{
                        cx::application{std::move(*lastOperator), std::move(*ret)},
                        std::move(right)
                    };
                    lastOperator = std::nullopt;
                } else if (!child.children.empty() &&
                    child.children[0]->type.starts_with("cx::parser::operators_")) {
                    auto op = build(*child.children[0]);
                    auto right = build(child);
                    ret = cx::application{
                        cx::application{std::move(op), std::move(*ret)},
                        std::move(right)
                    };
                } else if (ret) {
                    ret = cx::application{
                        std::move(*ret),
                        std::move(build(child))
                    };
                } else {
                    ret = build(child);
                }
            }
            if (ret)
                return *ret;
            // auto l = build(relevantChildren[0]);
            // auto o = build(relevantChildren[1]);
            // auto r = build(relevantChildren[2]);
            // return cx::application{
            //     cx::application{std::move(o), std::move(l)},
            //     std::move(r)
            // };
        }
    // } else if ("tao::pegtl::star_must<cx::parser::operators_0, cx::parser::seps, cx::parser::expr_1, cx::parser::seps>") {
    //     for (const auto& child: relevantChildren) {
    //         if (child->string_view().empty() || child->string_view() == std::string(" "))
    //             continue;
    //         if (child->string_view().starts_with("cx::parser::operators_") {
    //             auto o = build(*child);
    //         }
    //     }
    }
    // } else if ("tao::pegtl::star_must<cx::parser::operators_0, cx::parser::seps, cx::parser::expr_1, cx::parser::seps>") {
    // } else if ("tao::pegtl::star_must<cx::parser::operators_1, cx::parser::seps, cx::parser::expr_9, cx::parser::seps>") {
    // } else if ("tao::pegtl::star_must<cx::parser::operators_8, cx::parser::seps, cx::parser::expr_9, cx::parser::seps>") {
    // } else if ("tao::pegtl::star_must<cx::parser::operators_9, cx::parser::seps, cx::parser::operation_apply, cx::parser::seps>") {
    // }
    

    for (auto& child: node.children)
        return build(*child);

    throw std::runtime_error(std::string("empty leaf (") + node.type.data() + ")");
    return unit{};
}

}