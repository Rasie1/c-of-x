#include "System/Environment.h"
#include "Expressions/Expression.h"
#include "Expressions/TypeError.h"
#include "Expressions/Addition.h"
#include "Expressions/Subtraction.h"
#include "Expressions/Multiplication.h"
#include "Expressions/Pair.h"
#include "Expressions/Void.h"
#include "Expressions/Morphism.h"
#include "Expressions/Lambda.h"
#include "Expressions/Include.h"
#include "Expressions/Inverse.h"
#include "Expressions/Mutation.h"
#include "Expressions/Print.h"
#include "Expressions/Application.h"
#include "Expressions/Identifier.h"
#include "Expressions/String.h"
#include "Expressions/Then.h"
#include "Expressions/Union.h"
#include "Expressions/Intersection.h"
#include "Expressions/Quote.h"
#include "Expressions/Not.h"
#include "Expressions/EvalForce.h"
#include "Expressions/IntegerType.h"
#include "Expressions/Operation.h"
#include "Expressions/Any.h"
#include "Expressions/Let.h"
#include "Expressions/Equality.h"
#include "Expressions/Less.h"
#include "Expressions/More.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/DefaultOperator.h"

#include <vector>
#include <fstream>

Environment::Environment(const std::shared_ptr<DebugPrinter>& debugPrinter) :
    debugPrinter(debugPrinter),
    defaultOperator(make_ptr<Then>())
{
    addDefaultDefinitions();
}

void Environment::clear()
{
    data.clear();
    addDefaultDefinitions();
}

static ExpPtr unwrapEqual(ExpPtrArg value)
{
    if (checkType<Equals>(value))
        return s_cast<Equals>(value)->value;
    else if (checkType<Any>(value))
        return value;
    else if (checkType<Operation>(value) && checkType<Union>(s_cast<Operation>(value)->op))
    {
        auto operation = s_cast<Operation>(value);
        auto l = unwrapEqual(operation->left);
        auto r = unwrapEqual(operation->right);
        return make_ptr<Operation>(make_ptr<Union>(), l, r);
    }
    else if (checkType<Void>(value))
        return value;
    else
        return make_ptr<ValueInSet>(value);
}

ExpPtr Environment::getEqual(CExpPtrArg key) const
{
    auto value = get(key);

    return unwrapEqual(value);
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

bool Environment::getDebugPrint()
{
    return debugPrinter->getDebugPrint();
}

void Environment::setDebugPrint(bool enabled)
{
    debugPrinter->setDebugPrint(enabled);
}

void Environment::erase(CExpPtrArg e)
{
    data.erase(e);
}

ExpPtr Environment::add(CExpPtrArg key, ExpPtrArg value, bool excluding)
{
    debugPrint("ENV ADD KEY: " + key->show() + "\n", true);
    debugPrint("    NEW: " + value->show() + "\n", true);
//    auto constKey = std::const_pointer_cast<Expression>(get(key));
//    if (checkType<Operation>(value)
    // if (!excluding && data.find(key) != data.end())
    //     data[key] = make_ptr<Operation>(make_ptr<Union>(), data[key], value);
    // else
    if (data.find(key) != data.end())
    {
        auto current = data[key];
        debugPrint("    OLD: " + current->show() + "\n", true);
        auto intersection = intersect(current, value);
        debugPrint("    INT: " + intersection->show() + "\n", true);
        data[key] = intersection;
    }
    else
        data[key] = value;
    debugPrint("    RET: " + data[key]->show() + "\n", true);
    return data[key];
}

ExpPtr Environment::addEqual(CExpPtrArg key, ExpPtrArg value, bool excluding)
{
    return add(key, make_ptr<Equals>(value), excluding);
}

ExpPtr Environment::replace(CExpPtrArg key, ExpPtrArg value, bool excluding)
{
    debugPrint("ENV UPD KEY: " + key->show() + "\n", true);
    debugPrint("    NEW: " + value->show() + "\n", true);
    data[key] = value;
    return data[key];
}

ExpPtr Environment::replaceEqual(CExpPtrArg key, ExpPtrArg value, bool excluding)
{
    return replace(key, make_ptr<Equals>(value), excluding);
}

std::vector<std::string> Environment::getAllNames() const
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

template <class T>
void addVariable(Environment* env)
{
    env->addEqual(make_ptr<Identifier>(T::defaultName), 
                  make_ptr<T>(), 
                  true);
}

void Environment::addDefaultDefinitions()
{
    addVariable<Void>(this);
    addVariable<DefaultOperator>(this);
    addVariable<Lambda>(this);
    addVariable<Then>(this);
    addVariable<Quote>(this);
    addVariable<Not>(this);
    addVariable<Print>(this);
    addVariable<Include>(this);
    addVariable<Inverse>(this);
    addVariable<Addition>(this);
    addVariable<Union>(this);
    addVariable<IntegerType>(this);
    addVariable<Intersection>(this);
    addVariable<Mutation>(this);
    addVariable<Subtraction>(this);
    addVariable<Equality>(this);
    addVariable<Less>(this);
    addVariable<More>(this);
    addVariable<Let>(this);
    addVariable<Any>(this);
    addVariable<PrintInfo>(this);
    addVariable<PrintEnv>(this);
    addVariable<ReverseApplication>(this);
    addVariable<LowPriorityApplication>(this);
    addEqual(make_ptr<Identifier>(";"), make_ptr<DefaultOperator>(), true);
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
