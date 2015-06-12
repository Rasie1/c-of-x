#include <iostream>
#include "REPL.h"

int main(int argc, char* argv[])
{
    if (argc == 1)
    {
        Language::REPL repl;
        repl.start();
    }
    else
        std::cout << "Command line arguments are not yet implemented" << std::endl;

    return 0;
}
