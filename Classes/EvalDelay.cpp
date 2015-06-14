#include "EvalDelay.h"
#include "Application.h"

namespace Language
{

Expression* EvalDelay::applyConstEnv(Expression* e, Environment* env)
{
    return e;
}

std::string EvalDelay::toString() const
{
    return defaultName;
}

const std::string EvalDelay::defaultName = "\'";

}
