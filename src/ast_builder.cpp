#include "ast_builder.h"
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <iostream>
#include <sstream>

#include "robin_hood.h"
#include "types.h"
#include "grammar.h"
#include "util.h"
// template <typename... Args>
// auto mk(Args&&... args) -> decltype(std::make_unique<cx::expression>(std::forward<Args>(args)...)) {
//     return std::make_unique<cx::expression>(std::forward<Args>(args)...);
// }

auto mk(cx::expression&& e) { return e; }

namespace cx::parser {

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
        return mk(ret);
    } else if (node.type == "cx::parser::operators_0") {
        if (node.string() == "=")
            return mk(cx::equality{});
    } else if (node.type == "cx::parser::operators_8") {
        if (node.string() == "+")
            return mk(cx::addition{});
        else if (node.string() == "-")
            return mk(cx::subtraction{});
    } else if (node.type == "cx::parser::operators_9") {
        if (node.string() == "*")
            return mk(cx::multiplication{});
        else if (node.string() == "/")
            return mk(cx::multiplication{});
        else if (node.string() == "%")
            return mk(cx::multiplication{});
    } else if (node.type == "cx::parser::operation_apply" || node.type == "cx::parser::definition") {
        // todo: no, application can have 3 children
        if (node.children.size() == 2) {
            return mk(cx::application{
                std::move(build(*node.children[0])),
                std::move(build(*node.children[1]))
            });
        } else if (node.children.size() == 3) {
            auto l = build(*node.children[0]);
            auto o = build(*node.children[1]);
            auto r = build(*node.children[2]);
            return mk(cx::application{
                mk(cx::application{std::move(o), std::move(l)}),
                std::move(r)
            });
        }
    }

    for (auto& child: node.children)
        return build(*child);

    return mk(unit{});
}

}