#include "REPL.h"
#include <iostream>
#include <string>
#include "Expression.h"
#include "ExceptionParseError.h"
#include "Void.h"

using namespace std;

void REPL::start()
{
    cout << "Welcome to c(x), type -h to get help" << endl;

    while (true)
    {
        string instruction;
        cout << "&>";

        getline(cin, instruction);

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

            //cout << expression->show() << " =" << endl;
            DEBUG_LINEBREAK;
            auto evaluated = expression->eval(env);
            cout << evaluated->show() << endl;
            DEBUG_LINEBREAK;
        }
    }

    cout << "Quitting REPL" << endl;
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

    return false;
}

void REPL::loadFile(const std::string& s)
{
    auto expression = parser.parseFile(s, env);
    auto evaluated = expression->eval(env);
}
