#include "Parser/Lexer.h"
#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include <tuple>
#include <iostream>
#include <boost/optional.hpp>
#include <cctype>

using namespace std;
using namespace boost;

static constexpr bool isIdentifierCharacter(char c)
{
    return isalnum(c);
}

static constexpr bool isOperatorCharacter(char c)
{
    return  c == '\'' ||
            c == '\\' ||
            c == '_'  ||
            c == '+'  ||
            c == '-'  ||
            c == '*'  ||
            c == ':'  ||
            c == '#'  ||
            c == '~'  ||
            c == '$'  ||
            c == '>'  ||
            c == '<'  ||
            c == '*'  ||
            c == '@'  ||
            c == '!'  ||
            c == '%'  ||
            c == '&'  ||
            c == '`'  ||
            c == '|'  ||
            c == '?'  ||
            c == '/'  ||
            c == '^'  ||
            c == ';'  ||
            c == ','  ||
            c == '.'  ||
            c == '='
            ;
}

std::vector<Token> Lexer::getTokens()
{
    return parsedTokens;
}

static optional<size_t> shouldSplitWithSequence(const std::string& input,
                                                const vector<string>& splittingSequences)
{
    if (isOperatorCharacter(input[0]))
        return make_optional((size_t)1);
    for (auto& currentSplittingSequence : splittingSequences)
    {
        auto res = mismatch(currentSplittingSequence.begin(),
                            currentSplittingSequence.end(),
                            input.begin());

        if (res.first == input.end())
        {
            return make_optional(currentSplittingSequence.size());
        }
    }
    return none;
}

static Tokens::Tabulation calculateTabulation(const std::vector<size_t>& levels, size_t current)
{
    int tabSize = 1;
    for (auto& x : levels)
    {
        if (current + 2 >= x)
            break;
        tabSize++;
    }

    return Tokens::Tabulation{tabSize};
}

