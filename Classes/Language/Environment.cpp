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
#include "Any.h"
#include "Equality.h"

#include <vector>

Environment::Environment()
{
    addDefaultVariables();
}

void Environment::clear()
{
    data.clear();
    addDefaultVariables();
}

ExpPtr Environment::getEqual(ExpPtrArg key)
{
    auto value = get(key);
    auto val = d_cast<Equals>(value);
    if (val)
        return val->value;
    else
        return make_ptr<Any>();
}

ExpPtr Environment::get(ExpPtrArg key)
{
    auto x = data.find(key);
    if (x == std::end(data))
        return make_ptr<Any>();
    else
        return x->second;
}

void Environment::add(ExpPtrArg key, ExpPtrArg value)
{
    data[key] = value;
}

void Environment::addEqual(ExpPtrArg key, ExpPtrArg value)
{
    add(key, make_ptr<Equals>(value));
}

void Environment::addDefaultVariables()
{
    addEqual(make_ptr<Identifier>(Void           ::defaultName), make_ptr<Void>());
    addEqual(make_ptr<Identifier>(Lambda         ::defaultName), make_ptr<Lambda>());
    addEqual(make_ptr<Identifier>(Assignment     ::defaultName), make_ptr<Assignment>());
    addEqual(make_ptr<Identifier>(Then           ::defaultName), make_ptr<Then>());
    addEqual(make_ptr<Identifier>(EvalForce      ::defaultName), make_ptr<EvalForce>());
    addEqual(make_ptr<Identifier>(EvalDelay      ::defaultName), make_ptr<EvalDelay>());
    addEqual(make_ptr<Identifier>(Print          ::defaultName), make_ptr<Print>());
    addEqual(make_ptr<Identifier>(Include        ::defaultName), make_ptr<Include>());
    addEqual(make_ptr<Identifier>(Then           ::defaultName), make_ptr<Then>());
    addEqual(make_ptr<Identifier>(Addition       ::defaultName), make_ptr<Addition>());
    addEqual(make_ptr<Identifier>(Union          ::defaultName), make_ptr<Union>());
    addEqual(make_ptr<Identifier>(IntegerType    ::defaultName), make_ptr<IntegerType>());
    addEqual(make_ptr<Identifier>(Intersection   ::defaultName), make_ptr<Intersection>());
    //->addEqual(make_ptr<Identifier>(Mutation       ::defaultName), make_ptr<Mutation>());
    addEqual(make_ptr<Identifier>(Subtraction    ::defaultName), make_ptr<Subtraction>());
    //->addEqual(make_ptr<Identifier>(Multiplication ::defaultName), make_ptr<Multiplication>());
    addEqual(make_ptr<Identifier>(Assignment     ::defaultName), make_ptr<Assignment>());
    addEqual(make_ptr<Identifier>(Equality       ::defaultName), make_ptr<Equality>());
    addEqual(make_ptr<Identifier>(Pair           ::defaultName), make_ptr<Pair>());
    addEqual(make_ptr<Identifier>(Any            ::defaultName), make_ptr<Any>());
    addEqual(make_ptr<Identifier>(PrintInfo      ::defaultName), make_ptr<PrintInfo>());
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

ExpPtr Environment::intersect(ExpPtrArg l, ExpPtrArg r)
{
    auto env = this;

    auto lp = (d_cast<Identifier>(l) != nullptr) ? get(l) : l;
    auto rp = (d_cast<Identifier>(r) != nullptr) ? get(r) : r;
    return Intersection().operate(lp, rp, env);
}
