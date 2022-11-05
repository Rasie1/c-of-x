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
        cx::eval_file(argv[1]);
    } else if (argc == 4) {
        cx::parser::trace(argv[1]);
    } else {
        cx::parser::print_graphviz(argv[1], static_cast<int>(argc - 5));
    }

    return 0;
}