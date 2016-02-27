#pragma once
#include <string>

struct Token
{
    std::string name;
    bool attachedToL = false;
    bool attachedToR = false;
};
