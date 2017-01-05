#include <iostream>
#include <string>
#include <boost/program_options.hpp>
#include "Parser/Parser.h"
#include "REPL.h"

int main(int argc, char* argv[])
{
    try
    {
        boost::program_options::options_description desc("Allowed options");
        desc.add_options()
                ("help", "produce help message")
                ("no-preload", "prevent from loading preload.txt")
                ("debug-print", "enable debug output of calculations")
                ("file", boost::program_options::value<std::string>(), "evaluate code from file");
        boost::program_options::variables_map vm;
        boost::program_options::store(
                    boost::program_options::parse_command_line(argc, argv, desc),
                    vm);
        boost::program_options::notify(vm);

        if (vm.count("help"))
        {
            std::cout << desc << "\n";
            return 1;
        }

        if (vm.count("file"))
        {
            auto file = vm["file"].as<std::string>();
            Environment env;
            std::cout << Parser().parseFile(file, env)->show() << std::endl;
        }
        else
        {
            REPL repl;
            if (!(vm.count("no-preload")))
                  repl.loadFile("prelude.txt");
            if ((vm.count("debug-print")))
                  repl.toggleDebugPrint();
            repl.start();
        }
    }
    catch (std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        throw e;
        return -1;
    }
    catch (...)
    {
        std::cerr << "Unknown error!" << std::endl;
        throw;
        return -1;
    }

    return 0;
}
