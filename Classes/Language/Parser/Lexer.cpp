#include "Parser/Lexer.h"
#include <algorithm>
#include <boost/algorithm/string/predicate.hpp>
#include <boost/lexical_cast.hpp>

std::vector<std::string> Lexer::defaultSplittingSequences()
{
    std::vector<std::string> ret;
    ret.push_back("\'");
    ret.push_back("\"");
    ret.push_back("\\");
    ret.push_back("/");
    ret.push_back("+");
    ret.push_back("-");
    ret.push_back("*");
    ret.push_back(":");
    ret.push_back("#");
    ret.push_back("~");
    ret.push_back("$");
    ret.push_back(">");
    ret.push_back("<");
    ret.push_back("@");
    ret.push_back("=");
    ret.push_back("`");
    ret.push_back("$");
    ret.push_back("!");
    ret.push_back("%");
    ret.push_back("&");
    ret.push_back("|");
    ret.push_back("?");
    ret.push_back(",");
    ret.push_back(";");
    ret.push_back("^");
    ret.push_back(".");

    ret.push_back("(");
    ret.push_back(")");
    ret.push_back("{");
    ret.push_back("}");
    ret.push_back("[");
    ret.push_back("]");

    ret.push_back("->");
    ret.push_back("<-");
    ret.push_back("=>");
    ret.push_back("<=");
    ret.push_back(">=");
    ret.push_back("<=>");
    ret.push_back("++");
    ret.push_back("--");
    return ret;
}

static constexpr bool isNameCharacter(char c)
{
    return
            c >= 'a' && c <= 'z' ||
            c >= 'A' && c <= 'Z' ||
            c >= '0' && c <= '9'
            ;
}

static constexpr bool shouldSkipCharacter(char c)
{
    return c < '!';
}

bool Lexer::isSplittingPrefix(const std::string &s) const
{
    for (auto i = splittingSequences.rbegin(); i != splittingSequences.rend(); ++i)
    {
        //std::mismatch(i->begin(), i->end(), s);
    }
}

std::vector<Token> Lexer::split(const std::string &s) const
{
    throw "todo";
    std::vector<Token> ret;
    int i = 0;
    int n = s.size();

    while (i < n)
    {
        if (shouldSkipCharacter(s[i]))
        {
            ++i;
            continue;
        }

    }

    return ret;
}
