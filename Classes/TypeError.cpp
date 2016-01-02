#include "TypeError.h"

namespace Language
{

Expression* TypeError::evalConstEnv(Environment* env)
{
    return this;
}

std::string TypeError::show() const
{
    return "Type Error";
}

}
