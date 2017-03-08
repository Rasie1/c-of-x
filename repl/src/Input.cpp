#include "Input.h"

#include <stdio.h>
#include <sys/types.h>
#include <sys/file.h>
#include <sys/stat.h>
#include <sys/errno.h>
#include <iostream>
#include <sstream>
#include <readline/readline.h>
#include <readline/history.h>

using namespace std;


Input::Input()
{
}

std::vector<std::string>& Input::completerNames()
{
    return completer.names;
}

std::string Input::read()
{
    stringstream code;

    if (multilineInput) 
    {
        bool seenLineBreak = false; 

        auto getLineStartString = [&seenLineBreak]()
        {
            if (seenLineBreak)
                return " > ";
            else
                return "&> ";
        };

        while (true)
        {
            std::string line = ::readline(getLineStartString());
            if (line == "")
            {
                break;
            }
            else
            {
                code << line << endl;
                seenLineBreak = true;
            }
        }
    }
    else
        code << ::readline("&> ");

    auto str = code.str();

    add_history(str.c_str());

    return str;
}

