#include "parser.h"

#include <cassert>
#include <cstddef>
#include <iostream>

#include <tao/pegtl.hpp>
#include <tao/pegtl/contrib/analyze.hpp>
#include <tao/pegtl/contrib/parse_tree.hpp>
#include <tao/pegtl/contrib/parse_tree_to_dot.hpp>
#include <tao/pegtl/contrib/trace.hpp>
// #include <tao/pegtl/contrib/raw_string.hpp>

#include "grammar.h"
#include "ast_builder.h"

namespace cx::parser {

void trace(const char* file) {
    tao::pegtl::file_input in(file);
    cx::parser::state is;
    tao::pegtl::complete_trace<cx::parser::grammar, cx::parser::action>(in, is);
}

template<class Rule>
using graphviz_selector = tao::pegtl::parse_tree::selector<
    Rule,
    tao::pegtl::parse_tree::store_content::on<
        name,
        definition,
        identifier,
        numeral, digits,

        operation_apply
    >,
    tao::pegtl::parse_tree::remove_content::on<
    >/*,
    rearrange::on<
        definition
    >*/
>;

template<class Rule>
using selector = tao::pegtl::parse_tree::selector<
    Rule,
    tao::pegtl::parse_tree::store_content::on<
        name,
        definition,
        identifier,
        numeral, digits,

        operation_apply,
        operators_8,
        operators_9,
        operators_0
    >,
    tao::pegtl::parse_tree::remove_content::on<
    >/*,
    rearrange::on<
        definition
    >*/
>;

template<typename>
using all_selector = std::true_type;

void print_graphviz(const char* file, int level) {
    tao::pegtl::file_input in(file);
    cx::parser::state is;
    std::unique_ptr<tao::pegtl::parse_tree::node> root;
    if (level == 0) {
        root = tao::pegtl::parse_tree::parse<
            cx::parser::grammar,
            cx::parser::graphviz_selector,
            cx::parser::action>(in, is);
    } else if (level == 1) {
        root = tao::pegtl::parse_tree::parse<
            cx::parser::grammar,
            cx::parser::selector,
            cx::parser::action>(in, is);
    } else {
        root = tao::pegtl::parse_tree::parse<
            cx::parser::grammar,
            all_selector,
            cx::parser::action>(in, is);
    }
    if (!root)
        throw std::runtime_error("couldn't parse");
    tao::pegtl::parse_tree::print_dot(std::cout, *root);
}

std::shared_ptr<tao::pegtl::parse_tree::node> parse_file(const char* file) {
    tao::pegtl::file_input in(file);
    cx::parser::state is;
    return tao::pegtl::parse_tree::parse<cx::parser::grammar, cx::parser::selector, cx::parser::action>(in, is);
}

std::shared_ptr<tao::pegtl::parse_tree::node> parse_code(const char* code) {
    tao::pegtl::memory_input in(code, "");
    cx::parser::state is;
    return tao::pegtl::parse_tree::parse<cx::parser::grammar, cx::parser::selector, cx::parser::action>(in, is);
}

}