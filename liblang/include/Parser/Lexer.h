#pragma once
#include "Parser/Token.h"
#include <vector>
#include <string>

class Lexer
{
public:
    Lexer();

    bool tokenize(const std::string& s);

    void addSplittingSequence(const std::string& s);
    void removeSplittingSequence(const std::string& s);
    void clear();

    std::vector<Token> getTokens();
private:
    std::vector<Token> parsedTokens;
    std::vector<std::string> splittingSequences;
    std::vector<size_t> previousLineIndentationPoints;
    std::vector<size_t> currentLineIndentationPoints;
    size_t lastLineStartIndex = 0;
    size_t tokensSinceLastLine = 0;
};
