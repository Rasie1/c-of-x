#include "Lambda.h"
#include <iostream>
#include "UnknownName.h"
#include "Pattern.h"
#include "Closure.h"
#include "Function.h"
#include "Environment.h"

namespace Language
{

ExpPtr Lambda::apply(const ExpPtr& e, Environment*& env) const
{
    return std::make_shared<LambdaArguments>(e->pattern());
}

std::string Lambda::toString() const
{
    return defaultName;
}

const std::string Lambda::defaultName = "\\";


LambdaArguments::LambdaArguments(const PatPtr& argument)
    : pattern(argument)
{

}

ExpPtr LambdaArguments::apply(const ExpPtr& e, Environment*& env) const
{
    auto ret = std::make_shared<Closure>(std::make_shared<Function>(e, pattern),
                                         env);
    return ret;
}

std::string LambdaArguments::toString() const
{
    return "\u03BB" + pattern->toString() + ".";
}

}
