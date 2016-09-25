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

bool Lexer::tokenize(const std::string& input)
{
    bool parsingId = false;
    bool parsingString = false;
    bool parsingNumber = false;
    for (size_t currentCharacterIndex = 0;
         currentCharacterIndex < input.size();
         ++currentCharacterIndex)
    {
        char currentCharacter = input[currentCharacterIndex];
        bool shouldMove = false;
        size_t moveDistance = 0;
//        cout << "{" << currentCharacter << "}" << endl;

        if (parsingString)
        {
            if (currentCharacter == '\"')
            {

                parsedTokens.push_back(Tokens::StringData{input.substr(1,
                                                                       currentCharacterIndex - 1)});
                if (currentCharacterIndex + 1 < input.size() &&
                    (input[currentCharacterIndex + 1] != ' '  ||
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
                parsedTokens.push_back(Tokens::IntegerData{stoi(input.substr(0, currentCharacterIndex))});
                parsedTokens.push_back(Tokens::NoSpace());
                shouldMove = true;
            }
            else if (currentCharacter == ' ' || currentCharacter == '\n')
            {
                parsedTokens.push_back(Tokens::IntegerData{stoi(input.substr(0, currentCharacterIndex))});
                shouldMove = true;
                moveDistance = 1;
            }
        }
        else if (currentCharacter == '(')
        {
            if (parsingId)
            {
                parsedTokens.push_back(Tokens::Identifier{input.substr(0, currentCharacterIndex)});
                parsedTokens.push_back(Tokens::NoSpace());
            }
            parsedTokens.push_back(Tokens::Opening());
            shouldMove = true;
            moveDistance = 1;
        }
        else if (currentCharacter == ')')
        {
            if (parsingId)
            {
                parsedTokens.push_back(Tokens::Identifier{input.substr(0, currentCharacterIndex)});
            }
            parsedTokens.push_back(Tokens::Closing());
            shouldMove = true;
            moveDistance = 1;
            //todo : NoSpace case
        }
        else if (currentCharacter == ' ')
        {
            if (parsingId)
                parsedTokens.push_back(Tokens::Identifier{input.substr(0, currentCharacterIndex)});
            shouldMove = true;
            moveDistance = 1;
        }
        else if (currentCharacter == '\n')
        {
            if (parsingId)
                parsedTokens.push_back(Tokens::Identifier{input.substr(0, currentCharacterIndex)});
            if (true)
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
            if (parsingId)
            {
                parsedTokens.push_back(Tokens::Identifier{input.substr(0, currentCharacterIndex)});
                parsedTokens.push_back(Tokens::NoSpace());
                shouldMove = true;
            }
            else
            {
                parsingString = true;
            }
        }
        else if (isdigit(currentCharacter))
        {
            if (!parsingId)
            {
                // no space case?
                parsingNumber = true;
            }
        }
        else if (optional<size_t> splitTokenSize = shouldSplitWithSequence(input.substr(currentCharacterIndex,
                                                                                        string::npos),
                                                                           splittingSequences))
        {
            if (parsingId)
            {
                parsedTokens.push_back(Tokens::Identifier{input.substr(0, currentCharacterIndex)});
                parsedTokens.push_back(Tokens::NoSpace());
            }
            parsedTokens.push_back(Tokens::Identifier{input.substr(currentCharacterIndex, *splitTokenSize)});
            // todo: no space case
            shouldMove = true;
            moveDistance = *splitTokenSize;
        }
        else if (isIdentifierCharacter(currentCharacter))
        {
            if (parsingId == false)
            {
                parsingId = true;
            }
        }

        if (shouldMove)
        {
            return tokenize(input.substr(currentCharacterIndex + moveDistance,
                                         string::npos));
        }
    } // end while
    if (parsingId)
        parsedTokens.push_back(Tokens::Identifier{input});
    if (parsingNumber)
        parsedTokens.push_back(Tokens::IntegerData{stoi(input)});
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
    distanceFromLineBreak = 0;
    parsedTokens.clear();
    previousLineIndentationPoints.clear();
    currentLineIndentationPoints.clear();
}
