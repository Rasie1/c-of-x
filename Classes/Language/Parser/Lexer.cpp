#include "Parser/Lexer.h"
#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include <tuple>
#include <iostream>

using namespace std;

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

static tuple<bool, size_t> shouldSplitWithSequence(const std::string& input,
                                    const vector<string>& splittingSequences)
{
    if (isOperatorCharacter(input[0]))
        return make_tuple(true, 1);
    for (auto& currentSplittingSequence : splittingSequences)
    {
        auto res = mismatch(currentSplittingSequence.begin(),
                            currentSplittingSequence.end(),
                            input.begin());

        if (res.first == input.end())
        {
            return make_tuple(true, currentSplittingSequence.size());
        }
    }

    return make_tuple(false, 0);
}

void Lexer::tokenize(const std::string& input)
{
    bool parsingId = false;
    for (size_t currentCharacterIndex = 0;
         currentCharacterIndex < input.size();
         ++currentCharacterIndex)
    {
        char currentCharacter = input[currentCharacterIndex];
        bool shouldMove = false;
        size_t moveDistance = 0;

        if (currentCharacter == ' ')
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
        else
        {
            parsingId = true;

            { // Split with splittingSequences
                bool shouldSplit;
                size_t splitTokenSize;
                tie(shouldSplit, splitTokenSize) = shouldSplitWithSequence(input.substr(currentCharacterIndex,
                                                                                        string::npos),
                                                                           splittingSequences);
                if (shouldSplit)
                {
                    parsedTokens.push_back(Tokens::Identifier{input.substr(0, currentCharacterIndex)});
                    parsedTokens.push_back(Tokens::Identifier{input.substr(currentCharacterIndex, splitTokenSize)});
                    shouldMove = true;
                    moveDistance = splitTokenSize;
                }
            }
        }
        if (shouldMove)
        {
            tokenize(input.substr(currentCharacterIndex + moveDistance,
                                  string::npos));
            return;
        }
    }
    if (parsingId)
        parsedTokens.push_back(Tokens::Identifier{input});
}

void Lexer::addSplittingSequence(const std::string& s)
{
    // todo: insert it so it won't be matched after prefix string
    splittingSequences.push_back(s);
}

void Lexer::removeSplittingSequence(const std::string& s)
{
}

//static constexpr bool isNameCharacter(char c)
//{
//    return
//            (c >= 'a' && c <= 'z') ||
//            (c >= 'A' && c <= 'Z') ||
//            (c >= '0' && c <= '9')
//            ;
//}

//static constexpr bool shouldSkipCharacter(char c)
//{
//    return c < '!';
//}

//bool Lexer::isSplittingPrefix(const std::string &s) const
//{
//    for (auto i = splittingSequences.rbegin(); i != splittingSequences.rend(); ++i)
//    {
//        //std::mismatch(i->begin(), i->end(), s);
//    }
//    return false;
//}

//std::vector<Token> Lexer::split(const std::string &s) const
//{
//    throw "todo";
////    std::vector<Token> ret;
////    int i = 0;
////    int n = s.size();

////    while (i < n)
////    {
////        if (shouldSkipCharacter(s[i]))
////        {
////            ++i;
////            continue;
////        }

////    }

//    return ret;
//}
