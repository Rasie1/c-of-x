#include "Function.h"
#include "Environment.h"
#include "Void.h"
#include "Equality.h"
#include "Any.h"
#include "Identifier.h"
#include "String.h"
#include "Operation.h"
#include "Intersection.h"
#include "Union.h"

// For some reason, here intersection of function and argument gives us a result.
// I'm not really sure if it is correct, but now it fits everything
ExpPtr Function::intersect(ExpPtrArg other, const Environment& env)
{
    auto value = Identifier::unwrapIfId(other, env);

    env.debugPrint("Intersecting function:" + this->show() + "\n");
    env.debugPrint("WITH: " + other->show() + "\n");

    if (checkType<Equals>(value))
    {
        auto eq = s_cast<Equals>(value);
        auto exactValue = eq->value;
        auto envCopy = env;
        auto applied = apply(exactValue, envCopy);
        if (checkType<Void>(applied))
            return applied;
        return make_ptr<Equals>(applied);
    }

    // Same thing; possibly there are incorrect cases. Needs some research
    if (checkType<Function>(value))
    {
        return make_ptr<Operation>(make_ptr<Union>(),
                                   shared_from_this(),
                                   other);
    }

    return make_ptr<Operation>(make_ptr<Intersection>(),
                               shared_from_this(),
                               other);
}

bool ReversibleFunction::unapplyVariables(ExpPtrArg e, ExpPtrArg arg, Environment& env) const
{
    auto reversed = reverse()->apply(arg, env);

    auto ret = reversed->unapplyVariables(e, env);

    return ret;
}
