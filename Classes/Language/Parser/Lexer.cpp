#include "Parser/Lexer.h"
#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>
#include <iterator>
#include <tuple>

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
    for (size_t currentCharacterIndex = 0;
         currentCharacterIndex < input.size();
         ++currentCharacterIndex)
    {
        bool shouldSplit;
        size_t splitTokenSize;
        tie(shouldSplit, splitTokenSize) = shouldSplitWithSequence(input.substr(currentCharacterIndex,
                                                                                string::npos),
                                                                   splittingSequences);
        if (shouldSplit)
        {
            parsedTokens.push_back(Token{TokenTypeId::Identifier,
                                         input.substr(0, splitTokenSize)});
            parsedTokens.push_back(Token{TokenTypeId::Identifier,
                                         input.substr(splitTokenSize, string::npos)});

            tokenize(input.substr(currentCharacterIndex + splitTokenSize,
                                  string::npos));
            return;
        }
    }
}

void Lexer::addSplittingSequence(const std::string& s)
{

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
