#include "Parser/Parser.h"
#include <sstream>
#include <string>
#include <stack>
#include <queue>
#include <iostream>
#include <fstream>
#include "System/Environment.h"
#include "Expressions/Expression.h"
#include "Expressions/DefaultOperator.h"
#include "Expressions/Integer.h"
#include "Expressions/Application.h"
#include "Expressions/ExceptionParseError.h"
#include "Expressions/Void.h"
#include "Expressions/ParseError.h"
#include "Expressions/Identifier.h"
#include "Expressions/String.h"
#include "Expressions/Operation.h"
#include "Expressions/EvalForce.h"
#include "Expressions/Lambda.h"
using namespace std;

ExpPtr Parser::parse(const string& s, Environment& env)
{
    lexer.tokenize(s);
    auto tokens = lexer.getTokens();
    lexer.clear();

    return parse(begin(tokens), end(tokens), env, 0);
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

ExpPtr Parser::parseFile(const std::string& filename, Environment& env)
{
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()   ));

    return Parser::parse(content, env)->eval(env);
}

ExpPtr Parser::parse(const vector<Token>::iterator& begin,
                     const vector<Token>::iterator& end,
                     Environment& env,
                     size_t indentation)
{
    ExpPtr ret = nullptr;

    std::stack<std::shared_ptr<Operator>> operatorStack;
    std::deque<ExpPtr> q;
    bool applicationFlag = false;
    bool operatorWasLast = false;

    auto makeOperation = [&](){
            ExpPtr left;
        ExpPtr right;
        std::shared_ptr<Operator> op = operatorStack.top();
        operatorStack.pop();

        if (q.empty())
        {
            q.push_back(op->partialApplyNoArgs(env));
            return;
        }

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
            auto pa = operatorWasLast ? op->partialApplyLeft(right, env)
                                      : op->partialApplyRight(right, env);
            q.push_back(pa);
            operatorWasLast = false;
        }
    };

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
            int parsingParenthesisDepth = 1;
            while (newEnd != end)
            {
                auto currentToken = *newEnd;
                if (currentToken.type() == typeid(Tokens::Closing))
                {
                    parsingParenthesisDepth--;
                    if (false) // todo: case with wrong brackets
                    {
                        return make_ptr<ParseError>("Parentheses mismatch");
                    }
                    if (parsingParenthesisDepth == 0)
                    {
                        e = parse(next(currentTokenIt), newEnd, env, 0);
                        currentTokenIt = newEnd;
                        break;
                    }
                }
                else if (currentToken.type() == typeid(Tokens::Opening))
                {
                    parsingParenthesisDepth++;
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
            return make_ptr<ParseError>("Unexpected parentheses");
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
        else if (currentToken.type() == typeid(Tokens::LineBreak))
        {
            operatorStack.push(make_ptr<DefaultOperator>());
            makeOperation();
        }
        else if (currentToken.type() == typeid(Tokens::Tabulation))
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
                makeOperation();

            operatorStack.push(op);
            operatorWasLast = true;
        }
        else
        {
            q.push_back(e);
            if (applicationFlag)
            {
                operatorStack.push(make_ptr<Application>());
                makeOperation();
            }

            operatorWasLast = false;
            applicationFlag = true;
        }
    }

    while (!operatorStack.empty())
        makeOperation();

    if (!q.empty())
        ret = q.front();
    if (!ret)
        ret = make_ptr<Void>();

    return ret;
}
