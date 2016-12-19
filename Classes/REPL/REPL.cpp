#include "REPL.h"
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "Expression.h"
#include "ExceptionParseError.h"
#include "Void.h"
#include "AutoCompleter.h"

using namespace std;

REPL::REPL() :
    env(make_ptr<DebugPrinter>(&std::cout))
{

}

void REPL::start()
{
    cout << "Welcome to c(x), type -h to get help" << endl;


    while (true)
    {
//        try
//        {
            formCompletionNames();
            string instruction;

            instruction = completer.readline();
            boost::trim_right(instruction);

            if (shouldExit(instruction))
                break;
            else if (command(instruction))
                continue;
            else
            {
                ExpPtr expression;
                try
                {
                    expression = parser.parse(instruction, env);
                }
                catch(ExceptionParseError& e)
                {
                    cout << "Parse error: " << e.s << endl;
                    continue;
                }

                auto evaluated = expression->eval(env);
                cout << endl << "evaluated: " << evaluated->show() << endl;
            }
//        }
//        catch (std::exception& e)
//        {
//            std::cerr << "Error: " << e.what() << std::endl;
//        }
//        catch (...)
//        {
//            std::cerr << "Unknown error!" << std::endl;
//        }
    }

    std::cout << "Quitting REPL" << endl;
}

void REPL::formCompletionNames()
{
    completer.names = env.getKeys();
    completer.names.push_back("-quit");
    completer.names.push_back("-clear");
    completer.names.push_back("-help");
    completer.names.push_back("-debugprint");
    completer.names.push_back("-load");
}

bool REPL::shouldExit(const std::string& s)
{
    return s == "-q" || s == "-quit";
}

void displayHelp()
{
    cout << "Help commands :" << endl <<
            "-q -quit      : quit" << endl <<
            "-clear        : clear environment" << endl <<
            "-h -help      : this message" << endl <<
            "-debugprint   : toggle debug print (disabled by default)" << endl <<
            "-load         : open and evaluate file" << endl;
}

bool REPL::command(const std::string& s)
{
    if (s == "")
        return true;
    if (s == "-load")
        loadFile("");//todo: add arg
    else if (s == "-clear")
        env.clear();
    else if (s == "-h" || s == "-help")
        displayHelp();
    else if (s == "-debugprint")
        toggleDebugPrint();

    return false;
}

void REPL::toggleDebugPrint()
{
    env.toggleDebugPrint();
}

void REPL::loadFile(const std::string& s)
{
    cout << "Loading file: " << s << endl;
    auto expression = parser.parseFile(s, env);
    auto evaluated = expression->eval(env);
}

