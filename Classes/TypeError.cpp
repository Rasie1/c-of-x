#include "TypeError.h"

Expression* TypeError::evalConstEnv(Environment* env)
{
    return this;
}

std::string TypeError::show() const
{
    return "Typing Error";
}
