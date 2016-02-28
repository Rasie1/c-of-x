#pragma once
#include <string>

enum class TokenTypeId
{
    Identifier = 0,
    Data,
    Tabulation,
    LineBreak,
    OpeningBrace,
    ClosingBrace,
    Comma,
    NoSpace
};

struct Token
{
    std::string s;
    TokenTypeId t;
    int tabulationAmount = 0;
};
