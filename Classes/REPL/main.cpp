#include <iostream>
#include "Parser/Parser.h"
#include "REPL.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        REPL repl;
        repl.start();
    }
    else
    {
        Environment env;
        std::cout << Parser().parseFile(argv[1], env)->show();
    }

    return 0;
}
