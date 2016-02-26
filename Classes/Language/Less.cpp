#include "Less.h"
#include "Equality.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"
#include "Lambda.h"
#include "Integer.h"


bool LessThan::holds(ExpPtrArg e, const Environment& env) const
{
    if (typeid(*e) == typeid(Integer))
    {
        auto v1 = s_cast<Integer>(value);
        auto v2 = s_cast<Integer>(e);
        return v2->value < v1->value;
    }
    return *value == *e;
}

bool LessThan::operator==(const Expression& other) const
{
    if (typeid(*this) == typeid(other))
    {
        auto x = static_cast<const LessThan&>(other);
        return value == x.value;
    }
    else
        return false;
}

std::string LessThan::show() const
{
    return defaultName + "(" + value->show() + ")";
}

const std::string LessThan::defaultName = "lessThan";

ExpPtr LessThan::intersect(ExpPtrArg other, const Environment& env)
{
    if (typeid(*this) == typeid(*other))
    {
        auto p = s_cast<LessThan>(other);
        if (typeid(*value) == typeid(Integer) &&
            typeid(*(p->value)) == typeid(Integer))
        {
            auto v1 = s_cast<Integer>(value);
            auto v2 = s_cast<Integer>(p->value);
            return make_ptr<LessThan>(make_ptr<Integer>(std::min(v1->value, v2->value)));
        }
        return make_ptr<Void>();
//        return make_ptr<LessThan>(make_ptr<Integer>(value));
    }
    return make_ptr<Operation>(make_ptr<Intersection>(), value, other);
}

Less::Less()
    : Operator(true, 2)
{

}

static bool operateHelper(ExpPtrArg first,
                          ExpPtrArg second,
                          Environment& env)
{
    auto lvalue = first;
    auto rvalue = second;
    if (typeid(*lvalue) != typeid(Identifier))
        while (lvalue->unwind(lvalue, rvalue, env));

    if (typeid(*lvalue) == typeid(Identifier))
    {
        auto id = s_cast<Identifier>(lvalue);


        rvalue = Identifier::unwrapIfId(rvalue, env);

        env.addEqual(id, rvalue->eval(env));
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

ExpPtr Less::operate(ExpPtrArg first,
                     ExpPtrArg second,
                     Environment& env) const

{
    auto f = s_cast<LessThan>(partialApply(second, env));

    return f->apply(first, env);


    auto l = LessThan(second).holds(Identifier::unwrapIfId(first, env), env);

    ExpPtr ret;
    if (l)
        ret = first;
    else
        return make_ptr<Void>();


    return ret;
}

std::string Less::show() const
{
    return Equality::defaultName;
}

const std::string Less::defaultName = "<";

ExpPtr Less::partialApply(ExpPtrArg e, Environment& env) const
{
     return make_ptr<LessThan>(e);
}
