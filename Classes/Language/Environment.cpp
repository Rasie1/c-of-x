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
#include "Operation.h"

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

ExpPtr Environment::getEqual(ExpPtrArg key)
{
    if (*(this->key) == *key)
    {
        auto val = d_cast<Equals>(value);
        if (val)
            return val->value;
        else
            return next->getEqual(key);
    }

    if (next == nullptr)
        return nullptr;

    return next->getEqual(key);
}

Environment* Environment::add(ExpPtrArg key, ExpPtrArg value)
{
    return new Environment(key, value, this);
}

Environment* Environment::addEqual(ExpPtrArg key, ExpPtrArg value)
{
    return add(key, make_ptr<Equals>(value));
}

Environment* Environment::loadDefaultVariables()
{
    return addEqual(make_ptr<Identifier>(Lambda         ::defaultName), make_ptr<Lambda>())
         ->addEqual(make_ptr<Identifier>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->addEqual(make_ptr<Identifier>(Then           ::defaultName), make_ptr<Then>())
         ->addEqual(make_ptr<Identifier>(EvalForce      ::defaultName), make_ptr<EvalForce>())
         ->addEqual(make_ptr<Identifier>(EvalDelay      ::defaultName), make_ptr<EvalDelay>())
         ->addEqual(make_ptr<Identifier>(Print          ::defaultName), make_ptr<Print>())
         ->addEqual(make_ptr<Identifier>(Include        ::defaultName), make_ptr<Include>())
         ->addEqual(make_ptr<Identifier>(Then           ::defaultName), make_ptr<Then>())
         ->addEqual(make_ptr<Identifier>(Addition       ::defaultName), make_ptr<Addition>())
         ->addEqual(make_ptr<Identifier>(Union          ::defaultName), make_ptr<Union>())
         ->addEqual(make_ptr<Identifier>(IntegerType    ::defaultName), make_ptr<IntegerType>())
         ->addEqual(make_ptr<Identifier>(Intersection   ::defaultName), make_ptr<Intersection>())
         //->addEqual(make_ptr<Identifier>(Mutation       ::defaultName), make_ptr<Mutation>())
         ->addEqual(make_ptr<Identifier>(Subtraction    ::defaultName), make_ptr<Subtraction>())
         //->addEqual(make_ptr<Identifier>(Multiplication ::defaultName), make_ptr<Multiplication>())
         ->addEqual(make_ptr<Identifier>(Assignment     ::defaultName), make_ptr<Assignment>())
         ->addEqual(make_ptr<Identifier>(Equality       ::defaultName), make_ptr<Equality>())
         ->addEqual(make_ptr<Identifier>(Pair           ::defaultName), make_ptr<Pair>())
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

std::pair<ExpPtr, Environment*> Environment::process(ExpPtrArg id,
                                                     const std::shared_ptr<Predicate>& f)
{
    throw 0;
    auto expr = this->getEqual(id);
    auto result = f;
//    auto result = f->intersect(expr);
    auto env = this;
    return std::pair<ExpPtr, Environment*>(result, env->addEqual(id, result));
//    auto expr = this->get(id);
//    if (expr == nullptr)
//        expr = id;
//    if (!f->holds(expr, this))
//        return std::pair<ExpPtr, Environment*>(std::make_shared<Void>(), this);
//    auto env = this;
////    auto result = f->apply(expr, env);
//    auto result = expr->intersect(f, this);

//    return std::pair<ExpPtr, Environment*>(result, env->add(id, result));
}


ExpPtr Environment::intersect(ExpPtrArg l, ExpPtrArg r)
{
    auto env = this;
    return Intersection().operate(l, r, env);
}
