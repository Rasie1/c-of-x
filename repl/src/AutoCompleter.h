#pragma once
#include <readline/readline.h>
#include <readline/history.h>
#include <string>
#include <vector>
class AutoCompleter
{
public:
    AutoCompleter();

    // Since libreadline is based on global state, it may lead to problems
    // if you use multiple AutoCompleters. Switch them with this function
    void switchContext();

    std::string readline();
    std::vector<std::string> names;

private:
};
