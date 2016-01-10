#include "Environment.h"
#include "Expression.h"
#include "TypeError.h"
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
#include "Variable.h"
#include "String.h"
#include "Then.h"
#include "Union.h"
#include "Intersection.h"
#include "EvalDelay.h"
#include "EvalForce.h"

#include <vector>

Environment::Environment(ExpPtrArg key, ExpPtrArg value, Environment* next)
    : key(key),
      value(value),
      next(next)
{
}

void Environment::clear()
{
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

ExpPtr Environment::get(ExpPtrArg key)
{
    if (this->key->match(key))
        return value;

    if (next == nullptr)
        return nullptr;

    return next->get(key);
}

Environment* Environment::add(ExpPtrArg key,
                              ExpPtrArg value)
{
    return new Environment(key, value, this);
}

Environment* Environment::loadDefaultVariables()
{
    return add(make_ptr<Variable>(Lambda         ::defaultName), make_ptr<Lambda>())
         ->add(make_ptr<Variable>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->add(make_ptr<Variable>(Then           ::defaultName), make_ptr<Then>())
         ->add(make_ptr<Variable>(EvalForce      ::defaultName), make_ptr<EvalForce>())
         ->add(make_ptr<Variable>(EvalDelay      ::defaultName), make_ptr<EvalDelay>())
         ->add(make_ptr<Variable>(Print          ::defaultName), make_ptr<Print>())
         ->add(make_ptr<Variable>(Include        ::defaultName), make_ptr<Include>())
         ->add(make_ptr<Variable>(Addition       ::defaultName), make_ptr<Addition>())
         ->add(make_ptr<Variable>(Union          ::defaultName), make_ptr<Union>())
         ->add(make_ptr<Variable>(Intersection   ::defaultName), make_ptr<Intersection>())
         //->add(make_ptr<Variable>(Mutation       ::defaultName), make_ptr<Mutation>())
         ->add(make_ptr<Variable>(Subtraction    ::defaultName), make_ptr<Subtraction>())
         //->add(make_ptr<Variable>(Multiplication ::defaultName), make_ptr<Multiplication>())
         ->add(make_ptr<Variable>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->add(make_ptr<Variable>(Pair           ::defaultName), make_ptr<Pair>())
         ;
}

Environment* Environment::create()
{
    auto environment = new Environment(make_ptr<Variable>(Void::defaultName),
                                       make_ptr<Void>(),
                                       nullptr);
    return environment->loadDefaultVariables();
}

bool Environment::compareOperators(ExpPtrArg first,
                                   ExpPtrArg second)
{
    int firstValue = 0, secondValue = 1;
    bool isFirstRightAssociative = false;

    std::shared_ptr<Operator> firstOperator, secondOperator;
    //std::shared_ptr<Variable> firstOperator, secondOperator;
    auto env = this;
    firstOperator  = d_cast<Operator>(first->eval(env));
    secondOperator = d_cast<Operator>(second->eval(env));

    if (firstOperator)
    {
        firstValue = firstOperator->priority;
        isFirstRightAssociative = firstOperator->isRightAssociative;
    }
    if (secondOperator)
        secondValue = secondOperator->priority;

    return firstValue < secondValue ||
           !isFirstRightAssociative && (firstValue <= secondValue);
}

std::pair<ExpPtr, ExpPtr> Environment::top()
{
    return std::pair<ExpPtr, ExpPtr>(key, value);
}

Environment* Environment::getNext()
{
    return this->next;
}

ExpPtr Environment::lookup(const std::string& name)
{
    return nullptr;
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

