#include "DataType.h"


ExpPtr DataType::eval(Environment*& env) const
{
    return std::const_pointer_cast<Expression>(shared_from_this());
}
