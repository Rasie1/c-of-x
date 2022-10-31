#pragma once
#include <memory>
#include <string>

namespace tao::pegtl::parse_tree { struct node; }

namespace cx::parser {

void trace(const char* file);
void print_graphviz(const char* file);
std::shared_ptr<tao::pegtl::parse_tree::node> parse_file(const char* file);
std::shared_ptr<tao::pegtl::parse_tree::node> parse_code(const char* code);

}