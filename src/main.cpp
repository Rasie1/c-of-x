#include <iostream>
#include <algorithm>
#include "parser.h"
#include "interpreter_test.h"

char* get_option(char** begin, char** end, const std::string& option) {
    char** itr = std::find(begin, end, option);
    if (itr != end && ++itr != end)
        return *itr;

    return nullptr;
}

bool option_exists(char** begin, char** end, const std::string& option) {
    return std::find(begin, end, option) != end;
}

int main(int argc, char** argv) 
{
    // if (pegtl::analyze<cx::grammar>() != 0) {
    //     std::cerr << "cycles without progress detected!\n";
    // }
    int verbosity = 0;
    if (option_exists(argv, argv + argc, "-logstate"))
        verbosity = 1;
    if (option_exists(argv, argv + argc, "-log"))
        verbosity = 2;
    if (option_exists(argv, argv + argc, "-test-interpreter")) {
        cx::interpreter::test();
    // if (option_exists(argv, argv + argc, "-exec")) {
    } else if (auto filename = get_option(argv, argv + argc, "-f")) {
        cx::eval_file(filename, verbosity);
    } else if (auto filename = get_option(argv, argv + argc, "-trace-file")) {
        cx::parser::trace(filename);
    } else if (auto filename = get_option(argv, argv + argc, "-graph")) {
        cx::parser::print_graphviz(filename, 1);
    } else if (auto filename = get_option(argv, argv + argc, "-graph-simple")) {
        cx::parser::print_graphviz(filename, 0);
    } else if (auto filename = get_option(argv, argv + argc, "-graph-all")) {
        cx::parser::print_graphviz(filename, 2);
    } else if (argc == 2 || argc == 3) {
        cx::eval(argv[1], verbosity);
    } else {
        std::cout << "./c-of-x [expr] [args]" << std::endl
                  << "         -f          - open file"               << std::endl
                //   << "         -exec       - omit eval result"        << std::endl
                  << "         -log        - print verbose debug log" << std::endl
                  << "         -logstate   - print general state"     << std::endl
                  << "         -trace-file - shows parser trace"      << std::endl
                  << "         -graph -graph-simple -graph-all"       << std::endl;
    }

    return 0;
}