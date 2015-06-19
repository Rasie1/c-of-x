#include "Parser.h"
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include "Integer.h"
#include "Application.h"
#include "ExceptionParseError.h"
#include "Void.h"
#include "ParseError.h"
#include "Variable.h"
#include "String.h"
#include "Operation.h"
#include "Environment.h"
#include "PatternOperator.h"

using namespace std;
namespace Language
{

Expression* Parser::parse(const string& s, Environment* env)
{
    size_t i = 0;
    return parse(s, i, s.size(), env);
}

inline bool notSpecialCharacter(char c)
{
    return c >= 'a' && c <= 'z' ||
           c >= 'A' && c <= 'Z' ||
           c >= '0' && c <= '9' ||
           c == '\'' ||
           c == '\\' ||
           c == '\"' ||
           c == '_'  ||
           c == '+'  ||
           c == '-'  ||
           c == '*'  ||
           c == ':'  ||
           c == '#'  ||
           c == '~'  ||
           c == '$'  ||
           c == '*'  ||
           c == '@'  ||
           c == '%'  ||
           c == '&'  ||
           c == '`'  ||
           c == '|'  ||
           c == '?'  ||
           c == '/'  ||
           c == '^'  ||
           c == '='  ;
}

inline bool isExpressionEnd(char c)
{
    return c == ';' ||
           c == ',' ||
           c == ')' ||
           c == '}' ||
           c == '.' ||
           c == ']';
}

inline bool shouldSkipCharacter(char c)
{
    return c < '!';
}


Expression* Parser::parseName(const std::string& s,
                              size_t& i,
                              size_t n,
                              Environment* env)
{
    if (s[i] == '\"')
    {
        int start = i;
        ++i;
        while (i < n && s[i] != '\"')
            ++i;
        auto ss = s.substr(start + 1, i - start - 1);
        ++i;
        return new String(ss);
    }
    if (s[i] >= '0' && s[i] <= '9')
    {
        int start = i;
        ++i;
        while (i < n && notSpecialCharacter(s[i]))
            ++i;

        auto ss = s.substr(start, i - start);

        long long number;
        try
        {
            number = stoll(ss);
        }
        catch (std::exception&)
        {
            return new ParseError();
        }

        return new Integer(stoll(ss));
    }
    if (notSpecialCharacter(s[i]))
    {
        int start = i;
        ++i;
        while (i < n && notSpecialCharacter(s[i]))
            ++i;

        auto ss = s.substr(start, i - start);

        //Operator* op = env->get(new PatternOperator(

        return new Variable(ss);
    }
}

Expression* Parser::parse(const std::string& s,
                          size_t& i,
                          size_t n,
                          Environment* env)
{
    Expression* ret = nullptr;

    std::stack<Expression*> operatorStack;


    while (i < n)
    {
        char currentCharacter = s[i];

        if (shouldSkipCharacter(currentCharacter))
            ++i;
        else if (isExpressionEnd(currentCharacter))
        {
            ++i;
            break;
        }
        else
        {
            Expression* e;
            if (notSpecialCharacter(currentCharacter))
                e = parseName(s, i, n, env);
            else if (currentCharacter == '(')
                e = parse(s, ++i, n, env);
            /*
            if (ret != nullptr)
            {
                if (e->isOperator(env))
                {
                    while (operatorStack.top()->isOperator())
                    {

                    }

                    ret = new Operation(new Application(),
                                        ret,
                                        e);
                }
                else
                    operatorStack.push(e);
            }
            else
                ret = e;
            */


            if (ret != nullptr)
                ret = new Operation(new Application(),
                                    ret,
                                    e);
            else
                ret = e;
        }
    }
    if (!ret)
        ret = new Void();

    return ret;
}


}
