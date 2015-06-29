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
#include "EvalForce.h"

using namespace std;
namespace Language
{

ExpPtr Parser::parse(const string& s, Environment* env)
{
    size_t i = 0;
    return parse(s, i, s.size(), env);
}

inline bool isOperatorCharacter(char c)
{
    return
            c == '\'' ||
            c == '\\' ||
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
            c == ';'  ||
            c == ','  ||
            c == '.'  ||
            c == '='
            ;
}

inline bool isNameCharacter(char c)
{
    return
            c >= 'a' && c <= 'z' ||
            c >= 'A' && c <= 'Z' ||
            c >= '0' && c <= '9'
            ;
}


inline bool isExpressionEnd(char c)
{
    return c == ')'/* ||
           c == '}' ||
           c == ']'*/;
}

inline bool shouldSkipCharacter(char c)
{
    return c < '!';
}


ExpPtr Parser::parseName(const std::string& s,
                         size_t start,
                         size_t end,
                         Environment* env)
{
    if (s[start] >= '0' && s[start] <= '9')
    {
        auto ss = s.substr(start, end - start);

        long long number;
        try
        {
            number = stoll(ss);
        }
        catch (std::exception&)
        {
            return std::make_shared<ParseError>();
        }

        return std::make_shared<Integer>(stoll(ss));
    }
    if (isNameCharacter(s[start]))
    {
        auto ss = s.substr(start, end - start);
/*
        auto variable = new Variable(ss);
        auto e = env->get(variable->pattern());
        if (e)
            */
        //auto op = new PatternOperator(

        return std::make_shared<Variable>(ss);
    }
    if (isOperatorCharacter(s[start]))
    {
        auto ss = s.substr(start, end - start);

        return std::make_shared<Variable>(ss);
    }
}

void makeOperation(std::stack<ExpPtr>& operatorStack,
                   std::deque<ExpPtr>& q,
                   Environment* env)
{
    ExpPtr left;
    ExpPtr right;
    ExpPtr top = operatorStack.top();
    //Operator* op = (Operator*)(top);
    std::shared_ptr<Operator> op = std::dynamic_pointer_cast<Operator>(top->eval(env)); // eval in parser?!
    operatorStack.pop();

    right = q.back();
    q.pop_back();
    left  = q.back();
    q.pop_back();

    // "Preprocessor" handling (#)
    auto variable = std::dynamic_pointer_cast<Variable>(left);
    bool evalForce = false;
    if (variable)
        evalForce = variable->name == "#";

    if (evalForce)
        q.push_back(right->eval(env));
    else
        q.push_back(std::make_shared<Operation>(op, left, right));
}

ExpPtr Parser::parse(const std::string& s,
                     size_t& i,
                     size_t n,
                     Environment* env)
{
    ExpPtr ret = nullptr;

    std::stack<ExpPtr> operatorStack;
    std::deque<ExpPtr> q;
    bool applicationFlag = false;

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
            ExpPtr e;
            if (isNameCharacter(currentCharacter))
            {
                // Find name end
                size_t start = i;
                ++i;
                while (i < n && isNameCharacter(s[i]))
                    ++i;

                e = parseName(s, start, i, env);
            }
            else if (isOperatorCharacter(s[i]))
            {
                e = parseName(s, i, i + 1, env);
                ++i;
            }
            else if (currentCharacter == '(')
                e = parse(s, ++i, n, env);
            else if (s[i] == '\"')
            {
                size_t start = i;
                ++i;
                while (i < n && s[i] != '\"')
                    ++i;
                auto ss = s.substr(start + 1, i - start - 1);
                ++i;
                e = std::make_shared<String>(ss);
            }

            /* SHUNTING-YARD
             * f token = name     => q.push token
             *         | function => s.push token
             *         | operator => 1. while s.top is operator
             *                       && (token is left  associative
             *                       && token <= s.top
             *                       || token is right associative
             *                       && token <  s.top)
             *                       { q.push(s.pop) }
             *                       2. s.push token
             * (modified version is used, this ^ is just for a reference
             */

            if (e->isOperator(env))
            {
                applicationFlag = false;

                while (!operatorStack.empty()
                       && env->compareOperators(e, operatorStack.top()))
                    makeOperation(operatorStack, q, env);

                operatorStack.push(e);
            }
            else
            {
                q.push_back(e);
                if (applicationFlag)
                {
                    operatorStack.push(std::make_shared<Application>());
                    makeOperation(operatorStack, q, env);
                }

                applicationFlag = true;
            }
        }
    }

    while (!operatorStack.empty())
        makeOperation(operatorStack, q, env);

    if (!q.empty())
        ret = q.front();
    if (!ret)
        ret = std::make_shared<Void>();

    return ret;
}


}
