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
    return "(_ : " + this->set->show() + ")";
}

bool ValueInSet::unapplyVariables(const Object& e, Environment& env)
{
    auto element = set->element(env);
    if (auto stillValueInSet = cast<ValueInSet>(env, element))
    {
        auto ret = Application().unapplyVariables(e,
                                                  set,
                                                  makeObject<Any>(),
                                                  env);
        return ret;
    }

    return element->unapplyVariables(e, env);

}

Object ValueInSet::intersect(const Object& other, Environment& env)
{
    auto envc = env;
    if (checkType<Any>(envc, this->set))
        return other;

    auto operation = makeOperation<Application>(this->set, other);
    auto ret = operation;//->eval(envc);

    return ret;
}

Object ValueInSet::eval(Environment& env)
{
    auto result = set->element(env);

    if (auto v = std::dynamic_pointer_cast<ValueInSet>(result.expression))
    if (v->set == set)
        return thisObject();

    return result;
}

