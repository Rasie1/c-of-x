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

Lexer::Lexer()
{
    // TODO: make it dynamic
    addSplittingSequence("==>");
    addSplittingSequence(":-");
    addSplittingSequence("-:");
    addSplittingSequence("->");
    addSplittingSequence("<-");
    addSplittingSequence("=>");
    addSplittingSequence("<=");
    addSplittingSequence(">=");
    addSplittingSequence("=<");
    addSplittingSequence("\'");
    addSplittingSequence("\\");
    addSplittingSequence("_");
    addSplittingSequence("+");
    addSplittingSequence("-");
    addSplittingSequence("*");
    addSplittingSequence(":");
    addSplittingSequence("#");
    addSplittingSequence("~");
    addSplittingSequence("$");
    addSplittingSequence(">");
    addSplittingSequence("<");
    addSplittingSequence("*");
    addSplittingSequence("@");
    addSplittingSequence("!");
    addSplittingSequence("%");
    addSplittingSequence("&");
    addSplittingSequence("`");
    addSplittingSequence("|");
    addSplittingSequence("?");
    addSplittingSequence("/");
    addSplittingSequence("^");
    addSplittingSequence(";");
    addSplittingSequence(",");
    addSplittingSequence(".");
    addSplittingSequence("=");
}

std::vector<Token> Lexer::getTokens()
{
    return parsedTokens;
}

static optional<size_t> shouldSplitWithSequence(const std::string& input,
                                                const vector<string>& splittingSequences)
{
    for (auto& currentSplittingSequence : splittingSequences)
    {
        auto res = mismatch(currentSplittingSequence.begin(),
                            currentSplittingSequence.end(),
                            input.begin());

        if (res.first == currentSplittingSequence.end())
        {
            return make_optional(currentSplittingSequence.size());
        }
    }
    return none;
}

static Tokens::Tabulation calculateTabulation(const std::vector<size_t>& levels, size_t current)
{
    size_t tabSize = 1;
    for (auto& x : levels)
    {
        if (current < x)
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
    auto pushNumber = [&](){
        parsedTokens.push_back(Tokens::IntegerData{stoi(input.substr(currentTokenStart, currentCharacterIndex - currentTokenStart))});
        parsingNumber = false;
    };

    for (; currentCharacterIndex < input.size(); ++currentCharacterIndex)
    {
        char currentCharacter = input[currentCharacterIndex];

        if (parsingString)
        {
            if (currentCharacter == '\"')
            {
                parsedTokens.push_back(Tokens::StringData{
                    input.substr(currentTokenStart + 1, currentCharacterIndex - currentTokenStart - 1)});
                if (currentCharacterIndex + 1 < input.size() &&
                    (input[currentCharacterIndex + 1] != ' ' &&
                     input[currentCharacterIndex + 1] != '\n'))
                    parsedTokens.push_back((Tokens::NoSpace()));

                parsingString = false;
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
            if (parsingNumber)
            {
                pushNumber();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            parsedTokens.push_back(Tokens::Opening());
        }
        else if (currentCharacter == ')')
        {
            if (parsingId)
                pushId();
            if (parsingNumber)
                pushNumber();

            parsedTokens.push_back(Tokens::Closing());
        }
        else if (currentCharacter == ' ')
        {
            if (parsingId || parsingNumber)
                currentLineIndentationPoints.push_back(currentCharacterIndex);
            if (parsingId)
                pushId();
            if (parsingNumber)
                pushNumber();
        }
        else if (currentCharacter == '\n')
        {
            parsingIndentation = true;
            lastLineStartIndex = currentCharacterIndex;
            if (parsingId)
                pushId();
            if (parsingNumber)
                pushNumber();

            if (!parsedTokens.empty() && (parsedTokens.back().which() != 1))
            {
                parsedTokens.push_back(Tokens::LineBreak());
                previousLineIndentationPoints = currentLineIndentationPoints;
                currentLineIndentationPoints.clear();
            }
        }
        else if (currentCharacter == '\"')
        {
            if (parsingIndentation)
                pushIndentation();
            if (parsingId)
            {
                pushId();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            if (parsingNumber)
            {
                pushNumber();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            {
                currentTokenStart = currentCharacterIndex;
                parsingString = true;
            }
        }
        else if (isdigit(currentCharacter))
        {
            if (parsingIndentation)
                pushIndentation();
            if (!parsingId && !parsingNumber)
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
            if (parsingIndentation)
                pushIndentation();
            if (parsingId)
            {
                pushId();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            if (parsingNumber)
            {
                pushNumber();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            parsedTokens.push_back(Tokens::Identifier{input.substr(currentCharacterIndex, *splitTokenSize)});
            currentTokenStart = currentCharacterIndex;

            auto moveDistance = *splitTokenSize;

            if (currentCharacterIndex + moveDistance < input.size() &&
                (input[currentCharacterIndex + moveDistance] != ' ' &&
                 input[currentCharacterIndex + moveDistance] != '\n'))
                parsedTokens.push_back((Tokens::NoSpace()));

            currentCharacterIndex += moveDistance - 1;
        }
        else if (isIdentifierCharacter(currentCharacter))
        {
            if (parsingIndentation)
                pushIndentation();
            if (parsingNumber)
            {
                pushNumber();
                parsedTokens.push_back(Tokens::NoSpace());
            }
            if (!parsingId && !parsingNumber)
            {
                currentTokenStart = currentCharacterIndex;
                parsingId = true;
            }
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
    // TODO: insert it so it won't be matched after prefix string
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
