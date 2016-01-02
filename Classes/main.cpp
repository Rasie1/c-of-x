#include <iostream>
#include "Parser.h"
#include "REPL.h"
#include "Expression.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        REPL repl;
        repl.start();
    }
    else
    {
        std::cout << Parser::parseFile(argv[1])->show();
    }

    return 0;
}
