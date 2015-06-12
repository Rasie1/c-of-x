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
    return add(new Language::PatternAbstraction("+"), new Addition())
         ->add(new Language::PatternAbstraction("-"), new Subtraction())
         ->add(new Language::PatternAbstraction("*"), new Multiplication())
         ->add(new Language::PatternAbstraction("="), new Assignment())
         ->add(new Language::PatternAbstraction("#"), new Preprocess())
         ->add(new Language::PatternAbstraction("pair"), new PairConstructor())
         ->add(new Language::PatternAbstraction("ppat"), new PrintPattern())
         ->add(new Language::PatternAbstraction("include"), new Include())
         ->add(new Language::PatternAbstraction("\\"), new Lambda());
}

Environment* Environment::create()
{
    auto firstVariable = new EnvironmentalVariable(
                new Language::PatternAbstraction("void"), new Void());
    auto environment = new Environment(firstVariable, nullptr);
    return environment->loadDefaultVariables();
}

}
