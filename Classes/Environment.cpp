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

ExpPtr Environment::get(const PatPtr& p)
{
    if (data->match(p, this))
        return data->get();

    if (next == nullptr)
        return nullptr;

    return next->get(p);
}

Environment* Environment::add(const PatPtr& p,
                              const ExpPtr& e)
{
    auto variable = new EnvironmentalVariable(p, e);
    return new Environment(variable, this);
}

Environment* Environment::loadDefaultVariables()
{
    return add(std::make_shared<PatternVariable>(Mutation       ::defaultName), std::make_shared<Mutation>())
         ->add(std::make_shared<PatternVariable>(Assignment     ::defaultName), std::make_shared<Assignment>())
         ->add(std::make_shared<PatternVariable>(EvalForce      ::defaultName), std::make_shared<EvalForce>())
         ->add(std::make_shared<PatternVariable>(EvalDelay      ::defaultName), std::make_shared<EvalDelay>())
         ->add(std::make_shared<PatternVariable>(ReturnPattern  ::defaultName), std::make_shared<ReturnPattern>())
         ->add(std::make_shared<PatternVariable>(Print          ::defaultName), std::make_shared<Print>())
         ->add(std::make_shared<PatternVariable>(PrintPattern   ::defaultName), std::make_shared<PrintPattern>())
         ->add(std::make_shared<PatternVariable>(Include        ::defaultName), std::make_shared<Include>())
         ->add(std::make_shared<PatternVariable>(Lambda         ::defaultName), std::make_shared<Lambda>())
         ->add(std::make_shared<PatternOperator>(Addition       ::defaultName, std::make_shared<Addition>()), std::make_shared<Addition>())
         ->add(std::make_shared<PatternOperator>(Subtraction    ::defaultName, std::make_shared<Subtraction>()), std::make_shared<Subtraction>())
         ->add(std::make_shared<PatternOperator>(Multiplication ::defaultName, std::make_shared<Multiplication>()), std::make_shared<Multiplication>())
         ->add(std::make_shared<PatternOperator>(Assignment     ::defaultName, std::make_shared<Assignment>()), std::make_shared<Assignment>())
         ->add(std::make_shared<PatternOperator>(Pair           ::defaultName, std::make_shared<Pair>()), std::make_shared<Pair>());
}

Environment* Environment::create()
{
    auto firstVariable = new EnvironmentalVariable(
                std::make_shared<PatternVariable>(Void::defaultName),
                std::make_shared<Void>());
    auto environment = new Environment(firstVariable, nullptr);
    return environment->loadDefaultVariables();
}

bool Environment::compareOperators(const ExpPtr& first,
                                   const ExpPtr& second)
{
    int firstValue = 0, secondValue = 1;
    bool isFirstRightAssociative = false;

    std::shared_ptr<Operator> firstOperator, secondOperator;
    //std::shared_ptr<Variable> firstOperator, secondOperator;
    auto env = this;
    firstOperator  = std::dynamic_pointer_cast<Operator>(first->eval(env));
    secondOperator = std::dynamic_pointer_cast<Operator>(second->eval(env));

    if (firstOperator)
    {
        firstValue = firstOperator->priority;
        isFirstRightAssociative = firstOperator->isRightAssociative;
    }
    if (secondOperator)
        secondValue = secondOperator->priority;

    return firstValue < secondValue ||
           isFirstRightAssociative && (firstValue <= secondValue);
}

std::pair<PatPtr, ExpPtr> Environment::top()
{
    return std::pair<PatPtr, ExpPtr>(
                data->getPattern(),
                data->get());
}

/*
bool PrecedenceTable::more(Operator* first, Operator* second)
{
    return false;
}

void PrecedenceTable::add(PrecedenceInfo* op)
{

}

void PrecedenceTable::remove(PrecedenceInfo* op)
{

}*/

}
