#include "Expressions/ValueInSet.h"
#include "Expressions/Equality.h"
#include "Expressions/Application.h"
#include "Expressions/Identifier.h"
#include "Expressions/Any.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Union.h"
#include "System/Environment.h"
#include "System/Cast.h"

std::string ValueInSet::show() const
{
    return "(\u0251 : " + this->set->show() + ")";
}

bool ValueInSet::unapplyVariables(const Object& e, Environment& env)
{
//    if (auto operation = d_cast<Operation>(e))
//    {
//        if (checkType<Intersection>(operation->op))
//        {

//        }
//    }
    auto element = set->element(env);
    if (auto stillValueInSet = d_cast<ValueInSet>(element))
    {
        auto ret = Application().unapplyVariables(e,
                                                  set,
                                                  make_ptr<Any>(),
                                                  env);
        return ret;
    }

    return element->unapplyVariables(e, env);

}

Object ValueInSet::intersect(const Object& other, const Environment& env)
{
    auto envc = env;
    if (checkType<Any>(this->set))
        return other;

    auto operation = make_ptr<Operation>(make_ptr<Application>(), this->set, other);
    auto ret = operation->eval(envc);

    return ret;
}

Object ValueInSet::eval(Environment& env) const
{
    return set->element(env);
}

