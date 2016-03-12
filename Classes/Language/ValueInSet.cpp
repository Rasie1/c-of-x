#include "ValueInSet.h"
#include "Equality.h"

std::string ValueInSet::show() const
{
    return "(\u0251 : " + this->set->show() + ")";
}

ExpPtr ValueInSet::eval(Environment& env) const
{
    return make_ptr<ValueInSet>(set);
}

