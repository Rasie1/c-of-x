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
#include "Identifier.h"
#include "String.h"
#include "Then.h"
#include "Union.h"
#include "Intersection.h"
#include "EvalDelay.h"
#include "EvalForce.h"
#include "IntegerType.h"

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
    if (*(this->key) == *key)
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
    return add(make_ptr<Identifier>(Lambda         ::defaultName), make_ptr<Lambda>())
         ->add(make_ptr<Identifier>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->add(make_ptr<Identifier>(Then           ::defaultName), make_ptr<Then>())
         ->add(make_ptr<Identifier>(EvalForce      ::defaultName), make_ptr<EvalForce>())
         ->add(make_ptr<Identifier>(EvalDelay      ::defaultName), make_ptr<EvalDelay>())
         ->add(make_ptr<Identifier>(Print          ::defaultName), make_ptr<Print>())
         ->add(make_ptr<Identifier>(Include        ::defaultName), make_ptr<Include>())
         ->add(make_ptr<Identifier>(Then           ::defaultName), make_ptr<Then>())
         ->add(make_ptr<Identifier>(Addition       ::defaultName), make_ptr<Addition>())
         ->add(make_ptr<Identifier>(Union          ::defaultName), make_ptr<Union>())
         ->add(make_ptr<Identifier>(IntegerType    ::defaultName), make_ptr<IntegerType>())
         ->add(make_ptr<Identifier>(Intersection   ::defaultName), make_ptr<Intersection>())
         //->add(make_ptr<Identifier>(Mutation       ::defaultName), make_ptr<Mutation>())
         ->add(make_ptr<Identifier>(Subtraction    ::defaultName), make_ptr<Subtraction>())
         //->add(make_ptr<Identifier>(Multiplication ::defaultName), make_ptr<Multiplication>())
         ->add(make_ptr<Identifier>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->add(make_ptr<Identifier>(Pair           ::defaultName), make_ptr<Pair>())
         ;
}

Environment* Environment::create()
{
    auto environment = new Environment(make_ptr<Identifier>(Void::defaultName),
                                       make_ptr<Void>(),
                                       nullptr);
    return environment->loadDefaultVariables();
}

bool Environment::compareOperators(const std::shared_ptr<Operator>& first,
                                   const std::shared_ptr<Operator>& second)
{
    int firstValue = 0, secondValue = 1;
    bool isFirstRightAssociative = false;

    auto env = this;

    firstValue = first->priority;
    isFirstRightAssociative = first->isRightAssociative;
    secondValue = second->priority;

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

std::pair<ExpPtr, Environment*> Environment::process(ExpPtrArg id, ExpPtrArg f)
{
    auto expr = this->get(id);
    auto result = expr->intersect(f, this);

    return std::pair<ExpPtr, Environment*>(result, this->add(id, result));
}
