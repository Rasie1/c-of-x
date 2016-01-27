#include "Parser.h"
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include "Integer.h"
#include "Application.h"
#include "ExceptionParseError.h"
#include "Void.h"
#include "ParseError.h"
#include "Identifier.h"
#include "String.h"
#include "Operation.h"
#include "Environment.h"
#include "EvalForce.h"
#include "Lambda.h"

using namespace std;

ExpPtr Parser::parse(const string& s, Environment* env)
{
    size_t i = 0;
    return parse(s, i, s.size(), env);
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

static constexpr bool isNameCharacter(char c)
{
    return
            c >= 'a' && c <= 'z' ||
            c >= 'A' && c <= 'Z' ||
            c >= '0' && c <= '9'
            ;
}


static constexpr bool isExpressionEnd(char c)
{
    return c == ')'/* ||
           c == '}' ||
           c == ']'*/;
}

static constexpr bool shouldSkipCharacter(char c)
{
    return c < '!';
}

bool isOperator(ExpPtr e, Environment* env)
{
    if (d_cast<Operator>(e))
        return true;
    if (d_cast<Identifier>(e))
    {
        auto value = env->get(e);
        return d_cast<Operator>(value) != nullptr;
    }
    return false;
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
            return make_ptr<ParseError>(ss);
        }

        return make_ptr<Integer>(stoll(ss));
    }
    if (isNameCharacter(s[start]))
    {
        auto ss = s.substr(start, end - start);

        return make_ptr<Identifier>(ss);
    }
    if (isOperatorCharacter(s[start]))
    {
        auto ss = s.substr(start, end - start);

        return make_ptr<Identifier>(ss);
    }
}

void makeOperation(std::stack<std::shared_ptr<Operator>>& operatorStack,
                   std::deque<ExpPtr>& q,
                   Environment* env)
{
    ExpPtr left;
    ExpPtr right;
    std::shared_ptr<Operator> op = operatorStack.top();
    operatorStack.pop();

    right = q.back();
    q.pop_back();
    if (!q.empty())
    {
        left = q.back();
        q.pop_back();

        // "Preprocessor" handling (#)
        auto variable = d_cast<Identifier>(left);
        bool evalForce = false;
        if (variable)
            evalForce = variable->name == "#";

        if (evalForce)
            q.push_back(right->eval(env));
        else
            q.push_back(make_ptr<Operation>(op, left, right));
    }
    else
    {
        // Unary operator case
        auto body = make_ptr<Operation>(
                    op,
                    right,
                    make_ptr<Integer>(0)); // should get type
        q.push_back(body); //and take default value of the type

        /*
        // if it's operator and one expression
        auto variableName = "snd";
        auto body = make_ptr<Operation>(
                    op,
                    make_ptr<Variable>(variableName),
                    right);
        auto lambdaApp = make_ptr<Operation>(
                    make_ptr<Application>(),
                    make_ptr<Lambda>(),
                    make_ptr<Variable>(variableName));
        auto lambda = make_ptr<Operation>(
                    make_ptr<Application>(),
                    lambdaApp,
                    body);
        q.push_back(lambda);*/
    }
}

ExpPtr Parser::parse(const std::string& s,
                     size_t& i,
                     size_t n,
                     Environment* env)
{
    ExpPtr ret = nullptr;

    std::stack<std::shared_ptr<Operator>> operatorStack;
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
                e = make_ptr<String>(ss);
            }
            else
            {
                e = make_ptr<ParseError>("Unable to parse expression");
                return e;
            }
            /* SHUNTING-YARD
             * f token = name     => q.push token
             *         | function => s.push token
             *         | operator => while s.top is operator
             *                          && (token is left associative
             *                          && token <= s.top
             *                          || token is right associative
             *                          && token <  s.top)
             *                       { q.push(s.pop) }
             *                       s.push token
             * (modified version is used, this ^ is just for a reference
             */

            if (isOperator(e, env))
            {
                applicationFlag = false;

                auto op = d_cast<Operator>(e);
                if (!op)
                    op = d_cast<Operator>(env->get(e));

                while (!operatorStack.empty() &&
                       env->compareOperators(op, operatorStack.top()))
                    makeOperation(operatorStack, q, env);

                operatorStack.push(op);
            }
            else
            {
                q.push_back(e);
                if (applicationFlag)
                {
                    operatorStack.push(make_ptr<Application>());
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
        ret = make_ptr<Void>();

    return ret;
}

ExpPtr Parser::parseFile(const std::string& filename)
{
    auto env = Environment::create();
    // Leak!
    auto exp = parseFile(filename, env);
    return exp;
}

ExpPtr Parser::parseFile(const std::string& filename, Environment* env)
{
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()   ));
    auto newEnv = env;
    return Parser::parse(content, env)->eval(newEnv);
}

std::vector<Parser::Token> Parser::split(const std::string& s,
                                         Environment* env)
{
    std::vector<Token> ret;

    bool parsingName = false;
    bool applicationFlag = false;
    auto currentNameStart = s.begin();
    for (auto c = s.begin(); c != s.end(); c++)
    {
        if (shouldSkipCharacter(*c))
        {
            continue;
        }
    }
}
