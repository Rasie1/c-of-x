#include "TypeError.h"

namespace Language
{

Expression* TypeError::evalConstEnv(Environment* env)
{
    return this;
}

std::string TypeError::toString() const
{
    return "Type Error";
}

}
