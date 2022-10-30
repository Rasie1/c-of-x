#include <iostream>
#include "parser.h"

int main(int argc, char** argv) 
{
    if (argc == 3) {
        cx::parser::trace(argv[1]);
    } else if (argc == 2) {
        throw std::invalid_argument("no input");
    } else {
        cx::parser::print_graphviz(argv[1]);
    }

    return 0;
}