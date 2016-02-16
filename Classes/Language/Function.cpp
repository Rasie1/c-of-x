#include "Function.h"
#include "Environment.h"
#include "Void.h"
#include "Predicate.h"

ExpPtr Function::intersect(ExpPtrArg other, Environment *env)
{
    auto x = d_cast<Equals>(other);

    ExpPtr val;
    if (x == nullptr)
    {
        val = env->getEqual(other);

        if (val == nullptr)
            return make_ptr<Void>();
    }
    else
        val = x->value;

    return apply(val, env); // what about commutative ops?
}
