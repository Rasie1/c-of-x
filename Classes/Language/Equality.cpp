#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"



bool Equals::holds(ExpPtrArg e, Environment* env) const
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

const std::string Equals::defaultName = "eqv";

ExpPtr Equals::intersect(ExpPtrArg other, Environment* env)
{
    if (typeid(*this) == typeid(*other))
    {
        auto p = s_cast<Equals>(other);
        auto& l = *value;
        auto& r = *(p->value);
        if (l == r)
            return value;
        else
            return make_ptr<Void>();
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), value, other);
}

Equality::Equality()
    : Operator(true, 2)
{

}

bool operateHelper(ExpPtrArg first,
                   ExpPtrArg second,
                   Environment*& env)
{

    auto lvalue = first;
    auto rvalue = second;
    if (typeid(*lvalue) != typeid(Identifier))
        while (lvalue->unwind(lvalue, rvalue, env));

    if (typeid(*lvalue) == typeid(Identifier))
    {
        auto id = s_cast<Identifier>(lvalue);
        env->addEqual(id, rvalue);

        return true;
    }
    else
    {
        auto le = first->eval(env);
        auto re = second->eval(env);
        re = Identifier::unwrapIfId(re, env);

        return *le == *re;
    }
}

ExpPtr Equality::operate(ExpPtrArg first,
                         ExpPtrArg second,
                         Environment*& env) const

{
    auto l = operateHelper(first, second, env);
    auto r = operateHelper(second, first, env);

    if (l && r)
        return first; // not always true
    else
        return make_ptr<Void>();

//    // How recursion was implemented
//    auto operation = d_cast<Operation>(rvalue);
//    if (operation && d_cast<Lambda>(operation->op))
//        env->addEqual(lvalue, rvalue);

}

std::string Equality::show() const
{
    return Equality::defaultName;
}

const std::string Equality::defaultName = "=";


//ExpPtr Equality::apply(ExpPtrArg e, Environment*& env) const
//{
//    return make_ptr<Equals>(e);
//}

//std::string Equality::show() const
//{
//    return defaultName;
//}

//const std::string Equality::defaultName = "eq";
