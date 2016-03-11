#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"
#include "Lambda.h"
#include "Application.h"


bool Equals::holds(ExpPtrArg e, const Environment& env) const
{
    return *value == *e;
}

bool Equals::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const Equals&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string Equals::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string Equals::defaultName = "eq";

ExpPtr Equals::intersect(ExpPtrArg other, const Environment& env)
{
    if (checkType<Equals>(other))
    {
        auto p = s_cast<Equals>(other);
        auto& l = *value;
        auto& r = *(p->value);
        if (l == r)
            return value;
        else
            return make_ptr<Void>();
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), shared_from_this(), other);
}

Equality::Equality()
    : Operator(true, 1)
{

}

static bool operateHelper(ExpPtrArg first,
                          ExpPtrArg second,
                          Environment& env)
{
    auto lvalue = first;//Identifier::unwrapIfId(first, env);
    auto rvalue = second;

    auto ret = lvalue->unapplyVariables(rvalue, env);

    env.debugPrint("unapplied: (" + lvalue->show() + ") = (" + rvalue->show() + ")", true);

    return ret;
}

ExpPtr Equality::operate(ExpPtrArg first,
                         ExpPtrArg second,
                         Environment& env) const
{
    auto envl = env;
    auto envr = env;
    env.debugPrint("Equality: (" + first->show() + ") = (" + second->show() + ")\n", true);
    env.debugPrint("L R:\n", true);
    env.increaseDebugIndentation();
    auto l = operateHelper(first, second, envl);
    env.decreaseDebugIndentation();

    ExpPtr ret;
    if (l)
    {
        env = envl;
        ret = first;
    }
//    else if (r)
//    {
//        env = envr;
//        ret = second;
//    }
    else
        return make_ptr<Void>();

    return ret;
}

std::string Equality::show() const
{
    return Equality::defaultName;
}

const std::string Equality::defaultName = "=";

ExpPtr Equality::partialApply(ExpPtrArg e, Environment& env) const
{
     return make_ptr<Equals>(e);
}
