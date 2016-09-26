#include "Environment.h"
#include "Expression.h"
#include "TypeError.h"
#include "Addition.h"
#include "Subtraction.h"
#include "Multiplication.h"
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
#include "Quote.h"
#include "EvalForce.h"
#include "IntegerType.h"
#include "Operation.h"
#include "Any.h"
#include "Equality.h"
#include "Less.h"
#include "More.h"
#include "ValueInSet.h"

#include <vector>
#include <fstream>

Environment::Environment(const std::shared_ptr<DebugPrinter>& debugPrinter) :
    debugPrinter(debugPrinter)
{
    addDefaultVariables();
}

void Environment::clear()
{
    data.clear();
    addDefaultVariables();
}

ExpPtr Environment::getEqual(CExpPtrArg key) const
{
    auto value = get(key);

    if (checkType<Equals>(value))
        return s_cast<Equals>(value)->value;
    else if (checkType<Any>(value))
        return value;
    else if (checkType<Void>(value))
        return value;
    else
        return make_ptr<ValueInSet>(value);
}

ExpPtr Environment::get(CExpPtrArg key) const
{
    debugPrint("ENV GET: (" + key->show() + ") : (", true);

    auto x = data.find(key);

    ExpPtr ret;
    if (x == std::end(data))
        ret = make_ptr<Any>();
    else
        ret = x->second;

    debugPrint(ret->show() + ")\n", false);

    return ret;
}

void Environment::debugPrint(const std::string& s, bool shouldIndent) const
{
    debugPrinter->debugPrint(s, shouldIndent);
}

void Environment::toggleDebugPrint()
{
    debugPrinter->toggleDebugPrint();
}

void Environment::erase(CExpPtrArg e)
{
    data.erase(e);
}

void Environment::add(CExpPtrArg key, ExpPtrArg value)
{
    debugPrint("ENV ADD: " + key->show() + "\n", true);
    get(key);
    debugPrint("    NEW: " + value->show() + "\n", true);
    auto constKey = std::const_pointer_cast<Expression>(get(key));
//    if (checkType<Operation>(value)
    data[key] = intersect(constKey, value);
    debugPrint("    RET: " + data[key]->show() + "\n", true);
}

void Environment::addEqual(CExpPtrArg key, ExpPtrArg value)
{
    add(key, make_ptr<Equals>(value));
}

std::vector<std::string> Environment::getKeys() const
{
    std::vector<std::string> ret;
    ret.reserve(data.size());

    for (auto &x : data)
        if (checkType<const Identifier>(x.first))
        {
            ret.push_back(s_cast<const Identifier>(x.first)->name);
        }

    return ret;
}

bool Environment::operator==(const Environment& other) const
{
    return data == other.data;
}

void Environment::addDefaultVariables()
{
    addEqual(make_ptr<Identifier>(Void           ::defaultName), make_ptr<Void>());
    addEqual(make_ptr<Identifier>(Lambda         ::defaultName), make_ptr<Lambda>());
    addEqual(make_ptr<Identifier>(Then           ::defaultName), make_ptr<Then>());
//    addEqual(make_ptr<Identifier>(EvalForce      ::defaultName), make_ptr<EvalForce>());
    addEqual(make_ptr<Identifier>(Quote          ::defaultName), make_ptr<Quote>());
    addEqual(make_ptr<Identifier>(Print          ::defaultName), make_ptr<Print>());
    addEqual(make_ptr<Identifier>(Include        ::defaultName), make_ptr<Include>());
    addEqual(make_ptr<Identifier>(Addition       ::defaultName), make_ptr<Addition>());
    addEqual(make_ptr<Identifier>(Union          ::defaultName), make_ptr<Union>());
    addEqual(make_ptr<Identifier>(IntegerType    ::defaultName), make_ptr<IntegerType>());
    addEqual(make_ptr<Identifier>(Intersection   ::defaultName), make_ptr<Intersection>());
    addEqual(make_ptr<Identifier>(Mutation       ::defaultName), make_ptr<Mutation>());
    addEqual(make_ptr<Identifier>(Subtraction    ::defaultName), make_ptr<Subtraction>());
    //addEqual(make_ptr<Identifier>(Multiplication ::defaultName), make_ptr<Multiplication>());
    addEqual(make_ptr<Identifier>(Equality       ::defaultName), make_ptr<Equality>());
//    addEqual(make_ptr<Identifier>(Pair           ::defaultName), make_ptr<Pair>());
    addEqual(make_ptr<Identifier>(Less           ::defaultName), make_ptr<Less>());
    addEqual(make_ptr<Identifier>(More           ::defaultName), make_ptr<More>());
    addEqual(make_ptr<Identifier>(Any            ::defaultName), make_ptr<Any>());
    addEqual(make_ptr<Identifier>(PrintInfo      ::defaultName), make_ptr<PrintInfo>());
    addEqual(make_ptr<Identifier>(PrintEnv       ::defaultName), make_ptr<PrintEnv>());
    addEqual(make_ptr<Identifier>(ReverseApplication::defaultName), make_ptr<ReverseApplication>());
    addEqual(make_ptr<Identifier>(LowPriorityApplication::defaultName), make_ptr<LowPriorityApplication>());
}

bool Environment::compareOperators(const std::shared_ptr<Operator>& first,
                                   const std::shared_ptr<Operator>& second)
{
    int firstValue = 0, secondValue = 1;
    bool isFirstRightAssociative = false;

    firstValue = first->priority;
    isFirstRightAssociative = first->isRightAssociative;
    secondValue = second->priority;

    return (firstValue < secondValue) ||
           (!isFirstRightAssociative && (firstValue <= secondValue));
}

ExpPtr Environment::intersect(ExpPtrArg l, ExpPtrArg r)
{
    auto env = *this;

    auto lp = checkType<Identifier>(l) ? get(l) : l;
    auto rp = checkType<Identifier>(r) ? get(r) : r;

    return Intersection().operate(lp, rp, env);
}

std::string Environment::show() const
{
    std::string ret = "";
    for (auto &x : data)
        ret += x.first->show() + " |--> " + x.second->show() + "\n";

    return ret;
}


void Environment::increaseDebugIndentation()
{
    debugPrinter->increaseDebugIndentation();
}

void Environment::decreaseDebugIndentation()
{
    debugPrinter->decreaseDebugIndentation();
}