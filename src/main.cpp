#include <iostream>
#include "parser.h"
#include "interpreter_test.h"

int main(int argc, char** argv) 
{
    // if (pegtl::analyze<cx::grammar>() != 0) {
    //     std::cerr << "cycles without progress detected!\n";
    // }
    if (argc == 1) {
        cx::interpreter::test();
    } else if (argc == 2) {
        cx::eval(argv[1]);
    } else if (argc == 3) {
        cx::parser::trace(argv[1]);
    } else {
        cx::parser::print_graphviz(argv[1]);
    }

    return 0;
}