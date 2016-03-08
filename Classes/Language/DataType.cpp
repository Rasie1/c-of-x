#include "DataType.h"

ExpPtr DataType::eval(Environment& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
}

bool DataType::unapplyVariables(ExpPtrArg e, Environment& env) const
{
    return *this == *e;
}
