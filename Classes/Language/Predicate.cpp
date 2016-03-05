#include "Predicate.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Intersection.h"
#include "Void.h"
#include "Any.h"

ExpPtr Predicate::apply(ExpPtrArg e, Environment& env) const
{
    if (checkType<Identifier>(e))
    {
        auto id = s_cast<Identifier>(e);
        ExpPtr x = std::const_pointer_cast<Expression>(shared_from_this());
        env.add(id, x);

        auto expr = env.getEqual(id);

        if (d_cast<Any>(expr) || holds(expr, env))
            return e;

        auto ret = env.intersect(std::const_pointer_cast<Expression>(shared_from_this()), id);

        return ret;
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
