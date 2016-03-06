#include "ValueInSet.h"

std::string ValueInSet::show() const
{
    return "(\u0251 : " + this->set->show() + ")";
}

