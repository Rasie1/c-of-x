#pragma once
#include <memory>
#include "types.h"

namespace tao::pegtl::parse_tree { struct node; }

namespace cx::parser {

cx::expression build(const tao::pegtl::parse_tree::node& node);

}