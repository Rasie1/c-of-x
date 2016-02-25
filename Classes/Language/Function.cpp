#include "Function.h"
#include "Environment.h"
#include "Void.h"
#include "Equality.h"
#include "Any.h"
#include "Identifier.h"

ExpPtr Function::intersect(ExpPtrArg other, const Environment& envc)
{
    auto env = envc;
    ExpPtr val;
    if (typeid(*other) == typeid(Equals))
    {
        auto eq = s_cast<Equals>(other);
        return apply(eq->value, env);
    }
    else
    {
        auto val = Identifier::unwrapIfId(other, env);
        if (typeid(*val) == typeid(Any))
            return make_ptr<Void>();
        else
            return apply(val, env);
    }
}
