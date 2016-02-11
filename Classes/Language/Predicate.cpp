#include "Predicate.h"
#include "Identifier.h"
#include "Environment.h"
#include "Operation.h"
#include "Lambda.h"
#include "Closure.h"
#include "Void.h"

ExpPtr Predicate::apply(ExpPtrArg e, Environment*& env) const
{
    ExpPtr value;
    if (d_cast<Identifier>(e))
        value = env->get(e);
    if (value == nullptr)
        value = e;

    if (holds(e, env))
        return e;
    else
        return make_ptr<Void>();
}
