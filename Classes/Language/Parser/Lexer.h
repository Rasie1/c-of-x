#pragma once
#include "Parser/Token.h"
#include <vector>

class Lexer
{
public:
    std::vector<Token> split(const std::string& s) const;

    std::vector<std::string> splittingSequences;

    bool isSplittingPrefix(const std::string& s) const;

    static std::vector<std::string> defaultSplittingSequences();
};
