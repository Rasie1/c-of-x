#include "IntegerType.h"
#include "Integer.h"
#include "Void.h"

ExpPtr IntegerType::apply(ExpPtrArg e, Environment*& env) const
{
    auto x = d_cast<Integer>(e);

    if (x != nullptr)
        return x;
    else
        return make_ptr<Void>();
}

std::string IntegerType::show() const
{
    return defaultName;
}

const std::string IntegerType::defaultName = "int";
