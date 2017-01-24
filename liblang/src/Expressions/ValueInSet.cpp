#include "Expressions/ValueInSet.h"
#include "Expressions/Equality.h"
#include "Expressions/Application.h"
#include "Expressions/Identifier.h"

std::string ValueInSet::show() const
{
    return "(\u0251 : " + this->set->show() + ")";
}

bool ValueInSet::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    // TODO: prove that there will be no shadowing problems
    auto id = make_ptr<Identifier>("id"); 
    return Application().unapplyVariables(e, set, id, env);
}

ExpPtr ValueInSet::eval(Environment& env) const
{
    return set->takeValue(env);
}

