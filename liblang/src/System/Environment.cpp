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
#include "System/Cast.h"

#include <vector>
#include <fstream>


inline Object intersect(const Object& l, const Object& r, Environment env)
{
    auto lp = checkType<Identifier>(env, l) ? 
        env.get(cast<Identifier>(env, l)->name) : l;
    auto rp = checkType<Identifier>(env, r) ? 
        env.get(cast<Identifier>(env, r)->name) : r;

    auto operation = makeOperation<Intersection>(lp, rp);
    auto result = operation;

    return result;
}

Environment::Environment(const std::shared_ptr<DebugPrinter>& debugPrinter) :
    debugPrinter(debugPrinter),
    defaultOperator(std::make_shared<Then>())
{
    addDefaultDefinitions();
}

void Environment::clear()
{
    data.clear();
    addDefaultDefinitions();
}

static Object unwrapEqual(const Object& value, Environment& env)
{
    if (auto eq = cast<Equals>(env, value))
        return eq->value;
    else if (checkType<Any>(env, value))
        return value;
    
    if (auto operation = cast<Operation>(env, value))
    if (checkType<Union>(env, operation->op))
    {
        auto l = unwrapEqual(operation->left, env);
        auto r = unwrapEqual(operation->right, env);
        return makeOperation<Union>(l, r);
    }
    if (checkType<Void>(env, value))
        return value;
    
    return makeObject<ValueInSet>(value);
}

Object Environment::getEqual(const std::string& key)
{
    auto value = get(key);

    return unwrapEqual(value, *this);
}

Object Environment::get(const std::string& key)
{
    debugPrint("ENV GET: (" + key + ") : (", true);

    auto x = data.find(key);

    Object ret;
    if (x == std::end(data))
        ret = makeObject<Any>();
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

void Environment::erase(const std::string& key)
{
    data.erase(key);
}

void Environment::add(const std::string& key, const Object& value, bool excluding)
{
    debugPrint("ENV ADD KEY: " + key + "\n", true);
    increaseDebugIndentation();
    debugPrint("NEW: " + value->show() + "\n", true);
    if (data.find(key) != data.end())
    {
        auto current = data[key];
        debugPrint("OLD: " + current->show() + "\n", true);
        auto intersection = intersect(current, value, *this);
        debugPrint("INT: " + intersection->show() + "\n", true);
        data[key] = intersection;
    }
    else
        data[key] = value;
    debugPrint("RET: " + data[key]->show() + "\n", true);
    decreaseDebugIndentation();
}

void Environment::addEqual(const std::string& key, const Object& value, bool excluding)
{
    add(key, makeObject<Equals>(value), excluding);
}

void Environment::replace(const std::string& key, const Object& value, bool excluding)
{
    debugPrint("ENV UPD KEY: " + key + "\n", true);
    debugPrint("NEW: " + value->show() + "\n", true);
    data[key] = value;
    // return data[key];
}

void Environment::replaceEqual(const std::string& key, const Object& value, bool excluding)
{
    replace(key, makeObject<Equals>(value), excluding);
}

std::vector<std::string> Environment::getAllNames() const
{
    std::vector<std::string> ret;
    ret.reserve(data.size());

    for (auto &x : data)
        ret.push_back(x.first);

    return ret;
}

bool Environment::operator==(const Environment& other) const
{
    return data == other.data;
}

template <class T>
void addVariable(Environment* env)
{
    env->addEqual(T::defaultName, 
                  makeObject<T>(), 
                  true);
}

void Environment::addDefaultDefinitions()
{
    addVariable<Void>(this);
    addVariable<DefaultOperator>(this);
    addVariable<Lambda>(this);
    addVariable<Then>(this);
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
    addEqual(";", makeObject<DefaultOperator>(), true);
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

std::string Environment::show() const
{
    std::string ret = "";
    for (auto &x : data)
        ret += x.first + " |--> " + x.second->show() + "\n";

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
