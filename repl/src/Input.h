#pragma once
#include <string>
#include <vector>
#include "AutoCompleter.h"

class Input
{
public:
    Input();

    std::string read();
    std::vector<std::string>& completerNames();

private:
    AutoCompleter completer;
};
