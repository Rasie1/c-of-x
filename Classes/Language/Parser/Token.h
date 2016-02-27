#pragma once
#include <string>

enum TokenTypeId
{
    None = 0,
    Tabulation,
    LineBreak,
    Space
};

struct Token
{
    std::string s;
    TokenTypeId t;
    int tabulationAmount = 0;
};
