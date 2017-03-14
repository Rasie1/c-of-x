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

ExpPtr Predicate::apply(ExpPtrArg e, Environment& env)
{
    auto evaluated = e->eval(env);

    auto expr = Identifier::unwrapIfId(evaluated, env);

    if (checkType<Any>(expr))
    {
        if (auto id = d_cast<Identifier>(evaluated))
        {
            env.add(id,
                    std::const_pointer_cast<Expression>(shared_from_this()));
            return evaluated;
        }
        else
            return make_ptr<ValueInSet>(std::const_pointer_cast<Expression>(shared_from_this()));

    }
    else if (auto id = d_cast<Identifier>(evaluated))
    {
        env.add(id, std::const_pointer_cast<Expression>(shared_from_this()));
        return id;
    }
    else
    {
        ExpPtr result;
        if (auto option = holds(expr, env))
            result =  *option ? expr
                              : make_ptr<Void>();
        else
            result = make_ptr<Operation>(make_ptr<Application>(),
                                         std::const_pointer_cast<Expression>(shared_from_this()),
                                         expr);
        return result;
    }

}

ExpPtr Predicate::inverse() const
{
    // kind of dangerous
    return d_cast<Morphism>(std::const_pointer_cast<Expression>(shared_from_this()));
}