bool Lexer::tokenize(const std::string& input)
{
    bool parsingId = false;
    bool parsingString = false;
    bool parsingNumber = false;
    bool parsingIndentation = true;
    size_t currentTokenStart = 0;
    size_t currentCharacterIndex = 0;
    auto distanceFromLineBreak = [&](){ return currentCharacterIndex - lastLineStartIndex; };
    auto pushIndentation = [&](){
            if (!parsingIndentation)
                return;
            auto dist = distanceFromLineBreak();
            if (dist > 1)
                parsedTokens.push_back(calculateTabulation(previousLineIndentationPoints, dist));
            parsingIndentation = false;
    };
    auto pushId = [&](){
        parsedTokens.push_back(Tokens::Identifier{input.substr(currentTokenStart, currentCharacterIndex - currentTokenStart)});
        parsingId = false;
    };

    for (; currentCharacterIndex < input.size(); ++currentCharacterIndex)
    {
        char currentCharacter = input[currentCharacterIndex];
        bool shouldMove = false;
        size_t moveDistance = 0;

        if (parsingString)
        {
            if (currentCharacter == '\"')
            {
                parsedTokens.push_back(Tokens::StringData{
                    input.substr(1, currentCharacterIndex - 1)});
                if (currentCharacterIndex + 1 < input.size() &&
                    (input[currentCharacterIndex + 1] != ' ' &&
                     input[currentCharacterIndex + 1] != '\n'))
                    parsedTokens.push_back((Tokens::NoSpace()));

                shouldMove = true;
                moveDistance = 1;
            }
        }
        else if (parsingNumber)
        {
            if (!isdigit(currentCharacter))
            {
                parsedTokens.push_back(Tokens::IntegerData{stoi(input.substr(currentTokenStart, currentCharacterIndex - currentTokenStart))});
                parsedTokens.push_back(Tokens::NoSpace());
                shouldMove = true;
            }
            else if (currentCharacter == ' ' || currentCharacter == '\n')
            {
                parsedTokens.push_back(Tokens::IntegerData{stoi(input.substr(currentTokenStart, currentCharacterIndex - currentTokenStart))});
                shouldMove = true;
                moveDistance = 1;
            }
        }
        else if (currentCharacter == '(')
        {
            if (parsingIndentation)
                pushIndentation();
            if (parsingId)
            {
                pushId();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            parsedTokens.push_back(Tokens::Opening());
            shouldMove = true;
            moveDistance = 1;
        }
        else if (currentCharacter == ')')
        {
            if (parsingId)
                pushId();

            parsedTokens.push_back(Tokens::Closing());
            shouldMove = true;
            moveDistance = 1;
        }
        else if (currentCharacter == ' ')
        {
            if (parsingId)
                pushId();

            // if (!parsedTokens.empty() && (parsedTokens.back().which() != 1))
            //     parsingIndentation = true;
            shouldMove = true;
            moveDistance = 1;
        }
        else if (currentCharacter == '\n')
        {
            parsingIndentation = true;
            lastLineStartIndex = currentCharacterIndex;
            if (parsingId)
                pushId();

            if (!parsedTokens.empty() && (parsedTokens.back().which() != 1))
            {
                parsedTokens.push_back(Tokens::LineBreak());
                previousLineIndentationPoints = currentLineIndentationPoints;
                currentLineIndentationPoints.clear();
            }
            shouldMove = true;
            moveDistance = 1;
        }
        else if (currentCharacter == '\"')
        {
            currentTokenStart = currentCharacterIndex;
            if (parsingIndentation)
                pushIndentation();
            if (parsingId)
            {
                pushId();
                parsedTokens.push_back(Tokens::NoSpace());
                shouldMove = true;
            }
            else
            {
                currentTokenStart = currentCharacterIndex;
                parsingString = true;
            }
        }
        else if (isdigit(currentCharacter))
        {
            if (parsingIndentation)
                pushIndentation();
            if (!parsingId)
            {
                currentTokenStart = currentCharacterIndex;
                // no space case?
                parsingNumber = true;
            }
        }
        else if (optional<size_t> splitTokenSize = shouldSplitWithSequence(input.substr(currentCharacterIndex,
                                                                                        string::npos),
                                                                           splittingSequences))
        {
            currentTokenStart = currentCharacterIndex;
            if (parsingIndentation)
                pushIndentation();
            if (parsingId)
            {
                pushId();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            parsedTokens.push_back(Tokens::Identifier{input.substr(currentCharacterIndex, *splitTokenSize)});

            shouldMove = true;
            moveDistance = *splitTokenSize; //////////!!!!!!!!!!!!

            if (currentCharacterIndex + moveDistance < input.size() &&
                (input[currentCharacterIndex + moveDistance] != ' ' &&
                 input[currentCharacterIndex + moveDistance] != '\n'))
                parsedTokens.push_back((Tokens::NoSpace()));
        }
        else if (isIdentifierCharacter(currentCharacter))
        {
            if (parsingIndentation)
                pushIndentation();
            if (parsingId == false)
            {
                currentTokenStart = currentCharacterIndex;
                parsingId = true;
            }
        }

        if (shouldMove)
        {
            parsingId = false;
            parsingString = false;
            parsingNumber = false;
        }
    } // end while
    if (parsingId)
        pushId();
    if (parsingNumber)
        parsedTokens.push_back(Tokens::IntegerData{stoi(input.substr(currentTokenStart, currentCharacterIndex - currentTokenStart))});
    if (parsingString)
        return false; // todo: more information

    return true;
}

void Lexer::addSplittingSequence(const std::string& s)
{
    // todo: insert it so it won't be matched after prefix string
    splittingSequences.push_back(s);
}

void Lexer::removeSplittingSequence(const std::string& s)
{
}

void Lexer::clear()
{
    lastLineStartIndex = 0;
    parsedTokens.clear();
    previousLineIndentationPoints.clear();
    currentLineIndentationPoints.clear();
}
