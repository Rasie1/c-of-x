#include "Expressions/Predicate.h"
#include "Expressions/Identifier.h"
#include "System/Environment.h"
#include "Expressions/Operation.h"
#include "Expressions/Intersection.h"
#include "Expressions/Void.h"
#include "Expressions/Any.h"
#include "Expressions/ValueInSet.h"

ExpPtr Predicate::apply(ExpPtrArg e, Environment& env) const
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
    else
    {
        auto result = holds(expr, env) ? expr
                                       : make_ptr<Void>();

        if (auto id = d_cast<Identifier>(evaluated))
            env.add(id,
                    std::const_pointer_cast<Expression>(shared_from_this()));

        return result;
    }

}

ExpPtr Predicate::inverse() const
{
    // kind of dangerous
    return s_cast<Morphism>(std::const_pointer_cast<Expression>(shared_from_this()));
}


