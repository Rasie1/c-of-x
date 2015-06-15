#include "EvalDelay.h"
#include "Application.h"

namespace Language
{

Expression* EvalDelay::apply(Expression* e, Environment*& env)
{
    return e;
}

std::string EvalDelay::toString() const
{
    return defaultName;
}

const std::string EvalDelay::defaultName = "\'";

}
