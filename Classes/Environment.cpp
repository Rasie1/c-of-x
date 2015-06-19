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
#include "List.h"
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

Expression* Environment::get(Pattern* p)
{
    if (data->match(p, this))
        return data->get();

    if (next == nullptr)
        return nullptr;

    return next->get(p);
}

Environment* Environment::add(Pattern* p, Expression* e)
{
    auto variable = new EnvironmentalVariable(p, e);
    return new Environment(variable, this);
}

Environment* Environment::loadDefaultVariables()
{
    return add(new Language::PatternVariable(Addition       ::defaultName), new Addition())
         ->add(new Language::PatternVariable(Subtraction::    defaultName), new Subtraction())
         ->add(new Language::PatternVariable(Multiplication:: defaultName), new Multiplication())
         ->add(new Language::PatternVariable(Mutation::       defaultName), new Mutation())
         ->add(new Language::PatternVariable(Assignment::     defaultName), new Assignment())
         ->add(new Language::PatternVariable(EvalForce::      defaultName), new EvalForce())
         ->add(new Language::PatternVariable(EvalDelay::      defaultName), new EvalDelay())
         ->add(new Language::PatternVariable(PairConstructor::defaultName), new PairConstructor())
         ->add(new Language::PatternVariable(Print::          defaultName), new Print())
         ->add(new Language::PatternVariable(PrintPattern::   defaultName), new PrintPattern())
         ->add(new Language::PatternVariable(Include::        defaultName), new Include())
         ->add(new Language::PatternVariable("list"),                       new MakeList())
         ->add(new Language::PatternVariable(Lambda::         defaultName), new Lambda())
         ->add(new Language::PatternOperator(Plus::           defaultName, new Plus()), new Plus());
}

Environment* Environment::create()
{
    auto firstVariable = new EnvironmentalVariable(
                new Language::PatternVariable(Void::defaultName), new Void());
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
