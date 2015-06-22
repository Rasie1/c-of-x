#include "EvalDelay.h"
#include "Application.h"

namespace Language
{

ExpPtr EvalDelay::apply(const ExpPtr& e, Environment*& env) const
{
    return e;
}

std::string EvalDelay::toString() const
{
    return defaultName;
}

const std::string EvalDelay::defaultName = "\'";

}
