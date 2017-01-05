#include "Predicate.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"
#include "Expressions/Any.h"

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

    auto ret = holds(expr, env) ? expr : make_ptr<Void>();

    return ret;
}

std::shared_ptr<Function> Predicate::reverse() const
{
    // kind of dangerous
    return s_cast<Function>(std::const_pointer_cast<Expression>(shared_from_this()));
}
