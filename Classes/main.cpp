#include <iostream>
#include "Parser.h"
#include "REPL.h"
#include "Expression.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        Language::REPL repl;
        repl.start();
    }
    else
    {
        std::cout << Language::Parser::parseFile(argv[1])->show();
    }

    return 0;
}
