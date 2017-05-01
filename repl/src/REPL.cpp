#include "REPL.h"
#include <iostream>
#include <string>
#include <boost/algorithm/string.hpp>
#include "Expressions/Expression.h"
#include "Expressions/Identifier.h"
#include "Expressions/ExceptionParseError.h"
#include "Expressions/Void.h"
#include "System/Cast.h"
#include "AutoCompleter.h"

using namespace std;

REPL::REPL() :
    env(std::make_shared<DebugPrinter>(&std::cout))
{

}

void REPL::start()
{
    cout << "Welcome to the language, type -h to get help" << endl;


    while (true)
    {
//        try
//        {
            formCompletionNames();
            string instruction;

            instruction = input.read();
            boost::trim_right(instruction);

            if (shouldExit(instruction))
                break;
            else if (command(instruction))
                continue;
            else
            {
                Object expression;
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
                cout << endl << "evaluated: ";
                cout << evaluated->show();
                cout << endl;
                if (auto id = cast<Identifier>(env, evaluated))
                {
                    auto previousDebugPrint = env.getDebugPrint();
                    env.setDebugPrint(false);
                    cout << " : " << env.get(id->name)->show();
                    env.setDebugPrint(previousDebugPrint);
                }
                cout << endl;
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
    auto& completerNames = input.completerNames();
    completerNames = env.getAllNames();
    completerNames.push_back(":quit");
    completerNames.push_back(":clear");
    completerNames.push_back(":help");
    completerNames.push_back(":debugprint");
    completerNames.push_back(":load");
}

bool REPL::shouldExit(const std::string& s)
{
    return s == ":q" || s == ":quit";
}

void displayHelp()
{
    cout << "Help commands :" << endl <<
            ":q :quit      : quit" << endl <<
            ":clear        : clear environment" << endl <<
            ":h :help      : this message" << endl <<
            ":debugprint   : toggle debug print (disabled by default)" << endl <<
            ":load         : open and evaluate file" << endl;
}

bool REPL::command(const std::string& s)
{
    if (s == "")
        return true;
    if (s == ":load")
        loadFile("");//todo: add arg
    else if (s == ":clear")
        env.clear();
    else if (s == ":h" || s == ":help")
        displayHelp();
    else if (s == ":debugprint")
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
    auto evaluated = expression;
}

