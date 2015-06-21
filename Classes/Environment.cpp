#include "Environment.h"
#include "Expression.h"
#include "TypeError.h"
#include "EnvironmentalVariable.h"
#include "PatternVariable.h"
#include "PatternOperator.h"

#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
#include "Assignment.h"
#include "Pair.h"
#include "Void.h"
#include "Function.h"
#include "Lambda.h"
#include "Include.h"
#include "Mutation.h"
#include "Print.h"
#include "Application.h"
//#include "Operator.h"

#include "EvalDelay.h"
#include "EvalForce.h"

namespace Language
{

Environment::Environment(EnvironmentalVariable* data, Environment* next)
    : data(data),
      next(next)
{
}

Environment::~Environment()
{
    delete data;
}

void Environment::clean()
{
}

void Environment::clear()
{
    delete data;
    data = nullptr;
    if (next == nullptr)
        return;

    next->clear();
    delete next;
    next = nullptr;
}

Environment* Environment::pop()
{
    auto next = this->next;
    delete this;

    return next;
}

Expression* Environment::get(const std::shared_ptr<Pattern>& p)
{
    if (data->match(p, this))
        return data->get();

    if (next == nullptr)
        return nullptr;

    return next->get(p);
}

Environment* Environment::add(const std::shared_ptr<Pattern>& p,
                              Expression* e)
{
    auto variable = new EnvironmentalVariable(p, e);
    return new Environment(variable, this);
}

Environment* Environment::loadDefaultVariables()
{
    return add(std::make_shared<PatternVariable>(Mutation       ::defaultName), new Mutation())
         ->add(std::make_shared<PatternVariable>(Assignment     ::defaultName), new Assignment())
         ->add(std::make_shared<PatternVariable>(EvalForce      ::defaultName), new EvalForce())
         ->add(std::make_shared<PatternVariable>(EvalDelay      ::defaultName), new EvalDelay())
         ->add(std::make_shared<PatternVariable>(Print          ::defaultName), new Print())
         ->add(std::make_shared<PatternVariable>(PrintPattern   ::defaultName), new PrintPattern())
         ->add(std::make_shared<PatternVariable>(Include        ::defaultName), new Include())
         ->add(std::make_shared<PatternVariable>(Lambda         ::defaultName), new Lambda())
         ->add(std::make_shared<PatternOperator>(Addition       ::defaultName, new Addition()), new Addition())
         ->add(std::make_shared<PatternOperator>(Subtraction    ::defaultName, new Subtraction()), new Subtraction())
         ->add(std::make_shared<PatternOperator>(Multiplication ::defaultName, new Multiplication()), new Multiplication())
         ->add(std::make_shared<PatternOperator>(Assignment     ::defaultName, new Assignment()), new Assignment())
         ->add(std::make_shared<PatternOperator>(Pair           ::defaultName, new Pair()), new Pair());
}

Environment* Environment::create()
{
    auto firstVariable = new EnvironmentalVariable(
                std::make_shared<PatternVariable>(Void::defaultName),
                new Void());
    auto environment = new Environment(firstVariable, nullptr);
    return environment->loadDefaultVariables();
}

bool Environment::compareOperators(Expression* first, Expression* second)
{
    int firstValue = 0, secondValue = 1;

    if (dynamic_cast<Application*>(first))
        firstValue = 10;
    if (dynamic_cast<Application*>(second))
        secondValue = 10;

    return firstValue < secondValue;
}

std::pair<std::shared_ptr<Pattern>, Expression*> Environment::top()
{
    return std::pair<std::shared_ptr<Pattern>, Expression*>(
                data->getPattern(),
                data->get());
}


bool PrecedenceTable::more(Operator* first, Operator* second)
{
    return false;
}

void PrecedenceTable::add(PrecedenceInfo* op)
{

}

void PrecedenceTable::remove(PrecedenceInfo* op)
{

}

}
