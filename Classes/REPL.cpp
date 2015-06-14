#include "REPL.h"
#include <iostream>
#include <string>
#include "Expression.h"
#include "ExceptionParseError.h"
#include "Void.h"

using namespace std;

namespace Language
{

void REPL::start()
{
    env = Environment::create();
    cout << "Welcome to Language REPL, type -h to get help" << endl;

    while (true)
    {
        string instruction;
        cout << ">";

        getline(cin, instruction);

        if (shouldExit(instruction))
            break;
        else if (command(instruction))
            continue;
        else
        {
            Expression* expression;
            try
            {
                expression = parser.parse(instruction);
            }
            catch(ExceptionParseError& e)
            {
                cout << "Parse error: " << e.s << endl;
                continue;
            }

            //cout << expression->toString() << " =" << endl;
            DEBUG_LINEBREAK;
            auto evaluated = expression->eval(env);
            //cout << evaluated->toString() << endl;
            DEBUG_LINEBREAK;
            // Something is wrong and makes assigned variables to
            // be associated with themselves in environment
            // so, almost all the memory is leaked while this line
            // is commented
            //delete evaluated;
            // TODO: fix
        }
    }

    cout << "Quitting REPL" << endl;
    env->clear();
    delete env;
}

bool REPL::shouldExit(const std::string& s)
{
    return s == "-q" || s == "-quit";
}

void displayHelp()
{
    cout << "Help commands :" << endl <<
            "-q -quit      : quit" << endl <<
            "-clean        : remove unused variables from env" << endl <<
            "-clear        : clear environment" << endl <<
            "-h -help      : this message" << endl;
}

bool REPL::command(const std::string& s)
{
    if (s == "")
        return 1;
    else if (s == "-clean")
        env->clean();
    else if (s == "-clear")
        env->clear();
    else if (s == "-h" || s == "-help")
        displayHelp();
    else
        return 0;
}

}
