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
    cx::state is;
    tao::pegtl::complete_trace<cx::grammar, cx::action>(in, is);
}

void print_graphviz(const char* file) {
    tao::pegtl::file_input in(file);
    cx::state is;
    auto root = tao::pegtl::parse_tree::parse<cx::grammar, cx::selector, cx::action>(in, is);
    if (root)
        tao::pegtl::parse_tree::print_dot(std::cout, *root);
}

}