#include "Expressions/Predicate.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"
#include "Expressions/ValueInSet.h"
#include "Expressions/Application.h"
#include "System/Cast.h"

Object Predicate::apply(const Object& e, Environment& env)
{
    auto evaluated = e;

    auto expr = Identifier::unwrapIfId(evaluated, env);

    if (checkType<Any>(env, expr))
    {
        if (auto id = cast<Identifier>(env, evaluated))
        {
            env.add(id->name,
                    thisObject());
            return evaluated;
        }
        else
            return makeObject<ValueInSet>(thisObject());

    }
    else if (auto id = cast<Identifier>(env, evaluated))
    {
        env.add(id->name, thisObject());
        return evaluated;
    }
    else
    {
        Object result;
        if (auto option = holds(expr, env))
            result =  *option ? expr
                              : makeObject<Void>();
        else
            result = makeOperation<Application>(thisObject(), expr);
        return result;
    }

}

Object Predicate::inverse()
{
    // kind of dangerous
    return thisObject();
}


