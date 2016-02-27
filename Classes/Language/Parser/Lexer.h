#pragma once
#include "Token.h"
#include <vector>

class Lexer
{
public:
    std::vector<Token> split(const std::string& s) const;

    std::vector<std::string> splittingSequences;
};
