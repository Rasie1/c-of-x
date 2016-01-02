#include "EvalDelay.h"
#include "Application.h"

namespace Language
{

ExpPtr EvalDelay::apply(ExpPtrArg e, Environment*& env) const
{
    return e;
}

std::string EvalDelay::show() const
{
    return defaultName;
}

const std::string EvalDelay::defaultName = "\'";

}
