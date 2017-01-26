#include "Expressions/ValueInSet.h"
#include "Expressions/Equality.h"
#include "Expressions/Application.h"
#include "Expressions/Identifier.h"
#include "Expressions/Any.h"

std::string ValueInSet::show() const
{
    return "(\u0251 : " + this->set->show() + ")";
}

bool ValueInSet::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    auto taken = set->element(env);
    if (auto stillValueInSet = d_cast<ValueInSet>(taken))
    {
        auto ret = Application().unapplyVariables(e,
                                                  set,
                                                  make_ptr<Any>(),
                                                  env);
        return ret;
    }

    return taken->unapplyVariables(e, env);

}

ExpPtr ValueInSet::eval(Environment& env) const
{
    return set->element(env);
}

