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
    if (checkType<Identifier>(e))
    {
        auto id = s_cast<Identifier>(e);
        ExpPtr x = std::const_pointer_cast<Expression>(shared_from_this());
        env.add(id, x, true);

        auto expr = env.getEqual(id);

        //auto ret = env.intersect(std::const_pointer_cast<Expression>(shared_from_this()), id);
        // it gets Any because right now it's "int" and isn't equal to anything

        // some expressions should be intersected at first, other should be iterated
        // to be more effective

        // not sure if "any" here is appropriate:

//        if (d_cast<Any>(expr) || holds(expr, env))
//            return e;


        return id;
    }

    auto expr = e->eval(env);

    if (checkType<Any>(e))
        return make_ptr<ValueInSet>(std::const_pointer_cast<Expression>(shared_from_this()));

    auto ret = holds(expr, env) ? expr : make_ptr<Void>();

    return ret;
}

ExpPtr Predicate::reverse() const
{
    // kind of dangerous
    return s_cast<Morphism>(std::const_pointer_cast<Expression>(shared_from_this()));
}
