#include "Environment.h"
#include "Expression.h"
#include "TypeError.h"
#include "EnvironmentalVariable.h"
#include "Pattern.h"

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
    return add(new Language::PatternAbstraction(Addition::defaultName),        new Addition())
         ->add(new Language::PatternAbstraction(Subtraction::defaultName),     new Subtraction())
         ->add(new Language::PatternAbstraction(Multiplication::defaultName),  new Multiplication())
         ->add(new Language::PatternAbstraction(Mutation::defaultName),        new Mutation())
         ->add(new Language::PatternAbstraction(Assignment::defaultName),      new Assignment())
         ->add(new Language::PatternAbstraction(EvalForce::defaultName),       new EvalForce())
         ->add(new Language::PatternAbstraction(EvalDelay::defaultName),       new EvalDelay())
         ->add(new Language::PatternAbstraction(PairConstructor::defaultName), new PairConstructor())
         ->add(new Language::PatternAbstraction(Print::defaultName),           new Print())
         ->add(new Language::PatternAbstraction(PrintPattern::defaultName),    new PrintPattern())
         ->add(new Language::PatternAbstraction(Include::defaultName),         new Include())
         ->add(new Language::PatternAbstraction("list"), new MakeList())
         ->add(new Language::PatternAbstraction(Lambda::defaultName),          new Lambda());
}

Environment* Environment::create()
{
    auto firstVariable = new EnvironmentalVariable(
                new Language::PatternAbstraction(Void::defaultName), new Void());
    auto environment = new Environment(firstVariable, nullptr);
    return environment->loadDefaultVariables();
}

}
