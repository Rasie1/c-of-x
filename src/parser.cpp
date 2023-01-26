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
        bracket_expr,
        curly_brace_expr,
        numeral, digits, literal_string,

        operation_apply
    >,
    tao::pegtl::parse_tree::remove_content::on<
    >
>;

template<class Rule>
using selector = tao::pegtl::parse_tree::selector<
    Rule,
    tao::pegtl::parse_tree::store_content::on<
        name,
        definition,
        identifier,
        bracket_expr,
        curly_brace_expr,
        numeral, digits, literal_string,

        operation_apply,
        operators_9,
        operators_8,
        operators_7,
        operators_3,
        operators_2,
        operators_1,
        operators_0,

        tao::pegtl::star_must<cx::parser::operators_0, cx::parser::seps, cx::parser::expr_1, cx::parser::seps>,
        tao::pegtl::star_must<cx::parser::operators_1, cx::parser::seps, cx::parser::expr_2, cx::parser::seps>,
        tao::pegtl::star_must<cx::parser::operators_2, cx::parser::seps, cx::parser::expr_3, cx::parser::seps>,
        tao::pegtl::star_must<cx::parser::operators_3, cx::parser::seps, cx::parser::expr_7, cx::parser::seps>,
        tao::pegtl::star_must<cx::parser::operators_7, cx::parser::seps, cx::parser::expr_8, cx::parser::seps>,
        tao::pegtl::star_must<cx::parser::operators_8, cx::parser::seps, cx::parser::expr_9, cx::parser::seps>,
        tao::pegtl::star_must<cx::parser::operators_9, cx::parser::seps, cx::parser::operation_apply, cx::parser::seps>
    >,
    tao::pegtl::parse_tree::remove_content::on<
    >
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
    // todo: output tree references something from the file
    auto in = new tao::pegtl::file_input(file);
    // tao::pegtl::file_input in(file);
    cx::parser::state is;
    auto root = tao::pegtl::parse_tree::parse<
        cx::parser::grammar, 
        cx::parser::selector, 
        cx::parser::action>(*in, is);
    return root;
}

std::shared_ptr<tao::pegtl::parse_tree::node> parse_code(const char* code) {
    tao::pegtl::memory_input in(code, "");
    cx::parser::state is;
    auto root = tao::pegtl::parse_tree::parse<
        cx::parser::grammar, 
        cx::parser::selector, 
        cx::parser::action>(in, is);
    return root;
}

}