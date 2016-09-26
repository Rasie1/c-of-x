#include "Parser.h"
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include "Expression.h"
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

ExpPtr Parser::parse(const string& s, Environment& env)
{
    lexer.tokenize(s);
    auto tokens = lexer.getTokens();
    lexer.clear();

    return parse(begin(tokens), end(tokens), env);
}

static bool isOperator(ExpPtr e, Environment& env)
{
    if (!e)
        return false;
    e = Identifier::unwrapIfId(e, env);
    if (d_cast<Operator>(e))
        return true;
    else
        return false;
}

static void makeOperation(std::stack<std::shared_ptr<Operator>>& operatorStack,
                          std::deque<ExpPtr>& q,
                          Environment& env)
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

        // Handle force-eval operator
        bool evalForce = false;
        if (checkType<Identifier>(left))
        {
            auto variable = s_cast<Identifier>(left);
            evalForce = variable->name == "#";
        }
        if (evalForce)
            q.push_back(right->eval(env));
        else
            q.push_back(make_ptr<Operation>(op, left, right));
    }
    else
    {
        auto pa = op->partialApply(right, env);
        q.push_back(pa);
    }
}

ExpPtr Parser::parseFile(const std::string& filename, Environment& env)
{
    std::ifstream ifs(filename);
//    std::string content((std::istreambuf_iterator<char>(ifs)),
//                        (std::istreambuf_iterator<char>()   ));
    std::string line;
    ExpPtr e;
    while (std::getline(ifs, line))
    {
        e = Parser::parse(line, env)->eval(env);
    }

    return e;
}

ExpPtr Parser::parse(const vector<Token>::iterator& begin,
                     const vector<Token>::iterator& end,
                     Environment& env)
{
    ExpPtr ret = nullptr;

    std::stack<std::shared_ptr<Operator>> operatorStack;
    std::deque<ExpPtr> q;
    bool applicationFlag = false;

    for (auto currentTokenIt = begin;
         currentTokenIt != end;
         currentTokenIt++)
    {
        auto& currentToken = *currentTokenIt;
        ExpPtr e = nullptr;
        if (currentToken.type() == typeid(Tokens::Identifier))
        {
            e = make_ptr<Identifier>(get<Tokens::Identifier>(currentToken).name);
        }
        else if (currentToken.type() == typeid(Tokens::Opening))
        {
            auto newEnd = next(currentTokenIt);
            while (newEnd != end)
            {
                auto currentToken = *newEnd;
                if (currentToken.type() == typeid(Tokens::Closing))
                {
                    if (false) // todo: case with wrong brackets
                    {
                        return make_ptr<ParseError>("Parentheses mismatch");
                    }
                    e = parse(next(currentTokenIt), newEnd, env);
                    currentTokenIt = newEnd;
                    break;
                }
                std::advance(newEnd, 1);
            }
            if (newEnd == end)
            {
                return make_ptr<ParseError>("No matching parentheses");
            }
        }
        else if (currentToken.type() == typeid(Tokens::Closing))
        {
            return make_ptr<ParseError>("Unexepected parentheses");
        }
        else if (currentToken.type() == typeid(Tokens::StringData))
        {
            e = make_ptr<String>(get<Tokens::StringData>(currentToken).data);
        }
        else if (currentToken.type() == typeid(Tokens::IntegerData))
        {
            e = make_ptr<Integer>(get<Tokens::IntegerData>(currentToken).data);
        }
        else if (currentToken.type() == typeid(Tokens::NoSpace))
        {
        }
        else
        {
            e = make_ptr<ParseError>(
                std::string("Unexpected token of type ") + currentToken.type().name());
            return e;
        }

        if (!e)
        {

        }
        else if (isOperator(e, env))
        {
            applicationFlag = false;

            auto op = d_cast<Operator>(e);
            if (!op)
                op = d_cast<Operator>(env.getEqual(e));

            while (!operatorStack.empty() &&
                   env.compareOperators(op, operatorStack.top()))
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

    while (!operatorStack.empty())
        makeOperation(operatorStack, q, env);

    if (!q.empty())
        ret = q.front();
    if (!ret)
        ret = make_ptr<Void>();

    return ret;
}
