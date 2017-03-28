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
#include "System/Cast.h"
using namespace std;

Object Parser::parse(const string& s, Environment& env)
{
    lexer.tokenize(s);
    auto tokens = lexer.getTokens();
    lexer.clear();

    return parse(begin(tokens), end(tokens), env, 0);
}

static bool isOperator(Object e, Environment& env)
{
    if (!e)
        return false;
    e = Identifier::unwrapIfId(e, env);
    if (cast<Operator>(env, e))
        return true;
    else
        return false;
}

Object Parser::parseFile(const std::string& filename, Environment& env)
{
    std::ifstream ifs(filename);
    std::string content((std::istreambuf_iterator<char>(ifs)),
                        (std::istreambuf_iterator<char>()   ));

    return Parser::parse(content, env);
}

Object Parser::parse(const vector<Token>::iterator& begin,
                     const vector<Token>::iterator& end,
                     Environment& env,
                     size_t indentation)
{
    {
        stringstream debugOutput;
        debugOutput << "Parsing: ";
        std::copy(begin, end, std::ostream_iterator<Token>(debugOutput, "; "));
        debugOutput << endl;
        env.debugPrint(debugOutput.str());
    }

    Object ret;

    std::stack<std::shared_ptr<Operator>> operatorStack;
    std::deque<Object> q;
    bool applicationFlag = false;
    bool operatorWasLast = false;
    bool expressionBegin = true;

    auto makeOperation = [&](){
        Object left;
        Object right;
        std::shared_ptr<Operator> op = operatorStack.top();
        operatorStack.pop();

        if (q.empty() && expressionBegin)
        {
            q.push_back(op->partialApplyNoArgs(env));
            // q.push_back(op);
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
            if (auto variable = cast<Identifier>(env, left))
                evalForce = variable->name == "#";
            if (evalForce)
                q.push_back(right);
            else
                q.push_back(makeObject<Operation>(op, left, right));
            expressionBegin = false;
        }
        else
        {
            auto pa = operatorWasLast ? op->partialApplyLeft(right, env)
                                      : op->partialApplyRight(right, env);
            q.push_back(pa);
            operatorWasLast = false;
            expressionBegin = false;
        }
    };

    for (auto currentTokenIt = begin;
         currentTokenIt != end;
         currentTokenIt++)
    {
        auto& currentToken = *currentTokenIt;
        Object e;
        if (currentToken.type() == typeid(Tokens::Identifier))
        {
            e = makeObject<Identifier>(get<Tokens::Identifier>(currentToken).name);
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
                        return makeObject<ParseError>("Parentheses mismatch");
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
                return makeObject<ParseError>("No matching parentheses");
            }
        }
        else if (currentToken.type() == typeid(Tokens::Closing))
        {
            return makeObject<ParseError>("Unexpected parentheses");
        }
        else if (currentToken.type() == typeid(Tokens::StringData))
        {
            e = makeObject<String>(get<Tokens::StringData>(currentToken).data);
        }
        else if (currentToken.type() == typeid(Tokens::IntegerData))
        {
            e = makeObject<Integer>(get<Tokens::IntegerData>(currentToken).data);
        }
        else if (currentToken.type() == typeid(Tokens::NoSpace))
        {
        }
        else if (currentToken.type() == typeid(Tokens::LineBreak))
        {
            operatorStack.push(std::make_shared<DefaultOperator>());
            makeOperation();
        }
        else if (currentToken.type() == typeid(Tokens::Tabulation))
        {
        }
        else
        {
            e = makeObject<ParseError>(
                std::string("Unexpected token of type ") + currentToken.type().name());
            return e;
        }

        if (!e)
        {

        }
        else if (isOperator(e, env))
        {
            applicationFlag = false;

            auto op = cast<Operator>(env, e);
            if (!op)
            if (auto id = cast<Identifier>(env, e))
                op = cast<Operator>(env, env.getEqual(id->name));

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
                operatorStack.push(std::make_shared<Application>());
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
        ret = makeObject<Void>();

    return ret;
}
